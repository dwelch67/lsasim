

lsa_core.v : lsatwo_64.cdl program.cdl
	cdl --verilog lsa_core.v --include-dir .  lsatwo_64.cdl
	verilator --lint-only lsa_core.v


program.cdl : program.s
	lsa-as program.s

clean :
	rm -f lsa_core.v
	rm -f program.cdl



