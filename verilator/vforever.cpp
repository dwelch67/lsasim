
#include <stdio.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

#include "Vlsa.h"

FILE *fpin;
char newline[1024];
char section[512];

#define SRAM_MASK 0x7FFF

unsigned short sram[SRAM_MASK+1];

Vlsa *top;


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











int main(int argc, char **argv)
{
    unsigned int tick;
    int ret;

#include "sram.h"

//    Verilated::commandArgs(argc, argv);
    if(argc>1)
    {
        fpin=fopen(argv[1],"rt");
        if(fpin==NULL)
        {
            printf("Error opening file [%s]\n",argv[1]);
            return(1);
        }
        ret=read_csv();
        fclose(fpin);
        if(ret)
        {
            printf("Error reading file [%s]\n",argv[1]);
            return(1);
        }
    }

//    Verilated::traceEverOn(false);

    top = new Vlsa;

    top->reset_in = 1;
    while (!Verilated::gotFinish())
    {
        tick++;
        if (tick > 11)
        {
            top->reset_in = 0;


            if(top->mem_oe==0)
            {
                top->mem_in = 0xFFFF;
            }
            else
            {
                top->mem_in = sram[(top->mem_add&SRAM_MASK)];
            }
            if(top->mem_we==0)
            {


            }
            else
            {
                if(top->mem_add<=SRAM_MASK)
                {
                    sram[(top->mem_add&SRAM_MASK)] = top->mem_out;
                }
                else
                {
                    //printf("[0x%04X] = 0x%04X %u\n",top->mem_add,top->mem_out,tick&1);
                    if((tick&1)==0)
                    {
                        switch(top->mem_add)
                        {
                            case 0xF000:
                            {
                                printf("uart_out 0x%02X\n",top->mem_out);
                                break;
                            }
                            case 0xFF00:
                            {
                                printf("show: 0x%04X\n",top->mem_out);
                                break;
                            }
                        }
                    }
                }
            }
        }
        top->clock_in = (tick & 1);
        top->eval();

//        if(tick>2000) break;
    }
    top->final();
    return 0;
}

