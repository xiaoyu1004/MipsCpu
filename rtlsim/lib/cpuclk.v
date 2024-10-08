module cpuclk (
    input  clk_in1 ,
    output locked  ,
    output clk_out1
);

assign locked   = clk_in1;
assign clk_out1 = clk_in1;
    
endmodule