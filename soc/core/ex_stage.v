`include "cpu.vh"

module ex_stage(
  input                         clk             ,
  input                         reset           ,
  // allouin
  input                         ms_allowin      ,
  output                        es_allowin      ,
  // from ds
  input                         ds_to_es_valid  ,
  input [`DS_TO_ES_BUS_WD-1:0]  ds_to_es_bus    ,
  // to ms
  output                        es_to_ms_valid  ,
  output [`ES_TO_MS_BUS_WD-1:0] es_to_ms_bus    ,
  // to data sram
  output                        cpu_data_en     ,
  output [3                 :0] cpu_data_wen    ,
  output [31                :0] cpu_data_addr   ,
  output [31                :0] cpu_data_wdata  ,
  // to ds
  output                        es_valid        ,
  output [4                 :0] es_rf_waddr                           
);
// wire es_valid;
wire es_ready_go  = 1'b1;

assign es_allowin = (es_ready_go && ms_allowin) || ~es_valid;

// es_valid
sirv_gnrl_dfflr #(
  .DW(1)
) u_esvld_vec_1_dff (
  .clk  (clk),
  .reset(reset),
  .lden (es_allowin),
  .dnxt (ds_to_es_valid),
  .qout (es_valid)
);

wire [`DS_TO_ES_BUS_WD-1:0]  ds_to_es_bus_r;

sirv_gnrl_dfflr #(
  .DW(`DS_TO_ES_BUS_WD)
) u_es_bus_vec_dff (
  .clk  (clk),
  .reset(reset),
  .lden (es_allowin && ds_to_es_valid),
  .dnxt (ds_to_es_bus),
  .qout (ds_to_es_bus_r)
);

// ctrl signals
// ex
wire [11:0]       es_alu_op;
wire              es_src1_is_sa;
wire              es_src1_is_pc;
wire              es_src2_is_imm;
wire              es_src2_is_8;
// mem
wire              es_load_op;
wire              es_mem_we;
// wb
wire              es_rf_we;

wire [15:0]       es_imm;
// wire [4:0]        es_rf_waddr;
wire [31:0]       es_rs_data;
wire [31:0]       es_rt_data;
wire [`XLEN-1:0]  es_pc;

assign {es_alu_op,
        es_src1_is_sa, 
        es_src1_is_pc, 
        es_src2_is_imm,
        es_src2_is_8,
        es_load_op,
        es_mem_we,
        es_rf_we,
        es_imm,
        es_rf_waddr,
        es_rs_data,
        es_rt_data,
        es_pc
        } = ds_to_es_bus_r;

wire [31:0] es_alu_src1;
wire [31:0] es_alu_src2;
wire [31:0] es_alu_result;

wire [31:0] es_sa  = {27'b0, es_imm[10:6]};
assign es_alu_src1 = es_src1_is_sa ? es_sa : (es_src1_is_pc ? es_pc : es_rs_data);
assign es_alu_src2 = es_src2_is_imm ? {{16{es_imm[15]}}, es_imm[15:0]} : (es_src2_is_8 ? 32'd8 : es_rt_data);

alu u_alu(
  .alu_op     (es_alu_op),
  .alu_src1   (es_alu_src1),
  .alu_src2   (es_alu_src2),
  .alu_result (es_alu_result)
);

// to ms
assign es_to_ms_valid = es_valid && es_ready_go;
assign es_to_ms_bus   = {es_pc, 
                         es_load_op, 
                         es_rf_we,
                         es_rf_waddr,
                         es_alu_result
                         };

// for data sram
assign cpu_data_en     = es_load_op;
assign cpu_data_wen    = {4{es_mem_we && es_valid}};
assign cpu_data_addr   = es_alu_result;
assign cpu_data_wdata  = es_rt_data;
endmodule