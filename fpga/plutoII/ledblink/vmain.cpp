
#include <stdio.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

#include "Vledblink.h"

FILE *fpin;
char newline[1024];
char section[512];

#define SRAM_MASK 0x7FFF

unsigned short sram[SRAM_MASK+1];

Vledblink *top;
VerilatedVcdC *trace;



unsigned int get_section ( char d[], unsigned int len, char s[], unsigned int off )
{
    unsigned int dx;
    //files generated with lsa-as do not have spaces or other characters
    //requiring ascii fields to have quotes
    dx=0;
    while(1)
    {
        if(s[off]==0)
        {
            d[dx]=0;
            break;
        }
        if(s[off]==',')
        {
            d[dx]=0;
            off++;
            break;
        }
        d[dx++]=s[off++];
    }
    return(off);
}



int read_csv ( void )
{
    unsigned int ra;
    unsigned int off;

    unsigned int addr;
    unsigned int data;

    unsigned int line;

    memset(sram,0xFF,sizeof(sram));

    line=0;
    while(fgets(newline,sizeof(newline)-1,fpin))
    {
        line++;
        off=0;
        off=get_section(section,sizeof(section)-1,newline,off);
        if(section[0]==0) continue; //warning/error?
        if(strcmp(section,"REM")==0) continue;
        if(strcmp(section,"COMMENT")==0) continue;
        if(strcmp(section,"END")==0) break;
        if(strcmp(section,"MEM")==0)
        {
            //address
            off=get_section(section,sizeof(section)-1,newline,off);
            ra=0;
            if(section[0]==0x30) ra++;
            if(section[1]=='x') ra++;
            if(section[1]=='X') ra++;
            if(ra!=2)
            {
                printf("<%u> Error: invalid format for address, expecting 0x1234\n",line);
                return(1);
            }
            addr=strtoul(section,NULL,16);
            if(addr&(~0xFFFF))
            {
                printf("<%u> Error: invalid address\n",line);
                return(1);
            }
            //data
            off=get_section(section,sizeof(section)-1,newline,off);
            ra=0;
            if(section[0]==0x30) ra++;
            if(section[1]=='x') ra++;
            if(section[1]=='X') ra++;
            if(ra!=2)
            {
                printf("<%u> Error: invalid format for data, expecting 0x1234\n",line);
                return(1);
            }
            data=strtoul(section,NULL,16);
            if(data&(~0xFFFF))
            {
                printf("<%u> Error: invalid data\n",line);
                return(1);
            }
            //END
            off=get_section(section,sizeof(section)-1,newline,off);
            if(strcmp(section,"END")!=0)
            {
                printf("<%u> Error: no END\n",line);
                return(1);
            }
            //printf("MEM,0x%04X,0x%04X,END,,\n",addr,data);
            sram[addr]=data;
        }
    }

    return(0);
}







int main ( void )
{
    unsigned int tick;
//    int ret;

//    Verilated::commandArgs(argc, argv);

    Verilated::traceEverOn(true);

    top = new Vledblink;

    trace = new VerilatedVcdC;

    top->trace(trace, 99);
    trace->open("test.vcd");

    while (!Verilated::gotFinish())
    {
        tick++;

        top->clk = (tick & 1);

        top->eval();
        trace->dump(tick);

        if(tick>100000) break;
    }
    trace->close();
    top->final();
    return 0;
}

