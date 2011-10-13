
all : ledblink.v lsa_top.v lsa_core.v lsa_mem.v 

obj_dir/Vledblink : ledblink.v lsa_top.v lsa_core.v lsa_mem.v vmain.cpp 
	verilator --cc ledblink.v --exe vmain.cpp --trace --trace-underscore 
	make -C obj_dir/ -f Vledblink.mk

ledblink.v : ledblink.cdl
	cdl --verilog ledblink.v ledblink.cdl

lsa_top.v : lsa_top.cdl
	cdl --verilog lsa_top.v --include-dir . lsa_top.cdl

lsa_core.v : lsa_core.cdl
	cdl --verilog lsa_core.v --include-dir . lsa_core.cdl

lsa_mem.v : lsa_mem.cdl
	cdl --verilog lsa_mem.v --include-dir . lsa_mem.cdl

clean :
	rm -rf obj_dir
	rm -f lsa_core.v
	rm -f lsa_mem.v
	rm -f lsa_top.v
	rm -f test.vcd
	rm -f *.s.csv
	rm -f *.s.o

test.vcd : obj_dir/Vledblink
	./obj_dir/Vledblink

