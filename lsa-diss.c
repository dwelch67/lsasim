
//-------------------------------------------------------------------
// Copyright (C) 2010 David Welch
//-------------------------------------------------------------------

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


int diss_lsa ( void )
{
    unsigned int ra;
    unsigned int addr;
    unsigned int inst;
    unsigned int pc;
    unsigned int sp;
    unsigned int rd;
    unsigned int rs;
    unsigned int op;

    for(addr=0;addr<=maxaddr;addr++)
    {
        for(lab=0;lab<nlabs;lab++)
        {
            if(lab_struct[lab].addr==addr) break;
        }
        if(lab<nlabs)
        {
            if(lab_struct[lab].type==LAB_TYPE_GLOBAL)
            {
                fprintf(fpout,"[------]:------ %s:\n",lab_struct[lab].name);
            }
        }
        inst=mem[addr];
        fprintf(fpout,"[0x%04X]:0x%04X ",addr,inst);

        if(lab<nlabs)
        {
            if(lab_struct[lab].type==LAB_TYPE_EXTERN)
            {
                fprintf(fpout,".word =%s\n",lab_struct[lab].name);
                continue;
            }
        }
        if((inst&0xF000)==0x0000)
        {
            rs=inst&0xFF;
            pc=(addr+1)+rs;
            rd=(inst>>8)&0xF;
            fprintf(fpout,"    lpc r%u,#%u ;0x%04X [0x%04X]",rd,rs,(unsigned short)pc,(unsigned short)mem[pc]);
            for(lab=0;lab<nlabs;lab++)
            {
                if(lab_struct[lab].addr==pc) break;
            }
            if(lab<nlabs)
            {
                fprintf(fpout," %s",lab_struct[lab].name);
            }
            fprintf(fpout,"\n");
            continue;
        }
        if((inst&0xF000)==0x1000)
        {
            rs=inst&0xFF;
            sp=addr+rs;
            rd=(inst>>8)&0xF;
            fprintf(fpout,"    lsp r%u,#%u ;0x%04X\n",rd,rs,sp);
            continue;
        }
        if((inst&0xF000)==0x2000)
        {
            rs=inst&0xFF;
            pc=(addr+1)+rs;
            rd=(inst>>8)&0xF;
            fprintf(fpout,"    spc #%u,r%u ;0x%04X",rs,rd,pc);
            for(lab=0;lab<nlabs;lab++)
            {
                if(lab_struct[lab].addr==pc) break;
            }
            if(lab<nlabs)
            {
                fprintf(fpout," %s",lab_struct[lab].name);
            }
            fprintf(fpout,"\n");
            continue;
        }
        if((inst&0xF000)==0x3000)
        {
            rs=inst&0xFF;
            sp=addr+rs;
            rd=(inst>>8)&0xF;
            fprintf(fpout,"    ssp #%u,r%u ;0x%04X\n",rs,rd,sp);
            continue;
        }
        if((inst&0xF000)==0x4000)
        {
            op=(inst>>0)&0xF;
            rs=(inst>>4)&0xF;
            rd=(inst>>8)&0xF;
            ra=1;
            switch(op)
            {
                case 0x0: fprintf(fpout,"    ldw r%u,[r%u]\n",rd,rs); break;
                case 0x1: fprintf(fpout,"    ldw r%u,[r%u++]\n",rd,rs); break;
                case 0x2: fprintf(fpout,"    ldw r%u,[++r%u]\n",rd,rs); break;
                case 0x3: fprintf(fpout,"    ldw r%u,[r%u--]\n",rd,rs); break;
                case 0x4: fprintf(fpout,"    ldw r%u,[--r%u]\n",rd,rs); break;

                case 0x6: fprintf(fpout,"    mov r%u,r%u\n",rd,rs); break;

                case 0x8: fprintf(fpout,"    stw [r%u],r%u\n",rd,rs); break;
                case 0x9: fprintf(fpout,"    stw [r%u++],r%u\n",rd,rs); break;
                case 0xA: fprintf(fpout,"    stw [++r%u],r%u\n",rd,rs); break;
                case 0xB: fprintf(fpout,"    stw [r%u--],r%u\n",rd,rs); break;
                case 0xC: fprintf(fpout,"    stw [--r%u],r%u\n",rd,rs); break;

                case 0xE: fprintf(fpout,"    swap r%u,r%u\n",rd,rs); break;
                default: ra=0; break;
            }
            if(ra) continue;
        }
        if((inst&0xF000)==0x5000)
        {
            op=(inst>>7)&0x1;
            rs=(inst>>0)&0x7F;
            rd=(inst>>8)&0xF;
            if(op)
            {
                fprintf(fpout,"    mov r%u,r%u\n",rs,rd);
            }
            else
            {
                fprintf(fpout,"    mov r%u,r%u\n",rd,rs);
            }
            continue;
        }
        if((inst&0xF000)==0x6000)
        {
            op=(inst>>0)&0xF;
            rs=(inst>>4)&0xF;
            rd=(inst>>8)&0xF;
            ra=1;
            switch(op)
            {
                case 0x0: fprintf(fpout,"    add r%u,r%u\n",rd,rs); break;
                case 0x1: fprintf(fpout,"    sub r%u,r%u\n",rd,rs); break;
                case 0x2: fprintf(fpout,"    and r%u,r%u\n",rd,rs); break;
                case 0x3: fprintf(fpout,"    dna r%u,r%u\n",rd,rs); break;
                case 0x4: fprintf(fpout,"    or r%u,r%u\n",rd,rs); break;
                case 0x5: fprintf(fpout,"    xor r%u,r%u\n",rd,rs); break;
                case 0x6: fprintf(fpout,"    neg r%u,r%u\n",rd,rs); break;
                case 0x7: fprintf(fpout,"    not r%u,r%u\n",rd,rs); break;
                case 0x8: fprintf(fpout,"    inc r%u,r%u\n",rd,rs); break;
                case 0x9: fprintf(fpout,"    dec r%u,r%u\n",rd,rs); break;
                case 0xA: fprintf(fpout,"    cmp r%u,r%u\n",rd,rs); break;
                case 0xB: fprintf(fpout,"    tst r%u,r%u\n",rd,rs); break;

                default: ra=0; break;
            }
            if(ra) continue;
        }
        if((inst&0xF000)==0x7000)
        {
            op=(inst>>0)&0xF;
            rs=(inst>>4)&0xF;
            rd=(inst>>8)&0xF;
            ra=1;
            switch(op)
            {
                case 0x0: fprintf(fpout,"    lsr r%u,r%u\n",rd,rs); break;
                case 0x1: fprintf(fpout,"    asr r%u,r%u\n",rd,rs); break;
                case 0x2: fprintf(fpout,"    lsl r%u,r%u\n",rd,rs); break;
                case 0x3: fprintf(fpout,"    ror r%u,r%u\n",rd,rs); break;
                case 0x4: fprintf(fpout,"    rol r%u,r%u\n",rd,rs); break;
                case 0x5: fprintf(fpout,"    rrc r%u,r%u\n",rd,rs); break;
                case 0x6: fprintf(fpout,"    rlc r%u,r%u\n",rd,rs); break;

                case 0x8: fprintf(fpout,"    lsr r%u,#%u\n",rd,rs); break;
                case 0x9: fprintf(fpout,"    asr r%u,#%u\n",rd,rs); break;
                case 0xA: fprintf(fpout,"    lsl r%u,#%u\n",rd,rs); break;
                case 0xB: fprintf(fpout,"    ror r%u,#%u\n",rd,rs); break;
                case 0xC: fprintf(fpout,"    rol r%u,#%u\n",rd,rs); break;
                case 0xD: fprintf(fpout,"    rrc r%u,#%u\n",rd,rs); break;
                case 0xE: fprintf(fpout,"    rlc r%u,#%u\n",rd,rs); break;

                default: ra=0; break;
            }
            if(ra) continue;
        }
        if((inst&0xF000)==0x8000)
        {
            rs=(inst>>0)&0xFF;
            rd=(inst>>8)&0xF;
            fprintf(fpout,"    llz r%u,#0x%02X\n",rd,rs);
            continue;
        }
        if((inst&0xF000)==0x9000)
        {
            rs=(inst>>0)&0xFF;
            rd=(inst>>8)&0xF;
            fprintf(fpout,"    lhz r%u,#0x%02X\n",rd,rs);
            continue;
        }
        if((inst&0xF000)==0xA000)
        {
            rs=(inst>>0)&0xFF;
            rd=(inst>>8)&0xF;
            fprintf(fpout,"    ll r%u,#0x%02X\n",rd,rs);
            continue;
        }
        if((inst&0xF000)==0xB000)
        {
            rs=(inst>>0)&0xFF;
            rd=(inst>>8)&0xF;
            fprintf(fpout,"    lh r%u,#0x%02X\n",rd,rs);
            continue;
        }
        if((inst&0xF000)==0xC000)
        {
            op=(inst>>8)&0xF;
            rs=(inst>>0)&0xFF;
            if(rs&0x80) rs|=(~0xFF);
            pc=(addr+1)+rs;
            ra=1;
            switch(op)
            {
                case 0x0: fprintf(fpout,"    b #%+d ; 0x%04X\n",(int)rs,(unsigned short)pc); break;
                case 0x1: fprintf(fpout,"    bz #%+d ; 0x%04X\n",(int)rs,(unsigned short)pc); break;
                case 0x2: fprintf(fpout,"    bnz #%+d ; 0x%04X\n",(int)rs,(unsigned short)pc); break;
                case 0x3: fprintf(fpout,"    bc #%+d ; 0x%04X\n",(int)rs,(unsigned short)pc); break;
                case 0x4: fprintf(fpout,"    bnc #%+d ; 0x%04X\n",(int)rs,(unsigned short)pc); break;
                case 0x5: fprintf(fpout,"    bn #%+d ; 0x%04X\n",(int)rs,(unsigned short)pc); break;
                case 0x6: fprintf(fpout,"    bnn #%+d ; 0x%04X\n",(int)rs,(unsigned short)pc); break;
                case 0x7: fprintf(fpout,"    bv #%+d ; 0x%04X\n",(int)rs,(unsigned short)pc); break;
                case 0x8: fprintf(fpout,"    bnv #%+d ; 0x%04X\n",(int)rs,(unsigned short)pc); break;
                case 0x9: fprintf(fpout,"    bug #%+d ; 0x%04X\n",(int)rs,(unsigned short)pc); break;
                case 0xA: fprintf(fpout,"    bsg #%+d ; 0x%04X\n",(int)rs,(unsigned short)pc); break;
                case 0xB: fprintf(fpout,"    bsl #%+d ; 0x%04X\n",(int)rs,(unsigned short)pc); break;

                default: ra=0; break;
            }
            if(ra) continue;
        }
        if((inst&0xF000)==0xD000)
        {
            op=(inst>>8)&0xF;
            rd=(inst>>4)&0xF;
            rs=(inst>>0)&0xF;
            ra=1;
            switch(op)
            {
                case 0x0: fprintf(fpout,"    b r%u\n",rs); break;
                case 0x1: fprintf(fpout,"    bz r%u\n",rs); break;
                case 0x2: fprintf(fpout,"    bnz r%u\n",rs); break;
                case 0x3: fprintf(fpout,"    bc r%u\n",rs); break;
                case 0x4: fprintf(fpout,"    bnc r%u\n",rs); break;
                case 0x5: fprintf(fpout,"    bn r%u\n",rs); break;
                case 0x6: fprintf(fpout,"    bnn r%u\n",rs); break;
                case 0x7: fprintf(fpout,"    bv r%u\n",rs); break;
                case 0x8: fprintf(fpout,"    bnv r%u\n",rs); break;
                case 0x9: fprintf(fpout,"    bug r%u\n",rs); break;
                case 0xA: fprintf(fpout,"    bsg r%u\n",rs); break;
                case 0xB: fprintf(fpout,"    bsl r%u\n",rs); break;

                case 0xF: fprintf(fpout,"    call r%u,r%u\n",rd,rs); break;

                default: ra=0; break;
            }
            if(ra) continue;
        }

        if(inst==0xFFFF)
        {
            fprintf(fpout,"    halt\n");
            continue;
        }

        fprintf(fpout,"    unknown\n");

    }
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

    diss_lsa();

    fclose(fpout);

    return(0);
}


int main ( int argc, char *argv[] )
{
    int ret;

    if(argc<2)
    {
        printf("lsa-diss filename.csv\n");
        return(1);
    }

    fpin=fopen(argv[1],"rt");
    if(fpin==NULL)
    {
        printf("Error opening file [%s]\n",argv[1]);
        return(1);
    }
    sprintf(filename,"%s.s",argv[1]);

    ret=read_csv();
    fclose(fpin);
    if(ret) return(1);
    return(0);
}

