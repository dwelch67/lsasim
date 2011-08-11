
# gcc or llvm will work, should not matter
CC = clang
#CC = gcc

all : lsa-as lsa-diss lsa-sim

lsa-as : lsa-as.c
	$(CC) lsa-as.c -o lsa-as

lsa-diss : lsa-diss.c
	$(CC) lsa-diss.c -o lsa-diss

lsa-sim : lsa-sim.c
	$(CC) lsa-sim.c -o lsa-sim

clean : 
	rm -f lsa-as
	rm -f lsa-diss
	rm -f lsa-sim


