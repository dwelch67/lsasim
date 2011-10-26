
all : lsa_core.v lsatwo_64.v lsatwo_32.v lsatwo_16.v lsatwo_64_simple.v obj_dir/Vlsatwo


lsatwo_64.v : lsatwo_64.cdl program.cdl
	cdl --verilog lsatwo_64.v --include-dir .  lsatwo_64.cdl
	verilator --lint-only lsatwo_64.v

lsatwo_32.v : lsatwo_32.cdl program.cdl
	cdl --verilog lsatwo_32.v --include-dir .  lsatwo_32.cdl
	verilator --lint-only lsatwo_32.v

lsatwo_16.v : lsatwo_16.cdl program.cdl
	cdl --verilog lsatwo_16.v --include-dir .  lsatwo_16.cdl
	verilator --lint-only lsatwo_16.v

lsatwo_64_simple.v : lsatwo_64_simple.cdl program.cdl
	cdl --verilog lsatwo_64_simple.v --include-dir .  lsatwo_64_simple.cdl


obj_dir/Vlsatwo : lsa_core.v vmain.cpp 
	verilator --cc lsa_core.v --exe vmain.cpp --trace --trace-underscore --trace-max-array 1024
	make -C obj_dir/ -f Vlsa_core.mk

lsa_core.v : lsatwo_64.cdl program.cdl
	cdl --verilog lsa_core.v --include-dir .  lsatwo_64.cdl

clean :
	rm -rf obj_dir
	rm -f test.vcd
	rm -f *.s.csv
	rm -f *.s.o
	rm -f lsatwo_64.v
	rm -f lsatwo_64_simple.v



