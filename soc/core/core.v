`include "cpu.vh"

module cpu_pipeline(
  input                         clk               ,
  input                         reset             ,
  // inst sram
  output                        inst_sram_en      ,
  output [3:0]                  inst_sram_wen     ,
  output [`XLEN-1:0]            inst_sram_addr    ,
  output [`XLEN-1:0]            inst_sram_wdata   ,
  input  [`XLEN-1:0]            inst_sram_rdata   ,
  // cpu data 
  output                        cpu_data_en       ,
  output [3                 :0] cpu_data_wen      ,
  output [31                :0] cpu_data_addr     ,
  output [31                :0] cpu_data_wdata    ,
  input  [31                :0] cpu_data_rdata    ,
  // debug
  output [31                :0] debug_wb_pc       ,
  output [3                 :0] debug_wb_rf_we    ,
  output [31                :0] debug_wb_rf_waddr ,
  output [31                :0] debug_wb_rf_wdata 
);
// if
wire                        ds_allowin;
wire [`BR_BUS_WD-1:0]       br_bus;
wire                        fs_to_ds_valid;
wire [`FS_TO_DS_BUS_WD-1:0] fs_to_ds_bus;

// id
wire                        es_allowin;
wire                        ds_to_es_valid;
wire [`DS_TO_ES_BUS_WD-1:0] ds_to_es_bus;
wire [4                 :0] rs_addr;
wire [4                 :0] rt_addr;
wire [31                :0] rs_data;
wire [31                :0] rt_data;

// ex
wire                        ms_allowin;
wire                        es_to_ms_valid;
wire [`ES_TO_MS_BUS_WD-1:0] es_to_ms_bus;

// mem
wire                        ws_allowin;
wire                        ms_to_ws_valid;
wire [`MS_TO_WS_BUS_WD-1:0] ms_to_ws_bus;
wire [`WS_TO_RF_BUS_WD-1:0] ws_to_rf_bus;

// if
if_stage u_if(
  .clk              (clk)             ,
  .reset            (reset)           ,
  // allowin
  .ds_allowin       (ds_allowin)      ,
  // br bus
  .br_bus           (br_bus)          ,
  // inst ram
  .inst_sram_en     (inst_sram_en)    ,
  .inst_sram_wen    (inst_sram_wen)   ,
  .inst_sram_addr   (inst_sram_addr)  ,
  .inst_sram_wdata  (inst_sram_wdata) ,
  .inst_sram_rdata  (inst_sram_rdata) ,
  // to ds
  .fs_to_ds_valid   (fs_to_ds_valid)  ,
  .fs_to_ds_bus     (fs_to_ds_bus)
);

// id
id_stage u_id(
  .clk            (clk)             ,
  .reset          (reset)           ,
  // allowin
  .es_allowin     (es_allowin)      ,
  .ds_allowin     (ds_allowin)      ,
  // from fs
  .fs_to_ds_valid (fs_to_ds_valid)  ,
  .fs_to_ds_bus   (fs_to_ds_bus)    ,
  // to fs
  .br_bus         (br_bus)          ,
  // to es
  .ds_to_es_valid (ds_to_es_valid)  ,
  .ds_to_es_bus   (ds_to_es_bus)    ,
  // to rf
  .rs_addr        (rs_addr)         ,
  .rt_addr        (rt_addr)         ,
  // from rf
  .rs_data        (rs_data)         ,
  .rt_data        (rt_data)
);

// ex
ex_stage u_ex(
  .clk              (clk)               ,
  .reset            (reset)             ,
  // allouin
  .ms_allowin       (ms_allowin)        ,
  .es_allowin       (es_allowin)        ,
  // from ds
  .ds_to_es_valid   (ds_to_es_valid)    ,
  .ds_to_es_bus     (ds_to_es_bus)      ,
  // to ms
  .es_to_ms_valid   (es_to_ms_valid)    ,
  .es_to_ms_bus     (es_to_ms_bus)      ,
  // to data sram
  .cpu_data_en      (cpu_data_en)       ,
  .cpu_data_wen     (cpu_data_wen)      ,
  .cpu_data_addr    (cpu_data_addr)     ,
  .cpu_data_wdata   (cpu_data_wdata)
);

// mem
mem_stage u_mem(
  .clk              (clk)             ,
  .reset            (reset)           ,
  // allowin
  .ws_allowin       (ws_allowin)      ,
  .ms_allowin       (ms_allowin)      ,
  // from es
  .es_to_ms_valid   (es_to_ms_valid)  ,
  .es_to_ms_bus     (es_to_ms_bus)    ,
  // to ws
  .ms_to_ws_valid   (ms_to_ws_valid)  ,
  .ms_to_ws_bus     (ms_to_ws_bus)    ,
  // from data sram
  .cpu_data_rdata   (cpu_data_rdata)
);

// wb
wb_stage u_wb(
  .clk                (clk)               ,
  .reset              (reset)             ,
  // allowin
  .ws_allowin         (ws_allowin)        ,
  // from ms
  .ms_to_ws_valid     (ms_to_ws_valid)    ,
  .ms_to_ws_bus       (ms_to_ws_bus)      ,
  // to regfile
  .ws_to_rf_bus       (ws_to_rf_bus)      ,
  // debug
  .debug_wb_pc        (debug_wb_pc)       ,
  .debug_wb_rf_we     (debug_wb_rf_we)    ,
  .debug_wb_rf_waddr  (debug_wb_rf_waddr) ,
  .debug_wb_rf_wdata  (debug_wb_rf_wdata)
);

assign {ws_rf_we, ws_rf_addr, ws_rf_wdata} = ws_to_rf_bus;

regfile u_regfile(
  .clk      (clk)         ,
  // read port 1
  .raddr1   (rs_addr)     ,
  .rdata1   (rs_data)     ,
  // read port 2
  .raddr2   (rt_addr)     ,
  .rdata2   (rt_data)     ,
  // write port
  .rf_we    (ws_rf_we)    ,
  .rf_waddr (ws_rf_addr)  ,
  .rf_wdata (ws_rf_wdata)
);
endmodule