`ifndef CPU_H
`define CPU_H

`define XLEN      32

`define INST_LEN  32

`define BR_BUS_WD 33

`define FS_TO_DS_BUS_WD (`XLEN + `INST_LEN)

`define DS_TO_ES_BUS_WD 136

`define ES_TO_MS_BUS_WD 71

`define MS_TO_WS_BUS_WD 70

`define WS_TO_RF_BUS_WD 38

`define DEBUG

`endif