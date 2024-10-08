module cpuclk (
    input  clk_in1 ,
    input  reset   ,
    output locked  ,
    output clk_out1
);

assign locked   = clk_in1 & reset;
assign clk_out1 = clk_in1;
    
endmodule