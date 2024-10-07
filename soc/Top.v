`include "cpu.vh"

module Top(
  input                         clk               ,
  input                         reset             ,
  // led
  output [31                :0] led               ,
  // smg
  output [31                :0] smg               
  // debug
`ifdef DEBUG
  ,
  output [31                :0] debug_wb_pc       ,
  output [3                 :0] debug_wb_rf_we    ,
  output [4                 :0] debug_wb_rf_waddr ,
  output [31                :0] debug_wb_rf_wdata 
`endif
);
// inst sram
wire                       inst_sram_en    ;
wire [3:0]                 inst_sram_wen   ;
wire [`XLEN-1:0]           inst_sram_addr  ;
wire [`XLEN-1:0]           inst_sram_wdata ;
wire [`XLEN-1:0]           inst_sram_rdata ;

// master : cpu data
wire                       cpu_data_en     ;
wire [3:0]                 cpu_data_wen    ;
wire [`XLEN-1:0]           cpu_data_addr   ;
wire [`XLEN-1:0]           cpu_data_wdata  ;
wire [`XLEN-1:0]           cpu_data_rdata  ;

// slave : data sram
wire                       data_sram_en    ;
wire [3:0]                 data_sram_wen   ;
wire [`XLEN-1:0]           data_sram_addr  ;
wire [`XLEN-1:0]           data_sram_wdata ;
wire [`XLEN-1:0]           data_sram_rdata ;

// slave : confreg
wire                       conf_en         ;
wire [3:0]                 conf_wen        ;
wire [`XLEN-1:0]           conf_addr       ;
wire [`XLEN-1:0]           conf_wdata      ;
wire [`XLEN-1:0]           conf_rdata      ;

// pipeline
cpu_pipeline u_pipe(
  .clk                (clk)               ,
  .reset              (reset)             ,
  // inst sram
  .inst_sram_en       (inst_sram_en)      ,
  .inst_sram_wen      (inst_sram_wen)     ,
  .inst_sram_addr     (inst_sram_addr)    ,
  .inst_sram_wdata    (inst_sram_wdata)   ,
  .inst_sram_rdata    (inst_sram_rdata)   ,
  // data sram
  .cpu_data_en        (cpu_data_en)       ,
  .cpu_data_wen       (cpu_data_wen)      ,
  .cpu_data_addr      (cpu_data_addr)     ,
  .cpu_data_wdata     (cpu_data_wdata)    ,
  .cpu_data_rdata     (cpu_data_rdata)    
  // debug
`ifdef DEBUG
  ,
  .debug_wb_pc        (debug_wb_pc)       ,
  .debug_wb_rf_we     (debug_wb_rf_we)    ,
  .debug_wb_rf_waddr  (debug_wb_rf_waddr) ,
  .debug_wb_rf_wdata  (debug_wb_rf_wdata)
`endif
);

// inst sram
sram u_inst_sram(
  .clk        (clk)                   ,
  .reset      (reset)                 ,
  .sram_en    (inst_sram_en)          ,
  .sram_wen   (inst_sram_wen)         ,
  .sram_addr  (inst_sram_addr[13:2])  ,
  .sram_wdata (inst_sram_wdata)       ,
  .sram_rdata (inst_sram_rdata)
);

bridge_1x2 u_bridge_1x2(
  .clk             (clk)            ,
  .reset           (reset)          ,
  // master : cpu data
  .cpu_data_en     (cpu_data_en)    ,
  .cpu_data_wen    (cpu_data_wen)   ,
  .cpu_data_addr   (cpu_data_addr)  ,
  .cpu_data_wdata  (cpu_data_wdata) ,
  .cpu_data_rdata  (cpu_data_rdata) ,
  // slave : data sram
  .data_sram_en    (data_sram_en)   ,
  .data_sram_wen   (data_sram_wen)  ,
  .data_sram_addr  (data_sram_addr) ,
  .data_sram_wdata (data_sram_wdata),
  .data_sram_rdata (data_sram_rdata),
  // slave : confreg
  .conf_en         (conf_en)        ,
  .conf_wen        (conf_wen)       ,
  .conf_addr       (conf_addr)      ,
  .conf_wdata      (conf_wdata)     ,
  .conf_rdata      (conf_rdata)
);

// data sram
sram u_data_sram(
  .clk        (clk)                   ,
  .reset      (reset)                 ,
  .sram_en    (data_sram_en)          ,
  .sram_wen   (data_sram_wen)         ,
  .sram_addr  (data_sram_addr[13:2])  ,
  .sram_wdata (data_sram_wdata)       ,
  .sram_rdata (data_sram_rdata)
);

// confreg
confreg u_confreg(
  .clk        (clk)       ,
  .reset      (reset)     ,
  // read and write from cpu
  .conf_en    (conf_en)   ,
  .conf_wen   (conf_wen)  ,
  .conf_addr  (conf_addr) ,
  .conf_wdata (conf_wdata),
  .conf_rdata (conf_rdata),
  .led        (led)       ,
  .smg        (smg)       
);
endmodule