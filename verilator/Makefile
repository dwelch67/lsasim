


obj_dir/Vlsa : lsa.v vmain.cpp sram.h
	verilator --cc lsa.v --exe vmain.cpp --trace --trace-underscore 
	make -C obj_dir/ -f Vlsa.mk

lsa.v : lsa.cdl
	cdl --verilog lsa.v --include-dir .  lsa.cdl

clean :
	rm -rf obj_dir
	rm -f test.vcd
	rm -f *.s.csv
	rm -f *.s.o


