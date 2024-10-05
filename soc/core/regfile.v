module regfile(
  input         clk       ,
  // read port 1
  input  [4 :0]  raddr1   ,
  output [31:0]  rdata1   ,
  // read port 2
  input  [4 :0]  raddr2   ,
  output [31:0]  rdata2   ,
  // write port
  input          rf_we    ,
  input  [4 :0]  rf_waddr ,
  input  [31:0]  rf_wdata
);
reg [31:0] regs[31:0];

// read port 1
assign rdata1 = (raddr1 == 5'b0) ? 32'b0 : regs[raddr1];

// read port 2
assign rdata2 = (raddr2 == 5'b0) ? 32'b0 : regs[raddr2];

// write
always @(posedge clk) begin
  if (rf_we) begin
    regs[rf_waddr] <= rf_wdata;
  end
end
endmodule