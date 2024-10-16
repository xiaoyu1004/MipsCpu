`include "cpu.vh"

module if_stage (
  input                         clk             ,
  input                         reset           ,
  // allowin
  input                         ds_allowin      ,
  // br bus
  input [`BR_BUS_WD-1:0]        br_bus          ,
  // inst ram interface
  output                        inst_sram_en    ,
  output [3:0]                  inst_sram_wen   ,
  output [`XLEN-1:0]            inst_sram_addr  ,
  output [`XLEN-1:0]            inst_sram_wdata ,
  input  [`XLEN-1:0]            inst_sram_rdata ,
  // to ds
  output                        fs_to_ds_valid  ,
  output [`FS_TO_DS_BUS_WD-1:0] fs_to_ds_bus
);

wire              br_taken;
wire [`XLEN-1:0]  br_target;

assign {br_taken, br_target} = br_bus;

wire [`XLEN-1:0]      fs_pc;
wire [`INST_LEN-1:0]  fs_inst;

// pre-if stage
wire [`XLEN-1:0] seq_pc;
wire [`XLEN-1:0] next_pc;

assign seq_pc = fs_pc + 32'h4;

mux21 #(
  .DW(`XLEN)
) u_pc_mux21 (
  .sel(br_taken),
  .in0(seq_pc),
  .in1(br_target),
  .out(next_pc)
);

wire to_fs_valid  = reset;

// if stage
wire fs_valid;

wire fs_ready_go  = 1'b1;
wire fs_allowin   = (fs_ready_go && ds_allowin) || ~fs_valid;

// fs_valid
sirv_gnrl_dfflr #(
  .DW(1)
) u_fsvld_vec_1_dff (
  .clk  (clk),
  .reset(reset),
  .lden (fs_allowin),
  .dnxt (to_fs_valid),
  .qout (fs_valid)
);

// fs_pc
sirv_gnrl_dfflrsv #(
  .DW(`XLEN)
) u_fs_pc_vec_32_dff (
  .clk  (clk),
  .reset(reset),
  .rsv  (32'hfffffffc),
  .lden (fs_allowin && to_fs_valid),
  .dnxt (next_pc),
  .qout (fs_pc)
);

// to inst ram
assign inst_sram_en    = to_fs_valid && fs_allowin;
assign inst_sram_wen   = 4'b0;
assign inst_sram_addr  = next_pc;
assign inst_sram_wdata = `XLEN'b0;
// from inst ram
assign fs_inst         = inst_sram_rdata;

// to ds
assign fs_to_ds_valid = fs_valid && fs_ready_go;
assign fs_to_ds_bus   = {fs_pc, fs_inst};

endmodule