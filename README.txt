

This is a project with many goals, lots of docs to follow, for now
lsa-isa.txt describes the instruction set for the processor.

lsa-as.c, lsa-diss.c, and lsa-sim.c are an assembler, disassembler, and
instruction set simulator for this processor.  Developed on linux these
should compile just fine on most any platform, windows, mac, *nix, etc.
The Makefile is Linux based and uses llvm (clang).  gcc or llvm or pretty much
any mainstream C compiler should be able to compile these files.

In addition to an instruction set simulator I also have implemented
the processor in HDL (Hardware Design Language) and have provided
files to go along with two different, free/open simulation environments.
The HDL used is called cyclicity-cdl or just cdl, not well known like
verilog or vhdl, it's compiler outputs verilog which you can then use
with your favorite verilog tools or with verilator as I have provided
an example for.  README.cdl.txt describes how to get and build and
get started with the cdl compiler and a simulation environment that
comes with cdl.  README.verilator.txt describes how to get and build
the verilator verilog simulator.  Cdl and Verilator might be limited
to Linux type environments.

Also a C compiler backend has been developed for the lsa instruction
set.  README.vbcc.txt describes how to get and build the vbcc compiler.








