#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

#include <verilated.h>
#if VM_TRACE
#include <verilated_vcd_c.h>  // Trace file format header
#endif

vluint64_t main_time = 0;  // Current simulation time
                           // This is a 64-bit integer to reduce wrap over issues and
                           // allow modulus.  You can also use a double, if you wish.

double sc_time_stamp() {  // Called by $time in Verilog
    return main_time;     // converts to double, to match
                          // what SystemC does
}

VTop* top;  // target design
#ifdef VM_TRACE
VerilatedVcdC* tfp;
#endif

const long timeout = 10000L;

void tick() {
    top->clk = 1;
    top->eval();
#if VM_TRACE
    if (tfp) tfp->dump((double)main_time);
#endif  // VM_TRACE
    main_time++;

    top->clk = 0;
    top->eval();
#if VM_TRACE
    if (tfp) tfp->dump((double)main_time);
#endif  // VM_TRACE
    main_time++;
}

static unsigned inst_cnt = 0;

static void read_bin(std::string& fname, std::vector<int>& vec) {
    std::ifstream fs(fname);
    if (!fs.is_open()) {
        printf("test asm file inst open fail");
        abort();
    }

    std::cout << "load inst success: " << fname << std::endl;

    std::string line;
    while (std::getline(fs, line)) {
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        unsigned hex;
        sscanf(line.c_str(), "%x", &hex);
        unsigned reverse_hex = ((hex >> 24) & 0xff) | ((hex >> 8) & 0xff00) |
            ((hex << 8) & 0xff0000) | ((hex << 24) & 0xff000000);
        vec.push_back(reverse_hex);
    }

    fs.close();
}

std::ifstream open_trace(std::string& tfile) {
    std::ifstream fs(tfile);
    if (!fs.is_open()) {
        std::cout << "sim asm program open fail \n";
        exit(-1);
    }
    return fs;
}

const std::string get_trace_line(std::ifstream& fs) {
    std::string line;
    if (!std::getline(fs, line)) {
        std::cout << "error! trace has no content yet! \n";
        exit(-1);
    }
    return line;
}

std::vector<std::string> split(std::string str, std::string pattern) {
    std::istringstream sstr(str);
    std::vector<std::string> result;

    std::string s;
    while (sstr >> s) {
        result.push_back(s);
    }

    return result;
}

bool verify(VTop* top, std::ifstream& fs) {
    if (!top->debug_wb_rf_we) {
        return true;
    }

    std::string trace_line       = get_trace_line(fs);
    std::vector<std::string> res = split(trace_line, " ");
    if (res.size() != 3) {
        std::cout << "error! trace line size must be eq to 3 \n";
    }

    int trace_wb_pc    = (int)stol(res[0], nullptr, 16);
    int trace_rf_waddr = (int)stol(res[1], nullptr, 10);
    int trace_rf_wdata = (int)stol(res[2], nullptr, 16);

    bool not_match = trace_wb_pc != (int)top->debug_wb_pc ||
        trace_rf_waddr != (int)top->debug_wb_rf_waddr ||
        trace_rf_wdata != (int)top->debug_wb_rf_wdata;

    printf(
        "debug_wb_pc: %x, debug_rf_waddr: %d, debug_rf_wdata: %x vs trace_wb_pc: %x, "
        "trace_rf_waddr: %d, trace_rf_wdata: %x \n",
        (int)top->debug_wb_pc, (int)top->debug_wb_rf_waddr, (int)top->debug_wb_rf_wdata,
        trace_wb_pc, trace_rf_waddr, trace_rf_wdata);

    if (not_match) {
        std::cout << "error occur! debug and trace not match, "
                  << (trace_wb_pc != (int)top->debug_wb_pc) << " "
                  << (trace_rf_waddr != (int)top->debug_wb_rf_waddr) << " "
                  << (trace_rf_wdata != (int)top->debug_wb_rf_wdata) << " \n";
        return false;
    }

    return true;
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);  // Remember args
    top = new VTop;                      // target design

    // init inst ram
    std::vector<int> inst_vec;
    std::string inst_fname(argv[1]);
    read_bin(inst_fname, inst_vec);
    std::copy(inst_vec.data(), inst_vec.data() + inst_vec.size(),
              top->Top__DOT__u_inst_sram__DOT__mem);

    // init data ram
    std::vector<int> data_vec;
    std::string data_fname(argv[2]);
    read_bin(data_fname, data_vec);
    std::copy(data_vec.data(), data_vec.data() + data_vec.size(),
              top->Top__DOT__u_data_sram__DOT__mem);

    inst_cnt = inst_vec.size();
    std::cout << "Starting simulation! inst count: " << inst_cnt << "\n";

    std::string d;
    const size_t idx = inst_fname.rfind('.');
    if (std::string::npos != idx) {
        d = inst_fname.substr(0, idx);
    }
    std::string vcd_fname   = d + ".vcd";
    std::string trace_fname = d + ".trace";

#if VM_TRACE                       // If verilator was invoked with --trace
    Verilated::traceEverOn(true);  // Verilator must compute traced signals
    VL_PRINTF("Enabling waves...\n");
    tfp = new VerilatedVcdC;
    top->trace(tfp, 99);                                // Trace 99 levels of hierarchy
    tfp->open(argc > 4 ? argv[4] : vcd_fname.c_str());  // Open the dump file
#endif

    // reset
    top->reset = 0;
    for (size_t i = 0; i < 5; i++) {
        tick();
    }

    // start
    top->reset = 1;

    bool ret        = false;
    bool run_finish = false;

    std::ifstream fs = open_trace(trace_fname);

    while (!run_finish && main_time < timeout) {
        tick();

        run_finish = top->debug_wb_pc >= (inst_cnt * 4 - 4);
        if (run_finish) {
            break;
        }

        ret = verify(top, fs);
        if (!ret) {
            break;
        }
    }

    // Run for 10 more clocks
    for (size_t i = 0; i < 10; i++) {
        tick();
    }

    std::cout << "Simulation terminated at time " << main_time << " cycle " << std::endl;

#if VM_TRACE
    if (tfp) tfp->close();
    delete tfp;
#endif
    delete top;

    std::cout << "simulation finish! program run " << (run_finish ? "finish;" : "not finish;")
              << " result : " << (ret ? " PASS " : " FAILED ") << std::endl;

    return 0;
}