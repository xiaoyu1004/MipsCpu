`include "cpu.vh"

module id_stage (
  input                         clk           ,
  input                         reset         ,
  // allowin
  input                         es_allowin    ,
  output                        ds_allowin    ,
  // from fs
  input                         fs_to_ds_valid,
  input  [`FS_TO_DS_BUS_WD-1:0] fs_to_ds_bus  ,
  // to fs
  output [`BR_BUS_WD      -1:0] br_bus        ,
  // to es
  output                        ds_to_es_valid,
  output [`DS_TO_ES_BUS_WD-1:0] ds_to_es_bus  ,
  // to rf
  output [4                 :0] rs_addr       ,
  output [4                 :0] rt_addr       ,
  // from rf
  input  [31                :0] rs_data       ,
  input  [31                :0] rt_data       ,
  // from ex
  input                         es_valid      ,
  input  [4                 :0] es_rf_waddr   ,
  // from mem
  input                         ms_valid      ,
  input  [4                 :0] ms_rf_waddr   ,
  // from wb
  input                         ws_valid      ,
  input  [4                 :0] ws_rf_waddr    
);

// ctrl signals
// ex
wire [11:0] alu_op;
wire        src1_is_sa;
wire        src1_is_pc;
wire        src2_is_imm;
wire        src2_is_8;
// mem
wire        load_op;
wire        mem_we;
// wb
wire        rf_we;

wire [15:0]       imm;
wire [4:0]        rf_waddr;

wire ds_valid;
wire ds_ready_go;

wire rs_hazard = ((es_valid && rs_addr == es_rf_waddr) || 
                  (ms_valid && rs_addr == ms_rf_waddr) ||
                  (ws_valid && rs_addr == ws_rf_waddr)) && rs_addr != 5'b0;

wire rt_hazard = ((es_valid && rt_addr == es_rf_waddr) || 
                  (ms_valid && rt_addr == ms_rf_waddr) ||
                  (ws_valid && rt_addr == ws_rf_waddr)) && rt_addr != 5'b0;

assign ds_ready_go = !rs_hazard && !rt_hazard;

// to es
assign ds_to_es_valid = ds_valid && ds_ready_go;
assign ds_to_es_bus   = {alu_op,
                        src1_is_sa,
                        src1_is_pc,
                        src2_is_imm,
                        src2_is_8,
                        load_op,
                        mem_we,
                        rf_we,
                        imm,
                        rf_waddr,
                        rs_data,
                        rt_data,
                        ds_pc
                        };

assign ds_allowin = (ds_ready_go && es_allowin) || ~ds_valid;

// ds_valid
sirv_gnrl_dfflr #(
  .DW(1)
) u_dsvld_vec_1_dff (
  .clk  (clk),
  .reset(reset),
  .lden (ds_allowin),
  .dnxt (fs_to_ds_valid),
  .qout (ds_valid)
);

// ds_pc && ds_inst
wire [`FS_TO_DS_BUS_WD-1:0] fs_to_ds_bus_r;

sirv_gnrl_dfflr #(
  .DW(`FS_TO_DS_BUS_WD)
) u_ds_bus_vec_dff (
  .clk  (clk),
  .reset(reset),
  .lden (ds_allowin && fs_to_ds_valid),
  .dnxt (fs_to_ds_bus),
  .qout (fs_to_ds_bus_r)
);

wire [31:0] ds_pc;
wire [31:0] ds_inst;
assign {ds_pc, ds_inst} = fs_to_ds_bus_r;

// parse op/rs/rt/rd/imm/jidx/sa/func
wire [5:0]  opcode    = ds_inst[31:26];
assign      rs_addr   = ds_inst[25:21];
assign      rt_addr   = ds_inst[20:16];
wire [4:0]  rd_addr   = ds_inst[15:11];
// for I-Type
assign      imm       = ds_inst[15:0];
// for J-Type
wire [25:0] jidx      = ds_inst[25:0];
// for R-Type
wire [4:0]  sa        = ds_inst[10:6];
wire [5:0]  func      = ds_inst[5:0];

// decoder
wire [63:0] opcode_d;
wire [63:0] func_d;

decoder_6_64 u_dec_op   (.in(opcode), .out(opcode_d));
decoder_6_64 u_dec_func (.in(func), .out(func_d));

// parse inst type
wire inst_addu  = opcode_d[0] && func_d[33];
wire inst_addiu = opcode_d[9];
wire inst_subu  = opcode_d[0] && func_d[35];

wire inst_lw    = opcode_d[35];
wire inst_sw    = opcode_d[43];

wire inst_beq   = opcode_d[4];
wire inst_bne   = opcode_d[5];

wire inst_jal   = opcode_d[3];
wire inst_jr    = opcode_d[0] && func_d[8];

wire inst_slt   = opcode_d[0] && func_d[42];
wire inst_sltu  = opcode_d[0] && func_d[43];

wire inst_sll   = opcode_d[0] && func_d[0];
wire inst_srl   = opcode_d[0] && func_d[2];
wire inst_sra   = opcode_d[0] && func_d[3];

wire inst_lui   = opcode_d[15];

wire inst_and   = opcode_d[0] && func_d[36];
wire inst_or    = opcode_d[0] && func_d[37];
wire inst_xor   = opcode_d[0] && func_d[38];
wire inst_nor   = opcode_d[0] && func_d[39];

// alu_op
assign alu_op[ 0] = inst_addu | inst_addiu | inst_lw | inst_sw;
assign alu_op[ 1] = inst_subu;
assign alu_op[ 2] = inst_slt;
assign alu_op[ 3] = inst_sltu;
assign alu_op[ 4] = inst_sll;
assign alu_op[ 5] = inst_srl;
assign alu_op[ 6] = inst_sra;
assign alu_op[ 7] = inst_lui;
assign alu_op[ 8] = inst_and;
assign alu_op[ 9] = inst_or;
assign alu_op[10] = inst_xor;
assign alu_op[11] = inst_nor;

assign src1_is_sa = inst_sll | inst_srl | inst_sra;
assign src1_is_pc = inst_jal;

assign src2_is_imm  = inst_addiu | inst_lw | inst_sw | inst_lui;
assign src2_is_8    = inst_jal;

assign load_op  = inst_lw;
assign mem_we   = inst_sw;

assign rf_we = ds_valid && ~inst_sw && ~inst_beq && ~inst_bne && ~inst_jr;

wire dst_is_r31 = inst_jal;
wire dst_is_rt  = inst_addiu | inst_lw | inst_lui;

assign rf_waddr = dst_is_r31 ? 5'd31 : (dst_is_rt ? rt_addr : rd_addr);

// for branch
wire        rs_eq_rt        = rs_data == rt_data;
wire [31:0] fs_pc           = fs_to_ds_bus[63:32];
wire [31:0] imm_shift_sext  = {{16{imm[13]}}, imm[13:0], 2'b0};
wire [31:0] jal_target      = {fs_pc[31:28], jidx, 2'b0};

wire        br_taken;
wire [31:0] br_target;

assign br_taken  = ((inst_beq && rs_eq_rt) || (inst_bne && ~rs_eq_rt) || inst_jal || inst_jr) && ds_valid;
assign br_target = (inst_beq || inst_bne) ? (fs_pc + imm_shift_sext) : (inst_jal ? jal_target : (inst_jr ? rs_data : 32'b0));

assign br_bus = {br_taken, br_target};

endmodule