`include "cpu.vh"

module mem_stage(
  input                         clk             ,
  input                         reset           ,
  // allowin
  input                         ws_allowin      ,
  output                        ms_allowin      ,
  // from es
  input                         es_to_ms_valid  ,
  input [`ES_TO_MS_BUS_WD-1 :0] es_to_ms_bus    ,
  // to ws
  output                        ms_to_ws_valid  ,
  output [`MS_TO_WS_BUS_WD-1:0] ms_to_ws_bus    ,
  // from data sram
  input [31                 :0] cpu_data_rdata  ,
  // to ds
  output                        ms_valid        ,
  output [4                 :0] ms_rf_waddr
);
// wire ms_valid;
wire ms_ready_go  = 1'b1;

wire ms_allowin       = (ws_allowin & ms_ready_go) || ~ms_valid;
assign ms_to_ws_valid = ms_valid && ms_ready_go;

// fs_valid
sirv_gnrl_dfflr #(
  .DW(1)
) u_msvld_vec_1_dff (
  .clk  (clk),
  .reset(reset),
  .lden (ms_allowin),
  .dnxt (es_to_ms_valid),
  .qout (ms_valid)
);

// ms bus
wire [`ES_TO_MS_BUS_WD-1:0] es_to_ms_bus_r;

sirv_gnrl_dfflr #(
  .DW(`ES_TO_MS_BUS_WD)
) u_ms_bus_vec_dff (
  .clk  (clk),
  .reset(reset),
  .lden (ms_allowin && es_to_ms_valid),
  .dnxt (es_to_ms_bus),
  .qout (es_to_ms_bus_r)
);

// from es
wire [31:0] ms_pc;
wire        ms_load_op;
wire        ms_rf_we;
// wire [4:0]  ms_rf_waddr;
wire [31:0] ms_alu_result;

assign {ms_pc,
        ms_load_op,
        ms_rf_we,
        ms_rf_waddr,
        ms_alu_result} = es_to_ms_bus_r;

// to ws
wire [31:0] rf_wdata = ms_load_op ? cpu_data_rdata : ms_alu_result;

assign ms_to_ws_bus = {ms_pc,
                      ms_rf_we,
                      ms_rf_waddr,
                      rf_wdata};
endmodule