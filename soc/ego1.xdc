create_clock -period 10.000 -name clk -waveform {0.000 5.000} [get_ports clk]

set_property -dict {PACKAGE_PIN P17 IOSTANDARD LVCMOS33} [ get_ports clk ]
set_property -dict {PACKAGE_PIN P15 IOSTANDARD LVCMOS33} [ get_ports reset ]

# led
set_property -dict {PACKAGE_PIN F6 IOSTANDARD LVCMOS33} [get_ports {led[15]}]
set_property -dict {PACKAGE_PIN G4 IOSTANDARD LVCMOS33} [get_ports {led[14]}]
set_property -dict {PACKAGE_PIN G3 IOSTANDARD LVCMOS33} [get_ports {led[13]}]
set_property -dict {PACKAGE_PIN J4 IOSTANDARD LVCMOS33} [get_ports {led[12]}]
set_property -dict {PACKAGE_PIN H4 IOSTANDARD LVCMOS33} [get_ports {led[11]}]
set_property -dict {PACKAGE_PIN J3 IOSTANDARD LVCMOS33} [get_ports {led[10]}]
set_property -dict {PACKAGE_PIN J2 IOSTANDARD LVCMOS33} [get_ports {led[9]}]
set_property -dict {PACKAGE_PIN K2 IOSTANDARD LVCMOS33} [get_ports {led[8]}]

set_property -dict {PACKAGE_PIN K1 IOSTANDARD LVCMOS33} [get_ports {led[7]}]
set_property -dict {PACKAGE_PIN H6 IOSTANDARD LVCMOS33} [get_ports {led[6]}]
set_property -dict {PACKAGE_PIN H5 IOSTANDARD LVCMOS33} [get_ports {led[5]}]
set_property -dict {PACKAGE_PIN J5 IOSTANDARD LVCMOS33} [get_ports {led[4]}]
set_property -dict {PACKAGE_PIN K6 IOSTANDARD LVCMOS33} [get_ports {led[3]}]
set_property -dict {PACKAGE_PIN L1 IOSTANDARD LVCMOS33} [get_ports {led[2]}]
set_property -dict {PACKAGE_PIN M1 IOSTANDARD LVCMOS33} [get_ports {led[1]}]
set_property -dict {PACKAGE_PIN K3 IOSTANDARD LVCMOS33} [get_ports {led[0]}]

# smg
set_property -dict {PACKAGE_PIN G1 IOSTANDARD LVCMOS33} [ get_ports smg[11] ]
set_property -dict {PACKAGE_PIN F1 IOSTANDARD LVCMOS33} [ get_ports smg[10] ]
set_property -dict {PACKAGE_PIN E1 IOSTANDARD LVCMOS33} [ get_ports smg[9] ]
set_property -dict {PACKAGE_PIN G6 IOSTANDARD LVCMOS33} [ get_ports smg[8] ]

set_property -dict {PACKAGE_PIN D4 IOSTANDARD LVCMOS33} [ get_ports smg[7] ]
set_property -dict {PACKAGE_PIN E3 IOSTANDARD LVCMOS33} [ get_ports smg[6] ]
set_property -dict {PACKAGE_PIN D3 IOSTANDARD LVCMOS33} [ get_ports smg[5] ]
set_property -dict {PACKAGE_PIN F4 IOSTANDARD LVCMOS33} [ get_ports smg[4] ]
set_property -dict {PACKAGE_PIN F3 IOSTANDARD LVCMOS33} [ get_ports smg[3] ]
set_property -dict {PACKAGE_PIN E2 IOSTANDARD LVCMOS33} [ get_ports smg[2] ]
set_property -dict {PACKAGE_PIN D2 IOSTANDARD LVCMOS33} [ get_ports smg[1] ]
set_property -dict {PACKAGE_PIN H2 IOSTANDARD LVCMOS33} [ get_ports smg[0] ]