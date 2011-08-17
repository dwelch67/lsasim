
//-------------------------------------------------------------------
// Copyright (C) 2010 David Welch
//-------------------------------------------------------------------

//#define WATCH_FETCH
//#define WATCH_MEM
//#define WATCH_REGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fpin;
FILE *fpout;

char section[512];

char filename[512];

unsigned int line;
char newline[1024];


unsigned int maxaddr;
unsigned short mem[0x10000];


#define LABLEN 64
#define LAB_TYPE_GLOBAL 1
#define LAB_TYPE_EXTERN 2

#define MAX_LABS 65536
struct
{
    char name[LABLEN];
    unsigned int addr;
    unsigned int line;
    unsigned int type;
} lab_struct[MAX_LABS];
unsigned int nlabs;
unsigned int lab;

unsigned short reg[128];
unsigned int state;
#define STATE_HALTED 0
#define STATE_FETCH  1
#define STATE_MEMORY 2
#define STATE_RESET  3

#define VBIT 8
#define NBIT 4
#define CBIT 2
#define ZBIT 1


unsigned int inst;
unsigned int rd;
unsigned int wr;
unsigned int add;
unsigned int data;

unsigned int opa;
unsigned int opb;
unsigned int res;


unsigned int fetch_count;
unsigned int read_count;
unsigned int write_count;



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


unsigned short mem_fetch ( unsigned short addr )
{
    unsigned short data;
    fetch_count++;
    data=mem[addr];
#ifdef WATCH_FETCH
    printf("fetch [0x%04X] 0x%04X\n",addr,data);
#endif
    return(data);
}

unsigned short mem_read ( unsigned short addr )
{
    unsigned short data;

    read_count++;
    data=mem[addr];
#ifdef WATCH_MEM
    printf("read  [0x%04X] 0x%04X\n",addr,data);
#endif
    return(data);
}

void mem_write ( unsigned short addr, unsigned short data )
{
    write_count++;
    mem[addr]=data;
#ifdef WATCH_MEM
    printf("write [0x%04X] 0x%04X\n",addr,data);
#endif
    switch(addr)
    {
        case 0xF000:
        {
            printf("uart_out[0x%02X]\n",data);
            break;
        }
        case 0xFF00:
        {
            printf("show: 0x%04X\n",data);
            break;
        }
    }



}

unsigned short read_register ( unsigned short r )
{
    unsigned short data;

    data=reg[r];
#ifdef WATCH_REGS
    printf("read_register(%u) = 0x%04X\n",r,data);
#endif
    return(data);
}

void write_register ( unsigned short r, unsigned short data )
{
#ifdef WATCH_REGS
    printf("write_register(%u,0x%04X)\n",r,data);
#endif
    reg[r]=data;
}


int reset_lsa ( void )
{
    unsigned int ra;

    write_register(0,0x0000);
    write_register(1,0x0000);
    write_register(2,0x0000);
    for(ra=3;ra<128;ra++) write_register(ra,0x0000);
    state=STATE_RESET;

    fetch_count=0;
    read_count=0;
    write_count=0;

    return(0);
}

int lsa_sim ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    unsigned int rega;
    unsigned int regb;

    unsigned int simcount;

    reset_lsa();

    simcount=0;

    while(1)
    {
//if(simcount++>30) break;
        if(state==STATE_HALTED) break;
//printf("[0x%04X] 0x%04X\n",read_register(0),read_register(3]);
        switch(state)
        {
            case STATE_HALTED: break;
            case STATE_FETCH:
                rega=read_register(0);
                inst=mem_fetch(rega);
                rega++;
                write_register(0,rega);
                rd=0;
                wr=0;
                switch(inst&0xF000)
                {
                    case 0x0000: // lpc load pc relative
                        rd=1;
                        add=read_register(0)+(inst&0xFF);
                        break;
                    case 0x1000: // lsp load sp relative
                        rd=1;
                        add=read_register(2)+(inst&0xFF);
                        break;
                    case 0x2000: // spc store pc relative
                        wr=1;
                        add=read_register(0)+(inst&0xFF);
                        data=read_register((inst>>8)&0xF);
                        break;
                    case 0x3000: // ssp store sp relative
                        wr=1;
                        add=read_register(2)+(inst&0xFF);
                        data=read_register((inst>>8)&0xF);
                        break;
                    case 0x4000: // ldw/stw load/store
                        switch(inst&0xF)
                        {
                            case 0x0: // ldw rd,[rs]
                                rd=1;
                                add=read_register((inst>>4)&0xF);
                                break;
                            case 0x1: // ldw rd,[rs++]
                                rd=1;
                                rega=read_register((inst>>4)&0xF);
                                add=rega;
                                rega++;
                                write_register((inst>>4)&0xF,rega);
                                break;
                            case 0x2: // ldw rd,[++rs]
                                rd=1;
                                rega=read_register((inst>>4)&0xF);
                                rega++;
                                add=rega;
                                write_register((inst>>4)&0xF,rega);
                                break;
                            case 0x3: // ldw rd,[rs--]
                                rd=1;
                                rega=read_register((inst>>4)&0xF);
                                add=rega;
                                rega--;
                                write_register((inst>>4)&0xF,rega);
                                break;
                            case 0x4: // ldw rd,[--rs]
                                rd=1;
                                rega=read_register((inst>>4)&0xF);
                                rega--;
                                add=rega;
                                write_register((inst>>4)&0xF,rega);
                                break;

                            case 0x6: // mov rd,rs
                                rega=read_register((inst>>4)&0xF);
                                write_register((inst>>8)&0xF,rega);
                                break;

                            case 0x8: // stw [rd],rs
                                wr=1;
                                add=read_register((inst>>8)&0xF);
                                data=read_register((inst>>4)&0xF);
                                break;
                            case 0x9: // stw [rd++],rs
                                wr=1;
                                rega=read_register((inst>>8)&0xF);
                                add=rega;
                                rega++;
                                write_register((inst>>8)&0xF,rega);
                                data=read_register((inst>>4)&0xF);
                                break;
                            case 0xA: // stw [++rd],rs
                                wr=1;
                                rega=read_register((inst>>8)&0xF);
                                rega++;
                                add=rega;
                                write_register((inst>>8)&0xF,rega);
                                data=read_register((inst>>4)&0xF);
                                break;
                            case 0xB: // stw [rd--],rs
                                wr=1;
                                rega=read_register((inst>>8)&0xF);
                                add=rega;
                                rega--;
                                write_register((inst>>8)&0xF,rega);
                                data=read_register((inst>>4)&0xF);
                                break;
                            case 0xC: // stw [--rd],rs
                                wr=1;
                                rega=read_register((inst>>8)&0xF);
                                rega--;
                                add=rega;
                                write_register((inst>>8)&0xF,rega);
                                data=read_register((inst>>4)&0xF);
                                break;

                            case 0xE: // swap rd,rs
                                rega=read_register((inst>>4)&0xF);
                                regb=read_register((inst>>8)&0xF);
                                write_register((inst>>4)&0xF,regb);
                                write_register((inst>>8)&0xF,rega);
                                break;
                        }
                        break;
                    case 0x5000: // move to/from high register
                        if(inst&0x80)
                        {
                            rega=read_register((inst>>8)&0xF);
                            write_register((inst>>0)&0x7F,rega);
                        }
                        else
                        {
                            rega=read_register((inst>>0)&0x7F);
                            write_register((inst>>8)&0xF,rega);
                        }
                        break;
                    case 0x6000: // alu operation
                        switch(inst&0xF)
                        {
                            case 0x6:
                                opa=0;
                                opb=read_register((inst>>4)&0xF);
                                break;
                            case 0x8:
                            case 0x9:
                                opa=read_register((inst>>4)&0xF);
                                opb=1;
                                break;
                            default:
                                opa=read_register((inst>>8)&0xF);
                                opb=read_register((inst>>4)&0xF);
                                break;
                        }
                        switch(inst&0xF)
                        {
                            case 0x0: res = opa + opb;      break;  //add
                            case 0x1: res = opa - opb;      break;  //sub
                            case 0x2: res = opa & opb;      break;  //and
                            case 0x3: res = opa & (~opb);   break;  //dna
                            case 0x4: res = opa | opb;      break;  //or
                            case 0x5: res = opa ^ opb;      break;  //xor
                            case 0x6: res = opa - opb;      break;  //neg
                            case 0x7: res =     ~ opb;      break;  //not
                            case 0x8: res = opa + opb;      break;  //inc
                            case 0x9: res = opa - opb;      break;  //dec
                            case 0xA: res = opa - opb;      break;  //cmp
                            case 0xB: res = opa & opb;      break;  //tst
                        }
                        rega=read_register(1); //dont use rega until after the switch
                        if((res&0xFFFF)==0) rega|=ZBIT; else rega&=(~ZBIT);
                        if(res&0x8000)      rega|=NBIT; else rega&=(~NBIT);
                        switch(inst&0xF)
                        {
                            case 0x0:
                            case 0x8:
                                //add
                                if(res&0x10000) rega|=CBIT; else rega&=(~CBIT);
                                if
                                (
                                    ((opa&0x8000) == (opb&0x8000))
                                    &&
                                    ((res&0x8000) != (opb&0x8000))
                                )    rega|=VBIT;
                                else rega&=(~VBIT);
                                break;
                            case 0x1:
                            case 0x6:
                            case 0x9:
                            case 0xA:
                                //sub
                                if(res&0x10000) rega&=(~CBIT); else rega|=CBIT;
                                if
                                (
                                    ((opa&0x8000) != (opb&0x8000))
                                    &&
                                    ((res&0x8000) == (opb&0x8000))
                                )    rega|=VBIT;
                                else rega&=(~VBIT);
                                break;
                            default:
                                rega&=(~CBIT);
                                rega&=(~VBIT);
                                break;
                        }
                        write_register(1,rega); //can use rega now
                        switch(inst&0xF)
                        {
                            case 0x0:  //add
                            case 0x1:  //sub
                            case 0x2:  //and
                            case 0x3:  //dna
                            case 0x4:  //or
                            case 0x5:  //xor
                            case 0x6:  //neg
                            case 0x7:  //not
                            case 0x8:  //inc
                            case 0x9:  //dec
                                write_register((inst>>8)&0xF,res);
                                break;
                            case 0xA:  //cmp
                            case 0xB:  //tst
                                //these set flags but do not store
                                break;
                        }
                        break;
                    case 0x7000: // shift
                        opa=read_register((inst>>8)&0xF);
                        switch(inst&0xF)
                        {
                            case 0x0: // register based
                            case 0x1:
                            case 0x2:
                            case 0x3:
                            case 0x4:
                            case 0x5:
                            case 0x6:
                                opb=read_register((inst>>4)&0xF)&0xF;
                                break;
                            case 0x8: // immediate
                            case 0x9:
                            case 0xA:
                            case 0xB:
                            case 0xC:
                            case 0xD:
                            case 0xE:
                                opb=(inst>>4)&0xF;
                                break;
                        }
                        res=opa;
                        if(opb)
                        {
                            switch(inst&0x7)
                            {
                                case 0x0: // lsr
                                    res=opa>>opb;
                                    break;
                                case 0x1: // asr
                                    res=opa>>opb;
                                    if(opa&0x8000) res|=(~0)<<(15-opb);
                                    break;
                                case 0x2: // lsl
                                    res=opa<<opb;
                                    break;
                                case 0x3: // ror
                                    res=opa>>opb;
                                    res|=opa<<(16-opb);
                                    break;
                                case 0x4: // rol
                                    res=opa<<opb;
                                    res|=opa>>(16-opb);
                                    break;
                                case 0x5: // rrc
                                    rega=read_register(1);
                                    res=opa>>opb;
                                    res|=opa<<(17-opb);
                                    if(rega&CBIT)
                                    {
                                        res|=0x8000>>(opb-1);
                                    }
                                    else
                                    {
                                        res&=~(0x8000>>(opb-1));
                                    }
                                    if(opa&(0x0001<<(opb-1))) rega|=CBIT;
                                    else                      rega&=~(CBIT);
                                    write_register(1,rega);
                                    break;
                                case 0x6: // rlc
                                    rega=read_register(1);
                                    res=opa<<opb;
                                    res|=opa>>(17-opb);
                                    if(rega&CBIT)
                                    {
                                        res|=1<<(opb-1);
                                    }
                                    else
                                    {
                                        res&=~(1<<(opb-1));
                                    }
                                    if(opa&(0x8000>>(opb-1))) rega|=CBIT;
                                    else                      rega&=~(CBIT);
                                    write_register(1,rega);
                                    break;
                            }
                        }
                        switch(inst&0xF)
                        {
                            case 0x0: //store result in rd
                            case 0x1:
                            case 0x2:
                            case 0x3:
                            case 0x4:
                            case 0x5:
                            case 0x6:
                            case 0x8:
                            case 0x9:
                            case 0xA:
                            case 0xB:
                            case 0xC:
                            case 0xD:
                            case 0xE:
                                write_register((inst>>8)&0xF,res);
                                break;
                        }
                        break;
                    case 0x8000: // llz load immediate low zero high
                        write_register((inst>>8)&0xF,(inst<<0)&0x00FF);
                        break;
                    case 0x9000: // lhz load immediate high zero high
                        write_register((inst>>8)&0xF,(inst<<8)&0xFF00);
                        break;
                    case 0xA000: // ll load immediate low
                        rega=read_register((inst>>8)&0xF);
                        rega&=0xFF00;
                        rega|=(inst<<0)&0x00FF;
                        write_register((inst>>8)&0xF,rega);
                        break;
                    case 0xB000: // load immediate high
                        rega=read_register((inst>>8)&0xF);
                        rega&=0x00FF;
                        rega|=(inst<<8)&0xFF00;
                        write_register((inst>>8)&0xF,rega);
                        break;
                    case 0xC000: // branch pc based + immediate
                        ra=inst&0xFF;
                        if(inst&0x80) ra|=~0xFF;
                        ra+=read_register(0);
                        switch(inst&0xF00)
                        {
                            case 0x000: write_register(0,ra); break; // b unconditional
                            case 0x100: if(  read_register(1)&ZBIT)  write_register(0,ra); break; // bz
                            case 0x200: if(!(read_register(1)&ZBIT)) write_register(0,ra); break; // bnz
                            case 0x300: if(  read_register(1)&CBIT)  write_register(0,ra); break; // bc
                            case 0x400: if(!(read_register(1)&CBIT)) write_register(0,ra); break; // bnc
                            case 0x500: if(  read_register(1)&NBIT)  write_register(0,ra); break; // bn
                            case 0x600: if(!(read_register(1)&NBIT)) write_register(0,ra); break; // bnn
                            case 0x700: if(  read_register(1)&VBIT)  write_register(0,ra); break; // bv
                            case 0x800: if(!(read_register(1)&VBIT)) write_register(0,ra); break; // bnv
                            case 0x900: // bsg
                                rega=read_register(1);
                                if(rega&NBIT) rb=1; else rb=0;
                                if(rega&VBIT) rc=1; else rc=0;
                                if((rb^rc)==0) write_register(0,ra);
                                break;
                            case 0xA00: //bsl
                                rega=read_register(1);
                                if(rega&NBIT) rb=1; else rb=0;
                                if(rega&VBIT) rc=1; else rc=0;
                                if((rb^rc)==1) write_register(0,ra);
                                break;
                        }
                        break;
                    case 0xD000: // branch register
                        switch(inst&0xF00)
                        {
                            case 0x000: write_register(0,read_register((inst>>0)&0xF)); break; // b unconditional
                            case 0x100: if(  read_register(1)&ZBIT)  write_register(0,read_register((inst>>0)&0xF)); break; // bz
                            case 0x200: if(!(read_register(1)&ZBIT)) write_register(0,read_register((inst>>0)&0xF)); break; // bnz
                            case 0x300: if(  read_register(1)&CBIT)  write_register(0,read_register((inst>>0)&0xF)); break; // bc
                            case 0x400: if(!(read_register(1)&CBIT)) write_register(0,read_register((inst>>0)&0xF)); break; // bnc
                            case 0x500: if(  read_register(1)&NBIT)  write_register(0,read_register((inst>>0)&0xF)); break; // bn
                            case 0x600: if(!(read_register(1)&NBIT)) write_register(0,read_register((inst>>0)&0xF)); break; // bnn
                            case 0x700: if(  read_register(1)&VBIT)  write_register(0,read_register((inst>>0)&0xF)); break; // bv
                            case 0x800: if(!(read_register(1)&VBIT)) write_register(0,read_register((inst>>0)&0xF)); break; // bnv
                            case 0x900: //bsg
                                rega=read_register(1);
                                if(rega&NBIT) rb=1; else rb=0;
                                if(rega&VBIT) rc=1; else rc=0;
                                if((rb^rc)==0) write_register(0,read_register((inst>>0)&0xF));
                                break;
                            case 0xB00: // bsl
                                rega=read_register(1);
                                if(rega&NBIT) rb=1; else rb=0;
                                if(rega&VBIT) rc=1; else rc=0;
                                if((rb^rc)==1) write_register(0,read_register((inst>>0)&0xF));
                                break;
                            case 0xF00: // call
                                rega=read_register((inst>>0)&0xF);
                                write_register((inst>>4)&0xF,read_register(0));
                                write_register(0,rega);
                                break;
                        }
                        break;
                }
                state=STATE_MEMORY;
                if(inst==0xFFFF) state=STATE_HALTED; // halt
                break;
            case STATE_MEMORY:
                if(rd) data=mem_read(add);
                if(wr) mem_write(add,data);

                switch(inst&0xF000)
                {
                    case 0x0000: // lpc load pc relative
                    case 0x1000: // lsp load sp relative
                        write_register((inst>>8)&0xF,data);
                        break;
                    case 0x4000: // ldw/stw load/store word
                        switch((inst>>0)&0xF)
                        {
                            case 0: // ldw
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                                write_register((inst>>8)&0xF,data);
                                break;
                        }
                        break;

                }
                state=STATE_FETCH;
                break;
            case STATE_RESET:
                state=STATE_FETCH;
                break;
        }
    }
    printf("fetch_count %u\n",fetch_count);
    printf("read_count %u\n",read_count);
    printf("write_count %u\n",write_count);
    return(0);
}

int read_csv ( void )
{
    unsigned int ra;
    unsigned int off;

    unsigned int addr;
    unsigned int data;

    memset(mem,0xFF,sizeof(mem));

    nlabs=0;
    maxaddr=0;
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
            if(addr>maxaddr) maxaddr=addr;
            mem[addr]=data;
        }
        if(strcmp(section,"GLOBAL")==0)
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
            //label
            off=get_section(section,sizeof(section)-1,newline,off);
            if(section[0]==0)
            {
                printf("<%u> Error: empty string\n",line);
                return(1);
            }
            strcpy(lab_struct[nlabs].name,section);
            //END
            off=get_section(section,sizeof(section)-1,newline,off);
            if(strcmp(section,"END")!=0)
            {
                printf("<%u> Error: no END\n",line);
                return(1);
            }
            lab_struct[nlabs].addr=addr;
            lab_struct[nlabs].line=line;
            lab_struct[nlabs].type=LAB_TYPE_GLOBAL;
            //printf("GLOBAL,0x%04X,%s,END,,\n",lab_struct[nlabs].addr,lab_struct[nlabs].name);

            nlabs++;
        }
        if(strcmp(section,"EXTERN")==0)
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
            //label
            off=get_section(section,sizeof(section)-1,newline,off);
            if(section[0]==0)
            {
                printf("<%u> Error: empty string\n",line);
                return(1);
            }
            strcpy(lab_struct[nlabs].name,section);
            //END
            off=get_section(section,sizeof(section)-1,newline,off);
            if(strcmp(section,"END")!=0)
            {
                printf("<%u> Error: no END\n",line);
                return(1);
            }
            lab_struct[nlabs].addr=addr;
            lab_struct[nlabs].line=line;
            lab_struct[nlabs].type=LAB_TYPE_EXTERN;
            //printf("EXTERN,0x%04X,%s,END,,\n",lab_struct[nlabs].addr,lab_struct[nlabs].name);

            nlabs++;
        }
    }

    fpout=fopen(filename,"wt");
    if(fpout==NULL)
    {
        printf("Error creating file [%s]\n",filename);
        return(1);
    }


    for(lab=0;lab<nlabs;lab++)
    {
        if(lab_struct[lab].type==LAB_TYPE_EXTERN) break;
    }
    if(lab<nlabs)
    {
        printf("Unresolved external variable names, need to link\n");
    }
    else
    {
        lsa_sim();
    }
    fclose(fpout);
    return(0);
}


int main ( int argc, char *argv[] )
{
    int ret;

    if(argc<2)
    {
        printf("lsa-sim filename.csv\n");
        return(1);
    }

    fpin=fopen(argv[1],"rt");
    if(fpin==NULL)
    {
        printf("Error opening file [%s]\n",argv[1]);
        return(1);
    }
    sprintf(filename,"%s.sim",argv[1]);

    ret=read_csv();
    fclose(fpin);
    if(ret) return(1);
    return(0);
}

