vlib work
vlib riviera

vlib riviera/xil_defaultlib

vmap xil_defaultlib riviera/xil_defaultlib

vlog -work xil_defaultlib  -v2k5 \
"../../../bd/design_2/sim/design_2.v" \


vlog -work xil_defaultlib \
"glbl.v"

