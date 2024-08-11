`include "cpu.vh"

`define LED_ADDR       16'hf000   //32'hbfaf_f000

module confreg(
  input                          clk           ,
  input                          reset         ,
  // read and write from cpu
  input                          conf_en       ,
  input   [3                 :0] conf_wen      ,
  input   [31                :0] conf_addr     ,
  input   [31                :0] conf_wdata    ,
  output  [31                :0] conf_rdata    ,
  // read and write to device on board
  output  [15                :0] led      
);

wire conf_we = conf_en & (|conf_wen);

// led conf reg
wire [31:0] led_data;
wire write_led = conf_we && (conf_addr == `LED_ADDR);

sirv_gnrl_dfflr u_led_vec_32_dff #(
  .DW(`XLEN)
) (
  .clk  (clk),
  .reset(reset),
  .lden (write_led),
  .dnxt (conf_wdata),
  .qout (led_data)
);

assign led = led_data[15:0];

endmodule