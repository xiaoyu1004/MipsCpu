`include "cpu.vh"

`define CONF_ADDR_BASE 32'hffff_0000
`define CONF_ADDR_MASK 32'hffff_0000

module bridge_1x2(
  input                        clk             ,
  input                        reset           ,
  // master : cpu data
  input                        cpu_data_en     ,
  input  [3:0]                 cpu_data_wen    ,
  input  [`XLEN-1:0]           cpu_data_addr   ,
  input  [`XLEN-1:0]           cpu_data_wdata  ,
  output [`XLEN-1:0]           cpu_data_rdata  ,
  // slave : data sram
  output                       data_sram_en    ,
  output [3:0]                 data_sram_wen   ,
  output [`XLEN-1:0]           data_sram_addr  ,
  output [`XLEN-1:0]           data_sram_wdata ,
  input  [`XLEN-1:0]           data_sram_rdata ,
  // slave : confreg
  output                       conf_en         ,
  output [3:0]                 conf_wen        ,
  output [`XLEN-1:0]           conf_addr       ,
  output [`XLEN-1:0]           conf_wdata      ,
  input  [`XLEN-1:0]           conf_rdata      
);
wire sel_sram;
wire sel_conf;

assign sel_conf = (cpu_data_addr & `CONF_ADDR_MASK) == `CONF_ADDR_BASE;
assign sel_sram = ~sel_conf;

wire sel_sram_r;
wire sel_conf_r;

sirv_gnrl_dfflr #(
  .DW(1)
) u_sel_sram_vec_1_dff (
  .clk  (clk),
  .reset(reset),
  .lden (1'b1),
  .dnxt (sel_sram),
  .qout (sel_sram_r)
);

sirv_gnrl_dfflr #(
  .DW(1)
) u_sel_conf_vec_1_dff (
  .clk  (clk),
  .reset(reset),
  .lden (1'b1),
  .dnxt (sel_conf),
  .qout (sel_conf_r)
);

// to data sram
assign data_sram_en   = cpu_data_en & sel_sram;
assign data_sram_wen  = cpu_data_wen & {4{sel_sram}};
assign data_sram_addr = cpu_data_addr;
assign data_sram_wdata  = cpu_data_wdata;

// to confreg
assign conf_en    = cpu_data_en & sel_conf;
assign conf_wen   = cpu_data_wen & {4{sel_conf}};
assign conf_addr  = cpu_data_addr;
assign conf_wdata = cpu_data_wdata;

// to cpu
assign cpu_data_rdata = {32{sel_sram_r}} & data_sram_rdata |
                        {32{sel_conf_r}} & conf_rdata;
endmodule