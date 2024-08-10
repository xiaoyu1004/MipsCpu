`include "cpu.vh"

module alu(
  input   [11:0]  alu_op    ,
  input   [31:0]  alu_src1  ,
  input   [31:0]  alu_src2  ,
  output  [31:0] alu_result 
);
wire op_add   = alu_op[ 0];
wire op_sub   = alu_op[ 1];
wire op_slt   = alu_op[ 2];
wire op_sltu  = alu_op[ 3];
wire op_sll   = alu_op[ 4];
wire op_srl   = alu_op[ 5];
wire op_sra   = alu_op[ 6];
wire op_lui   = alu_op[ 7];
wire op_and   = alu_op[ 8];
wire op_or    = alu_op[ 9];
wire op_xor   = alu_op[10];
wire op_nor   = alu_op[11];

wire [31:0] add_result;
wire [31:0] slt_result;
wire [31:0] sltu_result;
wire [31:0] sll_result;
wire [31:0] srl_result;
wire [31:0] sra_result;
wire [31:0] lui_result;
wire [31:0] and_result;
wire [31:0] or_result;
wire [31:0] xor_result;
wire [31:0] nor_result;

// 32 bit adder
wire [31:0] A;
wire [31:0] B;
wire        cin;
wire        cout;

assign A    = alu_src1;
assign B    = (op_sub | op_slt | op_sltu) ? ~alu_src2 : alu_src2;
assign cin  = (op_sub | op_slt | op_sltu) ? 1'b1 : 1'b0;

// add | sub
assign {cout, add_result} = A + B + cin;

// slt
assign slt_result  = {31'b0, sub_result[31]};

// sltu
assign sltu_result = {31'b0, ~cout};

// sll | srl | sra
wire [31:0] op_src;
wire [31:0] shift_result;
wire [31:0] shift_mask;

genvar i;
generate
  for (i = 0; i < 32; i = i + 1) begin: gen_for_rev_src
    assign op_src[i] = op_sll ? alu_src1[31-i] : alu_src1[i];
  end
endgenerate

assign shift_result = op_src >> alu_src2[4:0];

assign shift_mask = ~(32'hffffffff >> alu_src2[4:0]);

genvar j;
generate
  for (j = 0; j < 32; j = j + 1) begin : gen_for_rev_res
    assign sll_result[j] = shift_result[31-j];
  end
endgenerate

assign srl_result = shift_result;

assign sra_result = ({32{alu_src1[31]}} & shift_mask) | shift_result;

// lui
assign lui_result = {alu_src1[15:0], 16'b0};

// and
assign and_result = alu_src1 & alu_src2;

// or
assign or_result  = alu_src1 | alu_src2;

// xor
assign xor_result = alu_src1 ^ alu_src2;

// nor
assign nor_result = ~(alu_src1 ^ alu_src2);

assign alu_result = {
  {{32{op_add | op_sub}}  & add_result}  |
  {{32{op_slt}}           & slt_result}  |
  {{32{op_sltu}},         & sltu_result} |
  {{32{op_sll}},          & sll_result}  |
  {{32{op_srl}},          & srl_result}  |
  {{32{op_sra}},          & sra_result}  |
  {{32{op_lui}},          & lui_result}  |
  {{32{op_and}},          & and_result}  |
  {{32{op_or}},           & or_result}   |
  {{32{op_xor}},          & xor_result}  |
  {{32{op_nor}},          & nor_result}
};
endmodule