module sram #(
  parameter SRAM_SIZE = 4096
)(
  input                      clk        ,
  input                      reset      ,
  input                      sram_en    ,
  input  [3:0]               sram_wen   ,
  input  [11:0]              sram_addr  ,
  input  [31:0]              sram_wdata ,
  output [31:0]              sram_rdata 
);
reg [31:0] mem [SRAM_SIZE-1:0];

wire [11:0] mem_addr = sram_addr;

// read
reg [31:0] ram_rdata;

always @(posedge clk) begin
  if (sram_en) begin
    ram_rdata <= mem[mem_addr];
  end
end

assign sram_rdata = ram_rdata;

// write
always @(posedge clk) begin
  if (|sram_wen) begin
    mem[mem_addr] <= sram_wdata;
  end
end
endmodule