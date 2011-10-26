
#include <stdio.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

#include "Vlsa_core.h"

Vlsa_core *top;
VerilatedVcdC *trace;


int main ( void )
{
    unsigned int tick;
    unsigned int halt_count;
    int ret;

    halt_count=0;

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

if(top->v__DOT__mem_wr)
{
    if(top->in_clock == 1)
    {
        //printf("[0x%04X] 0x%08X\n",top->v__DOT__mem_add,top->v__DOT__mem_wdata);
        if(top->v__DOT__mem_add==0xFF00)
        {
            printf("show: 0x%04X\n",top->v__DOT__mem_wdata);
        }
    }
}

if(top->v__DOT__astate==0xF)
{
    if(tick>11)
    {
        if(halt_count==0) halt_count=10;
    }
}
if(halt_count)
{
    if(halt_count==1) break;
    halt_count--;
}


        if(tick>772000) break;
    }
    trace->close();
    top->final();
    return 0;
}

