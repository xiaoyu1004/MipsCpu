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
  input  [31                :0] cpu_data_rdata    
  // debug
`ifdef DEBUG
  ,
  output [31                :0] debug_wb_pc       ,
  output [3                 :0] debug_wb_rf_we    ,
  output [4                 :0] debug_wb_rf_waddr ,
  output [31                :0] debug_wb_rf_wdata 
`endif
);
// if
wire                        ds_allowin;
/* verilator lint_off UNOPTFLAT */
wire [`BR_BUS_WD-1:0]       br_bus;
/* verilator lint_on UNOPTFLAT */
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

// es forward to ds
wire                        es_to_ds_rf_we;
wire [4                 :0] es_to_ds_rf_waddr;
wire [31                :0] es_to_ds_rf_wdata;
// load op
wire                        es_to_ds_load_op;

// mem
wire                        ws_allowin;
wire                        ms_to_ws_valid;
wire [`MS_TO_WS_BUS_WD-1:0] ms_to_ws_bus;
wire [`WS_TO_RF_BUS_WD-1:0] ws_to_rf_bus;

// ms forward to ds
wire                        ms_to_ds_rf_we;
wire [4                 :0] ms_to_ds_rf_waddr;
wire [31                :0] ms_to_ds_rf_wdata;

// wb
wire                         ws_rf_we;
wire [4                 :0]  ws_rf_waddr;
wire [31                :0]  ws_rf_wdata;

assign {ws_rf_we, ws_rf_waddr, ws_rf_wdata} = ws_to_rf_bus;

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
  .clk                  (clk)                   ,
  .reset                (reset)                 ,
  // allowin
  .es_allowin           (es_allowin)            ,
  .ds_allowin           (ds_allowin)            ,
  // from fs
  .fs_to_ds_valid       (fs_to_ds_valid)        ,
  .fs_to_ds_bus         (fs_to_ds_bus)          ,
  // to fs
  .br_bus               (br_bus)                ,
  // to es
  .ds_to_es_valid       (ds_to_es_valid)        ,
  .ds_to_es_bus         (ds_to_es_bus)          ,
  // to rf
  .rs_addr              (rs_addr)               ,
  .rt_addr              (rt_addr)               ,
  // from rf
  .rs_data              (rs_data)               ,
  .rt_data              (rt_data)               ,
  // from es
  .es_to_ds_rf_we       (es_to_ds_rf_we)        ,
  .es_to_ds_rf_waddr    (es_to_ds_rf_waddr)     ,
  .es_to_ds_rf_wdata    (es_to_ds_rf_wdata)     ,
  .es_to_ds_load_op     (es_to_ds_load_op)      ,
  // from ms
  .ms_to_ds_rf_we       (ms_to_ds_rf_we)        ,
  .ms_to_ds_rf_waddr    (ms_to_ds_rf_waddr)     ,
  .ms_to_ds_rf_wdata    (ms_to_ds_rf_wdata)     ,
  // from ws
  .ws_to_ds_rf_we       (ws_rf_we)              ,
  .ws_to_ds_rf_waddr    (ws_rf_waddr)           ,
  .ws_to_ds_rf_wdata    (ws_rf_wdata)           
);

// ex
ex_stage u_ex(
  .clk                  (clk)               ,
  .reset                (reset)             ,
  // allouin
  .ms_allowin           (ms_allowin)        ,
  .es_allowin           (es_allowin)        ,
  // from ds
  .ds_to_es_valid       (ds_to_es_valid)    ,
  .ds_to_es_bus         (ds_to_es_bus)      ,
  // to ms
  .es_to_ms_valid       (es_to_ms_valid)    ,
  .es_to_ms_bus         (es_to_ms_bus)      ,
  // to data sram
  .cpu_data_en          (cpu_data_en)       ,
  .cpu_data_wen         (cpu_data_wen)      ,
  .cpu_data_addr        (cpu_data_addr)     ,
  .cpu_data_wdata       (cpu_data_wdata)    ,
  // to ds
  .es_to_ds_rf_we       (es_to_ds_rf_we)    ,
  .es_to_ds_rf_waddr    (es_to_ds_rf_waddr) ,
  .es_to_ds_rf_wdata    (es_to_ds_rf_wdata) ,
  .es_to_ds_load_op     (es_to_ds_load_op)
);

// mem
mem_stage u_mem(
  .clk                  (clk)               ,
  .reset                (reset)             ,
  // allowin
  .ws_allowin           (ws_allowin)        ,
  .ms_allowin           (ms_allowin)        ,
  // from es
  .es_to_ms_valid       (es_to_ms_valid)    ,
  .es_to_ms_bus         (es_to_ms_bus)      ,
  // to ws
  .ms_to_ws_valid       (ms_to_ws_valid)    ,
  .ms_to_ws_bus         (ms_to_ws_bus)      ,
  // from data sram
  .cpu_data_rdata       (cpu_data_rdata)    ,
  // to ds
  .ms_to_ds_rf_we       (ms_to_ds_rf_we)    ,
  .ms_to_ds_rf_waddr    (ms_to_ds_rf_waddr) ,
  .ms_to_ds_rf_wdata    (ms_to_ds_rf_wdata)
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
  .ws_to_rf_bus       (ws_to_rf_bus)      
  // debug
`ifdef DEBUG
  ,
  .debug_wb_pc        (debug_wb_pc)       ,
  .debug_wb_rf_we     (debug_wb_rf_we)    ,
  .debug_wb_rf_waddr  (debug_wb_rf_waddr) ,
  .debug_wb_rf_wdata  (debug_wb_rf_wdata) 
`endif
);

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
  .rf_waddr (ws_rf_waddr) ,
  .rf_wdata (ws_rf_wdata)
);
endmodule