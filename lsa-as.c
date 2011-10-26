
//-------------------------------------------------------------------
// Copyright (C) 2010 David Welch
//-------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


FILE *fpin;
FILE *fpout;


unsigned int rd,rs,rx;

char cstring[1024];


unsigned int mem[0x10000];


unsigned int curradd;


unsigned int line;

char newline[1024];

#define LABLEN 64
#define LAB_TYPE_LPC 1
#define LAB_TYPE_SPC 2
#define LAB_TYPE_BR  3
#define LAB_TYPE_EXT 4
#define LAB_TYPE_WORD 5

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




unsigned char hexchar[256]=
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

unsigned char numchar[256]=
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

#define NREGNAMES (128+3)
struct
{
    char name[4];
    unsigned char num;
} reg_names[NREGNAMES]=
{
    {"pc",0},
    {"sr",1},
    {"sp",2},
    {"r0",0},{"r1",1},{"r2",2},{"r3",3},{"r4",4},{"r5",5},{"r6",6},{"r7",7},
    {"r8",8},{"r9",9},{"r10",10},{"r11",11},{"r12",12},{"r13",13},{"r14",14},{"r15",15},
    {"r16",16},{"r17",17},{"r18",18},{"r19",19},{"r20",20},{"r21",21},{"r22",22},{"r23",23},
    {"r24",24},{"r25",25},{"r26",26},{"r27",27},{"r28",28},{"r29",29},{"r30",30},{"r31",31},
    {"r32",32},{"r33",33},{"r34",34},{"r35",35},{"r36",36},{"r37",37},{"r38",38},{"r39",39},
    {"r40",40},{"r41",41},{"r42",42},{"r43",43},{"r44",44},{"r45",45},{"r46",46},{"r47",47},
    {"r48",48},{"r49",49},{"r50",50},{"r51",51},{"r52",52},{"r53",53},{"r54",54},{"r55",55},
    {"r56",56},{"r57",57},{"r58",58},{"r59",59},{"r60",60},{"r61",61},{"r62",62},{"r63",63},
    {"r64",64},{"r65",65},{"r66",66},{"r67",67},{"r68",68},{"r69",69},{"r70",70},{"r71",71},
    {"r72",72},{"r73",73},{"r74",74},{"r75",75},{"r76",76},{"r77",77},{"r78",78},{"r79",79},
    {"r80",80},{"r81",81},{"r82",82},{"r83",83},{"r84",84},{"r85",85},{"r86",86},{"r87",87},
    {"r88",88},{"r89",89},{"r90",90},{"r91",91},{"r92",92},{"r93",93},{"r94",94},{"r95",95},
    {"r96",96},{"r97",97},{"r98",98},{"r99",99},{"r100",100},{"r101",101},{"r102",102},{"r103",103},
    {"r104",104},{"r105",105},{"r106",106},{"r107",107},{"r108",108},{"r109",109},{"r110",110},{"r111",111},
    {"r112",112},{"r113",113},{"r114",114},{"r115",115},{"r116",116},{"r117",117},{"r118",118},{"r119",119},
    {"r120",120},{"r121",121},{"r122",122},{"r123",123},{"r124",124},{"r125",125},{"r126",126},{"r127",127}
};

//-------------------------------------------------------------------
int rest_of_line ( unsigned int ra )
{
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    if(newline[ra])
    {
        printf("<%u> Error: garbage at end of line\n",line);
        return(1);
    }
    return(0);
}
//-------------------------------------------------------------------
int get_reg_number ( char *s, unsigned int *rx )
{
    unsigned int ra;

    for(ra=0;ra<NREGNAMES;ra++)
    {
        if(strcmp(s,reg_names[ra].name)==0)
        {
            *rx=reg_names[ra].num;
            return(0);
        }
    }
    return(1);
}
//-------------------------------------------------------------------
unsigned int parse_reg ( unsigned int ra, unsigned int hiok )
{
    unsigned int rb;

    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    rb=0;
    for(;newline[ra];ra++)
    {
        if(newline[ra]==',') break;
        if(newline[ra]==0x20) break;
        cstring[rb++]=newline[ra];
    }
    cstring[rb]=0;
    if(get_reg_number(cstring,&rx))
    {
        printf("<%u> Error: not a register\n",line);
        return(0);
    }
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    if((!hiok)&&(rx>15))
    {
        printf("<%u> Error: high register not allowed\n",line);
        return(0);
    }
    return(ra);
}
//-------------------------------------------------------------------
unsigned int parse_comma ( unsigned int ra )
{
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    if(newline[ra]!=',')
    {
        printf("<%u> Error: syntax error\n",line);
        return(0);
    }
    ra++;
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    return(ra);
}
//-------------------------------------------------------------------
unsigned int parse_reg_indirect ( unsigned int ra, unsigned int *flavor )
{
    unsigned int rb;

    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    *flavor=0;
    if(newline[ra]=='[')
    {
        ra++;
        rb=0;
        for(;newline[ra];ra++)
        {
            if(newline[ra]==0x20) continue;
            if(newline[ra]==']') break;
            cstring[rb++]=newline[ra];
        }
        cstring[rb]=0;
        if(newline[ra]!=']')
        {
            printf("<%u> Error: syntax error\n",line);
            return(0);
        }
        ra++;
        if(rb<2)
        {
            printf("<%u> Error: syntax error\n",line);
            return(0);
        }
        if(cstring[0]=='+')
        {
            if(cstring[1]!='+')
            {
                printf("<%u> Error: syntax error\n",line);
                return(0);
            }
            if(get_reg_number(&cstring[2],&rx))
            {
                printf("<%u> Error: syntax error\n",line);
                return(0);
            }
            *flavor=2;
        }
        else if(cstring[0]=='-')
        {
            if(cstring[1]!='-')
            {
                printf("<%u> Error: syntax error\n",line);
                return(0);
            }
            if(get_reg_number(&cstring[2],&rx))
            {
                printf("<%u> Error: syntax error\n",line);
                return(0);
            }
            *flavor=4;
        }
        else if(cstring[rb-1]=='-')
        {
            if(cstring[rb-2]!='-')
            {
                printf("<%u> Error: syntax error\n",line);
                return(0);
            }
            cstring[rb-2]=0;
            if(get_reg_number(cstring,&rx))
            {
                printf("<%u> Error: syntax error\n",line);
                return(0);
            }
            *flavor=3;
        }
        else if(cstring[rb-1]=='+')
        {
            if(cstring[rb-2]!='+')
            {
                printf("<%u> Error: syntax error\n",line);
                return(0);
            }
            cstring[rb-2]=0;
            if(get_reg_number(cstring,&rx))
            {
                printf("<%u> Error: syntax error\n",line);
                return(0);
            }
            *flavor=1;
        }
        else
        {
            if(get_reg_number(cstring,&rx))
            {
                printf("<%u> Error: syntax error\n",line);
                return(0);
            }
            *flavor=0;
        }
    }
    else
    {
        printf("<%u> Error: syntax error\n",line);
        return(0);
    }
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    return(ra);
}
//-------------------------------------------------------------------
unsigned int parse_immed ( unsigned int ra )
{
    unsigned int rb;

    if((newline[ra]==0x30)&&((newline[ra+1]=='x')||(newline[ra+1]=='X')))
    {
        ra+=2;
        rb=0;
        for(;newline[ra];ra++)
        {
            if(newline[ra]==0x20) break;
            if(!hexchar[newline[ra]])
            {
                printf("<%u> Error: invalid number\n",line);
                return(0);
            }
            cstring[rb++]=newline[ra];
        }
        cstring[rb]=0;
        if(rb==0)
        {
            printf("<%u> Error: invalid number\n",line);
            return(0);
        }
        rx=strtoul(cstring,NULL,16);
    }
    else
    {
        rb=0;
        for(;newline[ra];ra++)
        {
            if(newline[ra]==0x20) break;
            if(!numchar[newline[ra]])
            {
                printf("<%u> Error: invalid number\n",line);
                return(0);
            }
            cstring[rb++]=newline[ra];
        }
        cstring[rb]=0;
        if(rb==0)
        {
            printf("<%u> Error: invalid number\n",line);
            return(0);
        }
        rx=strtoul(cstring,NULL,10);
    }
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    return(ra);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------












//-------------------------------------------------------------------
//-------------------------------------------------------------------
unsigned int parse_two_regs ( unsigned int ra, unsigned int hiok )
{
    ra=parse_reg(ra,hiok); if(ra==0) return(0);
    rd=rx;
    ra=parse_comma(ra); if(ra==0) return(0);
    ra=parse_reg(ra,hiok); if(ra==0) return(0);
    rs=rx;
    return(ra);
}
//-------------------------------------------------------------------
unsigned int parse_shift_regs ( unsigned int ra, unsigned int *is_immed )
{
    ra=parse_reg(ra,0); if(ra==0) return(0);
    rd=rx;
    ra=parse_comma(ra); if(ra==0) return(0);
    if(newline[ra]=='#')
    {
        *is_immed=1;
        ra++;
        ra=parse_immed(ra);
        if(ra==0) return(0);
        rs=rx;
        if(rs&(~0xF))
        {
            printf("<%u> Error: number too big\n",line);
            return(0);
        }
    }
    else
    {
        *is_immed=0;
        ra=parse_reg(ra,0); if(ra==0) return(0);
        rs=rx;
    }
    return(ra);
}
//-------------------------------------------------------------------
unsigned int parse_hilo_immed ( unsigned int ra )
{
    ra=parse_reg(ra,0); if(ra==0) return(0);
    rd=rx;
    ra=parse_comma(ra); if(ra==0) return(0);
    if(newline[ra]=='#')
    {
        ra++;
        ra=parse_immed(ra); if(ra==0) return(0);
        rs=rx;
        if(rs&(~0xFF))
        {
            printf("<%u> Error: number too big\n",line);
            return(0);
        }
    }
    else
    {
        printf("<%u> Error: syntax error\n",line);
        return(0);
    }
    return(ra);
}
//-------------------------------------------------------------------
unsigned int parse_ldw ( unsigned int ra, unsigned int *flavor )
{
    unsigned int rb;

    ra=parse_reg(ra,0); if(ra==0) return(0);
    rd=rx;
    ra=parse_comma(ra); if(ra==0) return(0);
    ra=parse_reg_indirect(ra,flavor); if(ra==0) return(0);
    rs=rx;
    return(ra);
}
//-------------------------------------------------------------------
unsigned int parse_stw ( unsigned int ra, unsigned int *flavor )
{
    unsigned int rb;

    ra=parse_reg_indirect(ra,flavor); if(ra==0) return(0);
    rd=rx;
    ra=parse_comma(ra); if(ra==0) return(0);
    ra=parse_reg(ra,0); if(ra==0) return(0);
    rs=rx;
    return(ra);
}
//-------------------------------------------------------------------
unsigned int parse_lpc ( unsigned int ra, unsigned int lsp )
{
    unsigned int rb;

    ra=parse_reg(ra,0); if(ra==0) return(0);
    rd=rx;
    ra=parse_comma(ra); if(ra==0) return(0);

    if(newline[ra]=='#')
    {
        ra++;
        ra=parse_immed(ra); if(ra==0) return(0);
        rs=rx;
        if(rs&(~0xFF))
        {
            printf("<%u> Error: number too big\n",line);
            return(0);
        }
    }
    else
    {
        if(lsp)
        {
            printf("<%u> Error: syntax error\n",line);
            return(0);
        }
        else
        {
            rb=0;
            for(;newline[ra];ra++)
            {
                if(newline[ra]==0x20) break;
                cstring[rb++]=newline[ra];
            }
            cstring[rb]=0;
            if((rb+1)>=LABLEN)
            {
                printf("<%u> Error: Label too long\n",line);
                return(0);
            }
            strcpy(lab_struct[nlabs].name,cstring);
            lab_struct[nlabs].addr=curradd;
            lab_struct[nlabs].line=line;
            lab_struct[nlabs].type=LAB_TYPE_LPC;
            nlabs++;
            rs=0;
        }
    }
    return(ra);
}
//-------------------------------------------------------------------
unsigned int parse_spc ( unsigned int ra, unsigned int ssp )
{
    unsigned int rb;

    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    if(newline[ra]=='#')
    {
        ra++;
        ra=parse_immed(ra); if(ra==0) return(0);
        rd=rx;
        if(rd&(~0xFF))
        {
            printf("<%u> Error: number too big\n",line);
            return(0);
        }
    }
    else
    {
        if(ssp)
        {
            printf("<%u> Error: syntax error\n",line);
            return(0);
        }
        else
        {
            rb=0;
            for(;newline[ra];ra++)
            {
                if(newline[ra]==',') break;
                if(newline[ra]==0x20) break;
                cstring[rb++]=newline[ra];
            }
            cstring[rb]=0;
            if((rb+1)>=LABLEN)
            {
                printf("<%u> Error: Label too long\n",line);
                return(0);
            }
            strcpy(lab_struct[nlabs].name,cstring);
            lab_struct[nlabs].addr=curradd;
            lab_struct[nlabs].line=line;
            lab_struct[nlabs].type=LAB_TYPE_SPC;
            nlabs++;
            rd=0;
        }
    }
    ra=parse_comma(ra); if(ra==0) return(0);
    ra=parse_reg(ra,0); if(ra==0) return(0);
    rs=rx;
    return(ra);
}

//-------------------------------------------------------------------
unsigned int parse_branch ( unsigned int ra, unsigned int *type )
{
    unsigned int rb;

    *type=0;

    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    if(newline[ra]=='#')
    {
        ra++;
        rb=0;
        if(newline[ra]=='+')
        {
            ra++;
            rb=0;
        }
        else if(newline[ra]=='-')
        {
            ra++;
            rb=1;
        }
        ra=parse_immed(ra); if(ra==0) return(0);
        if(((rb==0)&&(rx>127))||((rb==1)&&(rx>128)))
        {
            printf("<%u> Error: Number too big\n",line);
            return(0);
        }
        if(rb) rx=-rx;
        rd=rx&0xFF;
        *type=1;
    }
    else
    {
        rb=0;
        for(;newline[ra];ra++)
        {
            if(newline[ra]==',') break;
            if(newline[ra]==0x20) break;
            cstring[rb++]=newline[ra];
        }
        cstring[rb]=0;
        if(get_reg_number(cstring,&rs))
        {
            *type=2;
            strcpy(lab_struct[nlabs].name,cstring);
            lab_struct[nlabs].addr=curradd;
            lab_struct[nlabs].line=line;
            lab_struct[nlabs].type=LAB_TYPE_BR;
            nlabs++;
        }
        else
        {
            *type=3;
        }
    }
    return(ra);
}






int assemble ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;


    curradd=0;
    nlabs=0;
    nlabs=0;
    nlabs=0;


    line=0;
    while(fgets(newline,sizeof(newline)-1,fpin))
    {
        line++;
        //tabs to spaces and other things
        for(ra=0;newline[ra];ra++)
        {
            if(newline[ra]<0x20)  newline[ra]=0x20;
            if(newline[ra]>=0x7F) newline[ra]=0;
        }

        //various ways to comment lines
        for(ra=0;newline[ra];ra++)
        {
            if(newline[ra]==';') newline[ra]=0;
            if(newline[ra]=='@') newline[ra]=0;
            if((newline[ra]=='/')&&(newline[ra+1]=='/')) newline[ra]=0;
            if(newline[ra]==0) break;
        }

        //skip spaces
        for(ra=0;newline[ra];ra++) if(newline[ra]!=0x20) break;
        if(newline[ra]==0) continue;

        //look for a label?
        for(rb=ra;newline[rb];rb++)
        {
            if(newline[rb]==0x20) break; //no spaces in labels
            if(newline[rb]==':') break;
        }
        if(newline[rb]==':')
        {
            //got a label
            rc=rb-ra;
            if(rc==0)
            {
                printf("<%u> Error: Invalid label\n",line);
                return(1);
            }
            rc--;
            if(rc>=LABLEN)
            {
                printf("<%u> Error: Label too long\n",line);
                return(1);
            }
            for(rb=0;rb<=rc;rb++)
            {
                lab_struct[nlabs].name[rb]=newline[ra++];
            }
            lab_struct[nlabs].name[rb]=0;
            lab_struct[nlabs].addr=curradd;
            lab_struct[nlabs].line=line;
            ra++;
            for(lab=0;lab<nlabs;lab++)
            {
                if(lab_struct[lab].type) continue;
                if(strcmp(lab_struct[lab].name,lab_struct[nlabs].name)==0) break;
            }
            if(lab<nlabs)
            {
                printf("<%u> Error: label [%s] already defined on line %u\n",line,lab_struct[lab].name,lab_struct[lab].line);
                return(1);
            }
            nlabs++;
            //skip spaces
            for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
            if(newline[ra]==0) continue;
        }
// .word -----------------------------------------------------------
        if(strncmp(&newline[ra],".word ",6)==0)
        {
            ra+=6;
            for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
            if(newline[ra]==0)
            {
                printf("<%u> Error: no data\n",line);
                return(1);
            }
            if(newline[ra]=='=')
            {
                ra++;
                rb=0;
                for(;newline[ra];ra++)
                {
                    if(newline[ra]==0x20) break;
                    cstring[rb++]=newline[ra];
                }
                cstring[rb]=0;
                if(rb==0)
                {
                    printf("<%u> Error: syntax error\n",line);
                    return(0);
                }
                strcpy(lab_struct[nlabs].name,cstring);
                lab_struct[nlabs].addr=curradd;
                lab_struct[nlabs].line=line;
                lab_struct[nlabs].type=LAB_TYPE_EXT;
                nlabs++;
                mem[curradd]|=0x80000000;
                mem[curradd]|=0x02000000;
                curradd++;
            }
            else
            {
                ra=parse_immed(ra);
                if(ra==0) return(1);
                if(rx&(~0xFFFF))
                {
                    printf("<%u> Error: number too big\n",line);
                    return(1);
                }
printf("0x%04X\n",rx);
                mem[curradd]=rx;
                mem[curradd]|=0x80000000;
                mem[curradd]|=0x01000000;
                curradd++;
            }
            if(rest_of_line(ra)) return(1);
            continue;
        }
// push -----------------------------------------------------------
        if(strncmp(&newline[ra],"push ",5)==0)
        {
            ra+=5;
            //push rs
            ra=parse_reg(ra,0); if(ra==0) return(0);
            rd=2; //stack pointer
            rs=rx;
            mem[curradd]=0x400C|(rd<<8)|(rs<<4);
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// pop -----------------------------------------------------------
        if(strncmp(&newline[ra],"pop ",4)==0)
        {
            ra+=4;
            //pop rs
            ra=parse_reg(ra,0); if(ra==0) return(0);
            rs=2; //stack pointer
            rd=rx;
            mem[curradd]=0x4001|(rd<<8)|(rs<<4);
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }

// mov -----------------------------------------------------------
        if(strncmp(&newline[ra],"mov ",4)==0)
        {
            ra+=4;
            //mov rd,rs
            ra=parse_two_regs(ra,1); if(ra==0) return(1);
            if((rd>15)&&(rs>15))
            {
                printf("<%u> Error: one low register and one high register or both low registers\n",line);
                return(1);
            }
            else if(rs>15)
            {
                mem[curradd]=0x5000|(rd<<8)|(rs<<0);
                mem[curradd]|=0x80000000;
            }
            else if(rd>15)
            {
                mem[curradd]=0x5080|(rd<<0)|(rs<<8);
                mem[curradd]|=0x80000000;
            }
            else
            {
                mem[curradd]=0x4006|(rd<<8)|(rs<<4);
                mem[curradd]|=0x80000000;
            }
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// add -----------------------------------------------------------
        if(strncmp(&newline[ra],"add ",4)==0)
        {
            ra+=4;
            //add rd,rs
            ra=parse_two_regs(ra,0); if(ra==0) return(1);
            mem[curradd]=0x6000|(rd<<8)|(rs<<4);
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// sub -----------------------------------------------------------
        if(strncmp(&newline[ra],"sub ",4)==0)
        {
            ra+=4;
            //sub rd,rs
            ra=parse_two_regs(ra,0); if(ra==0) return(1);
            mem[curradd]=0x6001|(rd<<8)|(rs<<4);
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// and -----------------------------------------------------------
        if(strncmp(&newline[ra],"and ",4)==0)
        {
            ra+=4;
            //and rd,rs
            ra=parse_two_regs(ra,0); if(ra==0) return(1);
            mem[curradd]=0x6002|(rd<<8)|(rs<<4);
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// dna -----------------------------------------------------------
        if(strncmp(&newline[ra],"dna ",4)==0)
        {
            ra+=4;
            //dna rd,rs
            ra=parse_two_regs(ra,0); if(ra==0) return(1);
            mem[curradd]=0x6003|(rd<<8)|(rs<<4);
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// or -----------------------------------------------------------
        if(strncmp(&newline[ra],"or ",3)==0)
        {
            ra+=3;
            //or rd,rs
            ra=parse_two_regs(ra,0); if(ra==0) return(1);
            mem[curradd]=0x6004|(rd<<8)|(rs<<4);
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// xor -----------------------------------------------------------
        if(strncmp(&newline[ra],"xor ",4)==0)
        {
            ra+=4;
            //xor rd,rs
            ra=parse_two_regs(ra,0); if(ra==0) return(1);
            mem[curradd]=0x6005|(rd<<8)|(rs<<4);
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// neg -----------------------------------------------------------
        if(strncmp(&newline[ra],"neg ",4)==0)
        {
            ra+=4;
            //neg rd,rs
            ra=parse_two_regs(ra,0); if(ra==0) return(1);
            mem[curradd]=0x6006|(rd<<8)|(rs<<4);
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// not -----------------------------------------------------------
        if(strncmp(&newline[ra],"not ",4)==0)
        {
            ra+=4;
            //not rd,rs
            ra=parse_two_regs(ra,0); if(ra==0) return(1);
            mem[curradd]=0x6007|(rd<<8)|(rs<<4);
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// inc -----------------------------------------------------------
        if(strncmp(&newline[ra],"inc ",4)==0)
        {
            ra+=4;
            //inc rd,rs
            ra=parse_two_regs(ra,0); if(ra==0) return(1);
            mem[curradd]=0x6008|(rd<<8)|(rs<<4);
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// dec -----------------------------------------------------------
        if(strncmp(&newline[ra],"dec ",4)==0)
        {
            ra+=4;
            //dec rd,rs
            ra=parse_two_regs(ra,0); if(ra==0) return(1);
            mem[curradd]=0x6009|(rd<<8)|(rs<<4);
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// cmp -----------------------------------------------------------
        if(strncmp(&newline[ra],"cmp ",4)==0)
        {
            ra+=4;
            //cmp rd,rs
            ra=parse_two_regs(ra,0); if(ra==0) return(1);
            mem[curradd]=0x600A|(rd<<8)|(rs<<4);
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// tst -----------------------------------------------------------
        if(strncmp(&newline[ra],"tst ",4)==0)
        {
            ra+=4;
            //tst rd,rs
            ra=parse_two_regs(ra,0); if(ra==0) return(1);
            mem[curradd]=0x600B|(rd<<8)|(rs<<4);
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// call -----------------------------------------------------------
        if(strncmp(&newline[ra],"call ",5)==0)
        {
            ra+=5;
            //call rd,rs
            ra=parse_two_regs(ra,0); if(ra==0) return(1);
            mem[curradd]=0xDF00|(rd<<4)|(rs<<0);
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// swap -----------------------------------------------------------
        if(strncmp(&newline[ra],"swap ",5)==0)
        {
            ra+=5;
            //op rd,rs
            ra=parse_two_regs(ra,0); if(ra==0) return(1);
            mem[curradd]=0x400E|(rd<<8)|(rs<<4);
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// lsr -----------------------------------------------------------
        if(strncmp(&newline[ra],"lsr ",4)==0)
        {
            ra+=4;
            //op rd,rs
            //op rd,#imm4
            ra=parse_shift_regs(ra,&rc); if(ra==0) return(1);
            mem[curradd]=0x7000|(rd<<8)|(rs<<4);
            if(rc) mem[curradd]|=0x0008;
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// asr -----------------------------------------------------------
        if(strncmp(&newline[ra],"asr ",4)==0)
        {
            ra+=4;
            //op rd,rs
            //op rd,#imm4
            ra=parse_shift_regs(ra,&rc); if(ra==0) return(1);
            mem[curradd]=0x7001|(rd<<8)|(rs<<4);
            if(rc) mem[curradd]|=0x0008;
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }

// lsl -----------------------------------------------------------
        if(strncmp(&newline[ra],"lsl ",4)==0)
        {
            ra+=4;
            //op rd,rs
            //op rd,#imm4
            ra=parse_shift_regs(ra,&rc); if(ra==0) return(1);
            mem[curradd]=0x7002|(rd<<8)|(rs<<4);
            if(rc) mem[curradd]|=0x0008;
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// ror -----------------------------------------------------------
        if(strncmp(&newline[ra],"ror ",4)==0)
        {
            ra+=4;
            //op rd,rs
            //op rd,#imm4
            ra=parse_shift_regs(ra,&rc); if(ra==0) return(1);
            mem[curradd]=0x7003|(rd<<8)|(rs<<4);
            if(rc) mem[curradd]|=0x0008;
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// rol -----------------------------------------------------------
        if(strncmp(&newline[ra],"rol ",4)==0)
        {
            ra+=4;
            //op rd,rs
            //op rd,#imm4
            ra=parse_shift_regs(ra,&rc); if(ra==0) return(1);
            mem[curradd]=0x7004|(rd<<8)|(rs<<4);
            if(rc) mem[curradd]|=0x0008;
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// rrc -----------------------------------------------------------
        if(strncmp(&newline[ra],"rrc ",4)==0)
        {
            ra+=4;
            //op rd,rs
            //op rd,#imm4
            ra=parse_shift_regs(ra,&rc); if(ra==0) return(1);
            mem[curradd]=0x7005|(rd<<8)|(rs<<4);
            if(rc) mem[curradd]|=0x0008;
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// rlc -----------------------------------------------------------
        if(strncmp(&newline[ra],"rlc ",4)==0)
        {
            ra+=4;
            //op rd,rs
            //op rd,#imm4
            ra=parse_shift_regs(ra,&rc); if(ra==0) return(1);
            mem[curradd]=0x7006|(rd<<8)|(rs<<4);
            if(rc) mem[curradd]|=0x0008;
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// llz ----------------------------------------------------------
        if(strncmp(&newline[ra],"llz ",4)==0)
        {
            ra+=4;
            //op rd,#imm8
            ra=parse_hilo_immed(ra); if(ra==0) return(1);
            mem[curradd]=0x8000|(rd<<8)|rs;
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// lhz ----------------------------------------------------------
        if(strncmp(&newline[ra],"lhz ",4)==0)
        {
            ra+=4;
            //op rd,#imm8
            ra=parse_hilo_immed(ra); if(ra==0) return(1);
            mem[curradd]=0x9000|(rd<<8)|rs;
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// ll -----------------------------------------------------------
        if(strncmp(&newline[ra],"ll ",3)==0)
        {
            ra+=3;
            //op rd,#imm8
            ra=parse_hilo_immed(ra); if(ra==0) return(1);
            mem[curradd]=0xA000|(rd<<8)|rs;
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// lh -----------------------------------------------------------
        if(strncmp(&newline[ra],"lh ",3)==0)
        {
            ra+=3;
            //op rd,#imm8
            ra=parse_hilo_immed(ra); if(ra==0) return(1);
            mem[curradd]=0xB000|(rd<<8)|rs;
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// ldw -----------------------------------------------------------
        if(strncmp(&newline[ra],"ldw ",4)==0)
        {
            ra+=4;
            //ldw rd,[rs]
            //ldw rd,[rs++]  (rs=rs+2 after, post increment)
            //ldw rd,[++rs]  (rs=rs+2 before, pre increment)
            //ldw rd,[rs--]  (rs=rs-2 after, post decrement)
            //ldw rd,[--rs]  (rs=rs-2 before, pre decrement)
            ra=parse_ldw(ra,&rc); if(ra==0) return(1);
            mem[curradd]=0x4000|(rd<<8)|(rs<<4)|rc|0;
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }

// stw -----------------------------------------------------------
        if(strncmp(&newline[ra],"stw ",4)==0)
        {
            ra+=4;
            ra=parse_stw(ra,&rc); if(ra==0) return(1);
            mem[curradd]=0x4000|(rd<<8)|(rs<<4)|rc|8;
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }

// lpc -----------------------------------------------------------
        if(strncmp(&newline[ra],"lpc ",4)==0)
        {
            ra+=4;
            ra=parse_lpc(ra,0); if(ra==0) return(1);
            mem[curradd]=0x0000|(rd<<8)|rs;
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }


// lsp -----------------------------------------------------------
        if(strncmp(&newline[ra],"lsp ",4)==0)
        {
            ra+=4;
            ra=parse_lpc(ra,1); if(ra==0) return(1);
            mem[curradd]=0x1000|(rd<<8)|rs;
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }


// spc -----------------------------------------------------------
        if(strncmp(&newline[ra],"spc ",4)==0)
        {
            ra+=4;
            ra=parse_spc(ra,0); if(ra==0) return(1);
            mem[curradd]=0x2000|(rs<<8)|rd;
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }


// ssp -----------------------------------------------------------
        if(strncmp(&newline[ra],"ssp ",4)==0)
        {
            ra+=4;
            ra=parse_spc(ra,1); if(ra==0) return(1);
            mem[curradd]=0x3000|(rs<<8)|rd;
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }


// b -----------------------------------------------------------
        if(strncmp(&newline[ra],"b ",2)==0)
        {
            ra+=2;
            mem[curradd]=0x0000;
            ra=parse_branch(ra,&rc); if(ra==0) return(1);
            switch(rc)
            {
                case 1: //pc relative
                    mem[curradd]|=0xC000|rd;
                    break;
                case 2: //label, pc relative
                    mem[curradd]|=0xC000;
                    mem[curradd]|=0x03000000;
                    break;
                case 3: //register
                    mem[curradd]|=0xD000|(rs<<0);
                    break;
            }
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }


// bz -----------------------------------------------------------
        if(strncmp(&newline[ra],"bz ",3)==0)
        {
            ra+=3;
            mem[curradd]=0x0100;
            ra=parse_branch(ra,&rc); if(ra==0) return(1);
            switch(rc)
            {
                case 1: //pc relative
                    mem[curradd]|=0xC000|rd;
                    break;
                case 2: //label, pc relative
                    mem[curradd]|=0xC000;
                    mem[curradd]|=0x03000000;
                    break;
                case 3: //register
                    mem[curradd]|=0xD000|(rs<<0);
                    break;
            }
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }


// bnz -----------------------------------------------------------
        if(strncmp(&newline[ra],"bnz ",4)==0)
        {
            ra+=4;
            mem[curradd]=0x0200;
            ra=parse_branch(ra,&rc); if(ra==0) return(1);
            switch(rc)
            {
                case 1: //pc relative
                    mem[curradd]|=0xC000|rd;
                    break;
                case 2: //label, pc relative
                    mem[curradd]|=0xC000;
                    mem[curradd]|=0x03000000;
                    break;
                case 3: //register
                    mem[curradd]|=0xD000|(rs<<0);
                    break;
            }
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }


// bc -----------------------------------------------------------
        if(strncmp(&newline[ra],"bc ",3)==0)
        {
            ra+=3;
            mem[curradd]=0x0300;
            ra=parse_branch(ra,&rc); if(ra==0) return(1);
            switch(rc)
            {
                case 1: //pc relative
                    mem[curradd]|=0xC000|rd;
                    break;
                case 2: //label, pc relative
                    mem[curradd]|=0xC000;
                    mem[curradd]|=0x03000000;
                    break;
                case 3: //register
                    mem[curradd]|=0xD000|(rs<<0);
                    break;
            }
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }


// bnc -----------------------------------------------------------
        if(strncmp(&newline[ra],"bnc ",4)==0)
        {
            ra+=4;
            mem[curradd]=0x0400;
            ra=parse_branch(ra,&rc); if(ra==0) return(1);
            switch(rc)
            {
                case 1: //pc relative
                    mem[curradd]|=0xC000|rd;
                    break;
                case 2: //label, pc relative
                    mem[curradd]|=0xC000;
                    mem[curradd]|=0x03000000;
                    break;
                case 3: //register
                    mem[curradd]|=0xD000|(rs<<0);
                    break;
            }
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }


// bn -----------------------------------------------------------
        if(strncmp(&newline[ra],"bn ",3)==0)
        {
            ra+=3;
            mem[curradd]=0x0500;
            ra=parse_branch(ra,&rc); if(ra==0) return(1);
            switch(rc)
            {
                case 1: //pc relative
                    mem[curradd]|=0xC000|rd;
                    break;
                case 2: //label, pc relative
                    mem[curradd]|=0xC000;
                    mem[curradd]|=0x03000000;
                    break;
                case 3: //register
                    mem[curradd]|=0xD000|(rs<<0);
                    break;
            }
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }


// bnn -----------------------------------------------------------
        if(strncmp(&newline[ra],"bnn ",4)==0)
        {
            ra+=4;
            mem[curradd]=0x0600;
            ra=parse_branch(ra,&rc); if(ra==0) return(1);
            switch(rc)
            {
                case 1: //pc relative
                    mem[curradd]|=0xC000|rd;
                    break;
                case 2: //label, pc relative
                    mem[curradd]|=0xC000;
                    mem[curradd]|=0x03000000;
                    break;
                case 3: //register
                    mem[curradd]|=0xD000|(rs<<0);
                    break;
            }
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }


// bv -----------------------------------------------------------
        if(strncmp(&newline[ra],"bv ",3)==0)
        {
            ra+=3;
            mem[curradd]=0x0700;
            ra=parse_branch(ra,&rc); if(ra==0) return(1);
            switch(rc)
            {
                case 1: //pc relative
                    mem[curradd]|=0xC000|rd;
                    break;
                case 2: //label, pc relative
                    mem[curradd]|=0xC000;
                    mem[curradd]|=0x03000000;
                    break;
                case 3: //register
                    mem[curradd]|=0xD000|(rs<<0);
                    break;
            }
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }


// bnv -----------------------------------------------------------
        if(strncmp(&newline[ra],"bnv ",4)==0)
        {
            ra+=4;
            mem[curradd]=0x0800;
            ra=parse_branch(ra,&rc); if(ra==0) return(1);
            switch(rc)
            {
                case 1: //pc relative
                    mem[curradd]|=0xC000|rd;
                    break;
                case 2: //label, pc relative
                    mem[curradd]|=0xC000;
                    mem[curradd]|=0x03000000;
                    break;
                case 3: //register
                    mem[curradd]|=0xD000|(rs<<0);
                    break;
            }
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }

// bsg -----------------------------------------------------------
        if(strncmp(&newline[ra],"bsg ",4)==0)
        {
            ra+=4;
            mem[curradd]=0x0900;
            ra=parse_branch(ra,&rc); if(ra==0) return(1);
            switch(rc)
            {
                case 1: //pc relative
                    mem[curradd]|=0xC000|rd;
                    break;
                case 2: //label, pc relative
                    mem[curradd]|=0xC000;
                    mem[curradd]|=0x03000000;
                    break;
                case 3: //register
                    mem[curradd]|=0xD000|(rs<<0);
                    break;
            }
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }


// bsl -----------------------------------------------------------
        if(strncmp(&newline[ra],"bsl ",4)==0)
        {
            ra+=4;
            mem[curradd]=0x0A00;
            ra=parse_branch(ra,&rc); if(ra==0) return(1);
            switch(rc)
            {
                case 1: //pc relative
                    mem[curradd]|=0xC000|rd;
                    break;
                case 2: //label, pc relative
                    mem[curradd]|=0xC000;
                    mem[curradd]|=0x03000000;
                    break;
                case 3: //register
                    mem[curradd]|=0xD000|(rs<<0);
                    break;
            }
            mem[curradd]|=0x80000000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }


// halt -----------------------------------------------------------
        if(strncmp(&newline[ra],"halt",4)==0)
        {
            if((newline[ra+4]==0x20)||(newline[ra+4]==0))
            {
                ra+=4;
                mem[curradd]=0xFFFF;
                mem[curradd]|=0x80000000;
                curradd++;
                if(rest_of_line(ra)) return(1);
                continue;
            }
        }


// -----------------------------------------------------------
        printf("<%u> Error: syntax error\n",line);
        return(1);

    }
    return(0);
}



int main ( int argc, char *argv[] )
{
    int ret;
    unsigned int ra;
    unsigned int rb;

    if(argc!=2)
    {
        printf("lsa-as filename\n");
        return(1);
    }

    fpin=fopen(argv[1],"rt");
    if(fpin==NULL)
    {
        printf("Error opening file [%s]\n",argv[1]);
        return(1);
    }

    sprintf(newline,"%s.o",argv[1]);
    fpout=fopen(newline,"wt");
    if(fpout==NULL)
    {
        printf("Error creating file [%s]\n",newline);
        fclose(fpin);
        return(1);
    }
    fclose(fpout);

    memset(mem,0x00,sizeof(mem));

    ret=assemble();
    fclose(fpin);
    if(ret) return(1);


    for(ra=0;ra<=0x10000;ra++)
    {
        if(mem[ra]&0x80000000) printf("mem[%04X]=0x%08X\n",ra,mem[ra]);
    }

    printf("lab:\n");
    for(lab=0;lab<nlabs;lab++) printf("<%u> [%u][%04X][%s]\n",lab_struct[lab].line,lab_struct[lab].type,lab_struct[lab].addr,lab_struct[lab].name);

    //fix up offsets, lpc, spc
    for(lab=0;lab<nlabs;lab++)
    {
        ra=0;
        if(lab_struct[lab].type==LAB_TYPE_LPC) ra++;
        if(lab_struct[lab].type==LAB_TYPE_SPC) ra++;
        if(ra==0) continue;
        for(ra=0;ra<nlabs;ra++)
        {
            if(lab_struct[ra].type) continue;
            if(strcmp(lab_struct[ra].name,lab_struct[lab].name)==0)
            {
                if(lab_struct[ra].addr<=lab_struct[lab].addr)
                {
                    printf("<%u> Error: must be a forward reference\n",lab_struct[ra].line);
                    return(1);
                }
                rb=lab_struct[ra].addr-lab_struct[lab].addr;
                rb--;
                if(rb&(~0xFF))
                {
                    printf("<%u> Error: reference too far (%u)\n",lab_struct[ra].line,rb);
                    return(1);
                }
                mem[lab_struct[lab].addr]&=(~(0xFF));
                mem[lab_struct[lab].addr]|=rb;
                break;
            }
        }
        if(ra<nlabs)
        {
        }
        else
        {
            printf("<%u> Error: [%s] not defined\n",lab_struct[ra].line,lab_struct[ra].name);
            return(1);
        }
    }


    //fix up offsets, branch
    for(lab=0;lab<nlabs;lab++)
    {
        ra=0;
        if(lab_struct[lab].type==LAB_TYPE_BR) ra++;
        if(ra==0) continue;
        for(ra=0;ra<nlabs;ra++)
        {
            if(lab_struct[ra].type) continue;
            if(strcmp(lab_struct[ra].name,lab_struct[lab].name)==0)
            {
                rb=lab_struct[ra].addr-(lab_struct[lab].addr+1);
                if(((rb&(~0xFF))==0)||(((~rb)&(~0xFF))==0))
                {
                }
                else
                {
                    printf("<%u> Error: reference too far (%u)\n",lab_struct[ra].line,rb);
                    return(1);
                }
                mem[lab_struct[lab].addr]&=(~(0xFF));
                mem[lab_struct[lab].addr]|=rb&0xFF;
                break;
            }
        }
        if(ra<nlabs)
        {
        }
        else
        {
            printf("<%u> Error: [%s] not defined\n",lab_struct[ra].line,lab_struct[ra].name);
            return(1);
        }
    }


    //fix up addresses
    for(lab=0;lab<nlabs;lab++)
    {
        ra=0;
        if(lab_struct[lab].type==LAB_TYPE_EXT) ra++;
        if(ra==0) continue;
        for(ra=0;ra<nlabs;ra++)
        {
            if(lab_struct[ra].type) continue;
            if(strcmp(lab_struct[ra].name,lab_struct[lab].name)==0)
            {
                mem[lab_struct[lab].addr]&=(~0xFFFF);
                mem[lab_struct[lab].addr]|=lab_struct[ra].addr;
                lab_struct[lab].type=LAB_TYPE_WORD;

                break;
            }
        }
        if(ra<nlabs)
        {
        }
        else
        {
            printf("<%u> Error: [%s] not defined\n",lab_struct[ra].line,lab_struct[ra].name);
            return(1);
        }
    }


    for(ra=0;ra<=0x10000;ra++)
    {
        if(mem[ra]&0x80000000) printf("mem[%04X]=0x%08X\n",ra,mem[ra]);
    }

    sprintf(newline,"%s.csv",argv[1]);
    fpout=fopen(newline,"wt");
    if(fpout==NULL)
    {
        printf("Error creating file [%s]\n",newline);
        return(1);
    }


    for(ra=0;ra<=0x10000;ra++)
    {
        if(mem[ra]&0x80000000) fprintf(fpout,"MEM,0x%04X,0x%04X,END,,\n",ra,mem[ra]&0xFFFF);
    }
    for(lab=0;lab<nlabs;lab++)
    {
        if(lab_struct[lab].type==0)
        {
            //need to have labels specified as global
          //  fprintf(fpout,"GLOBAL,0x%04X,%s,END,,\n",lab_struct[lab].addr,lab_struct[lab].name);
        }
    }

    for(lab=0;lab<nlabs;lab++)
    {
        if(lab_struct[lab].type==LAB_TYPE_EXT)
        {
            fprintf(fpout,"EXTERN,0x%04X,%s,END,,\n",lab_struct[lab].addr,lab_struct[lab].name);
        }
    }
    fprintf(fpout,"END,,\n");
    fclose(fpout);

    //sprintf(newline,"%s.ram.cdl",argv[1]);
    //fpout=fopen(newline,"wt");
    //if(fpout==NULL)
    //{
        //printf("Error creating file [%s]\n",newline);
        //return(1);
    //}
    //for(ra=0;ra<=0x10000;ra++)
    //{
        //if(mem[ra]&0x80000000) fprintf(fpout,"case 16h%04X: { out_data =0x%04X; }\n",ra,mem[ra]&0xFFFF);
    //}
    //fclose(fpout);

    //sprintf(newline,"%s.ram.txt",argv[1]);
    //fpout=fopen(newline,"wt");
    fpout=fopen("sram.h","wt");
    if(fpout==NULL)
    {
        printf("Error creating file [%s]\n",newline);
        return(1);
    }
    for(ra=0;ra<=0x10000;ra++)
    {
        if(mem[ra]&0x80000000) fprintf(fpout,"sram[0x%04X] = 0x%04X;\n",ra,mem[ra]&0xFFFF);
    }
    fclose(fpout);

    fpout=fopen("program.cdl","wt");
    if(fpout==NULL)
    {
        printf("Error creating file [%s]\n",newline);
        return(1);
    }
    for(ra=0;ra<=0x10000;ra++)
    {
        if(mem[ra]&0x80000000) fprintf(fpout,"case 16h%04X: { mem_rdata = 0x%04X; }\n",ra,mem[ra]&0xFFFF);
    }
    fclose(fpout);


    return(0);
}
