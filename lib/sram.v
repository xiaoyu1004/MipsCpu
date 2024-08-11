module sram #(
  parameter SRAM_SIZE = 4096
)(
  input                      clk        ,
  input                      reset      ,
  input                      sram_en    ,
  input  [3:0]               sram_wen   ,
  input  [31-1:0]            sram_addr  ,
  input  [31-1:0]            sram_wdata ,
  output [31-1:0]            sram_rdata 
);
reg [SRAM_SIZE-1:0] mem[31:0];

// read
assign sram_rdata = sram_en ? mem[sram_addr] : 32'b0;

// write
always (posedge clk) begin
  if (|sram_wen) begin
    mem[sram_addr] <= sram_wdata;
  end
end
endmodule