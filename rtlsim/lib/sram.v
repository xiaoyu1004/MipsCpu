module sram #(
  parameter SRAM_SIZE = 4096
)(
  input                      clka   ,
  input                      ena    ,
  input                      wea    ,
  input  [11:0]              addra  ,
  input  [31:0]              dina   ,
  output [31:0]              douta 
);
reg [31:0] mem [SRAM_SIZE-1:0];

wire [11:0] mem_addr = addra;

// read
reg [31:0] ram_rdata;

always @(posedge clka) begin
  if (ena) begin
    ram_rdata <= mem[mem_addr];
  end
end

assign douta = ram_rdata;

// write
always @(posedge clka) begin
  if (|wea) begin
    mem[mem_addr] <= dina;
  end
end
endmodule