`include "cpu.vh"

module wb_stage(
  input                         clk               ,
  input                         reset             ,
  // allowin
  output                        ws_allowin        ,
  // from ms
  input                         ms_to_ws_valid    ,
  input [`MS_TO_WS_BUS_WD-1 :0] ms_to_ws_bus      ,
  // to regfile
  output [`WS_TO_RF_BUS_WD-1:0] ws_to_rf_bus      ,
  // debug
`ifdef DEBUG
  output [31                :0] debug_wb_pc       ,
  output [3                 :0] debug_wb_rf_we    ,
  output [4                 :0] debug_wb_rf_waddr ,
  output [31                :0] debug_wb_rf_wdata ,
`endif
  // to ds
  output                        ws_valid          ,
  output [4                 :0] ws_rf_waddr
);
// wire ws_valid;
wire ws_ready_go = 1'b1;

assign ws_allowin = ws_ready_go || !ws_valid;

// ws_valid
sirv_gnrl_dfflr #(
  .DW(1)
) u_wsvld_vec_1_dff (
  .clk  (clk),
  .reset(reset),
  .lden (ws_allowin),
  .dnxt (ms_to_ws_valid),
  .qout (ws_valid)
);

// ws bus
wire [`MS_TO_WS_BUS_WD-1:0]  ms_to_ws_bus_r;

sirv_gnrl_dfflr #(
  .DW(`MS_TO_WS_BUS_WD)
) u_es_bus_vec_dff (
  .clk  (clk),
  .reset(reset),
  .lden (ws_allowin && ms_to_ws_valid),
  .dnxt (ms_to_ws_bus),
  .qout (ms_to_ws_bus_r)
);

wire [31:0] ws_pc;
wire        ws_rf_we;
// wire [4:0]  ws_rf_waddr;
wire [31:0] ws_rf_wdata;

assign {ws_pc,
        ws_rf_we,
        ws_rf_waddr,
        ws_rf_wdata} = ms_to_ws_bus_r;

// to regfile
assign ws_to_rf_bus = {ws_rf_we && ws_valid, ws_rf_waddr, ws_rf_wdata};

// debug
`ifdef DEBUG
assign debug_wb_pc        = ws_pc;
assign debug_wb_rf_we     = {4{ws_rf_we && ws_valid}};
assign debug_wb_rf_waddr  = ws_rf_waddr;
assign debug_wb_rf_wdata  = ws_rf_wdata;
`endif
endmodule