
#include <stdio.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

#include "Vlsa_core.h"

Vlsa_core *top;
VerilatedVcdC *trace;


int main ( void )
{
    unsigned int tick;
    int ret;

    Verilated::traceEverOn(true);

    top = new Vlsa_core;

    trace = new VerilatedVcdC;

    top->trace(trace, 99);
    trace->open("test.vcd");

    top->in_reset = 0;
    while (!Verilated::gotFinish())
    {
        tick++;
        if (tick > 11)
        {
            top->in_reset = 1;
        }
        top->in_clock = (tick & 1);
        top->eval();
        trace->dump(tick);
        if(tick>772000) break;
    }
    trace->close();
    top->final();
    return 0;
}

