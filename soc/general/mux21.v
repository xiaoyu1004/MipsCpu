module mux21 #(
  parameter DW = `XLEN
) (
  input            sel,
  input  [DW-1:0]  in0,
  input  [DW-1:0]  in1,
  output [DW-1:0]  out
);

assign out = ({DW{sel == 1'b0}} & in0) | 
             ({DW{sel == 1'b1}} & in1);
endmodule