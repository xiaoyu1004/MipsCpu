`include "cpu.vh"

`define LED_ADDR       32'hffff0000
`define SEG_ADDR       32'hffff0001

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
  output  [31                :0] led           ,
  output  [31                :0] seg           
);

// read
wire read_led = conf_en && (conf_addr == `LED_ADDR);
wire read_seg = conf_en && (conf_addr == `SEG_ADDR);

wire [31:0] conf_rdata_mux = ({32{read_led}} & led) | ({32{read_seg}} & seg);

wire [31:0] conf_rdata_r;

sirv_gnrl_dfflr #(
  .DW(`XLEN)
) u_conf_read_vec_32_dff (
  .clk  (clk),
  .reset(reset),
  .lden (conf_en),
  .dnxt (conf_rdata_mux),
  .qout (conf_rdata_r)
);

assign conf_rdata = conf_rdata_r;

// write
wire conf_we = |conf_wen;

// led reg
wire [31:0] led_data;
wire write_led = conf_we && (conf_addr == `LED_ADDR);

sirv_gnrl_dfflr #(
  .DW(`XLEN)
) u_led_vec_32_dff (
  .clk  (clk),
  .reset(reset),
  .lden (write_led),
  .dnxt (conf_wdata),
  .qout (led_data)
);

assign led = led_data;

// seg reg
wire [31:0] seg_data;
wire write_seg = conf_we && (conf_addr == `SEG_ADDR);

sirv_gnrl_dfflr #(
  .DW(`XLEN)
) u_seg_vec_32_dff (
  .clk  (clk),
  .reset(reset),
  .lden (write_seg),
  .dnxt (conf_wdata),
  .qout (seg_data)
);

assign seg = seg_data;

endmodule