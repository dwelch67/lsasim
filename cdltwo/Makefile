


obj_dir/Vlsatwo : lsa_core.v vmain.cpp 
	verilator --cc lsa_core.v --exe vmain.cpp --trace --trace-underscore --trace-max-array 1024
	make -C obj_dir/ -f Vlsa_core.mk

lsa_core.v : lsatwo.cdl
	cdl --verilog lsa_core.v --include-dir .  lsatwo.cdl

clean :
	rm -rf obj_dir
	rm -f test.vcd
	rm -f *.s.csv
	rm -f *.s.o


