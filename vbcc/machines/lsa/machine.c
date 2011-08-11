
//-------------------------------------------------------------------
//-------------------------------------------------------------------

#include "supp.h"

static char FILE_[]=__FILE__;

char cg_copyright[]="vbcc lsa code-generator";

int g_flags[MAXGF]={0};

char *g_flags_name[MAXGF]={"none"};

union ppi g_flags_val[MAXGF];

zmax char_bit;
zmax align[MAX_TYPE+1];
zmax maxalign;
zmax sizetab[MAX_TYPE+1];
zmax t_min[MAX_TYPE+1];
zumax t_max[MAX_TYPE+1];
zumax tu_max[MAX_TYPE+1];

char *regnames[MAXR+1];
zmax regsize[MAXR+1];
int regscratch[MAXR+1];
int regsa[MAXR+1];
int reg_prio[MAXR+1];

struct Typ *regtype[MAXR+1];

struct reg_handle empty_reg_handle={0};

static struct Typ ltyp={SHORT};

static char *marray[]={0,0};

int q1reg,q2reg;
//FIRST_GPR+0 program counter
//FIRST_GPR+1 stack pointer
//FIRST_GPR+2 flags
#define CALLREG (FIRST_GPR+3)
#define Q1REG (FIRST_GPR+4)
#define Q2REG (FIRST_GPR+5)

#define REGLOCALS 256
static unsigned int reglocal[REGLOCALS];

#define CALLNAMES 256
static char callname[CALLNAMES][256];
static unsigned int callnum[CALLNAMES];
static unsigned int ncallnames;
static unsigned int zzcallnum;

static int first_gen_code=0;

static char codestring[256];

char * show_code ( int code )
{
    switch(code)
    {
        case KOMMA       : sprintf(codestring,"code %2d KOMMA        ",code); break;
        case ASSIGN      : sprintf(codestring,"code %2d ASSIGN       ",code); break;
        case ASSIGNOP    : sprintf(codestring,"code %2d ASSIGNOP     ",code); break;

        case COND        : sprintf(codestring,"code %2d COND         ",code); break;
        case LOR         : sprintf(codestring,"code %2d LOR          ",code); break;
        case LAND        : sprintf(codestring,"code %2d LAND         ",code); break;
        case OR          : sprintf(codestring,"code %2d OR           ",code); break;
        case XOR         : sprintf(codestring,"code %2d XOR          ",code); break;
        case AND         : sprintf(codestring,"code %2d AND          ",code); break;
        case EQUAL       : sprintf(codestring,"code %2d EQUAL        ",code); break;
        case INEQUAL     : sprintf(codestring,"code %2d INEQUAL      ",code); break;
        case LESS        : sprintf(codestring,"code %2d LESS         ",code); break;
        case LESSEQ      : sprintf(codestring,"code %2d LESSEQ       ",code); break;
        case GREATER     : sprintf(codestring,"code %2d GREATER      ",code); break;
        case GREATEREQ   : sprintf(codestring,"code %2d GREATEREQ    ",code); break;

        case LSHIFT      : sprintf(codestring,"code %2d LSHIFT       ",code); break;
        case RSHIFT      : sprintf(codestring,"code %2d RSHIFT       ",code); break;
        case ADD         : sprintf(codestring,"code %2d ADD          ",code); break;
        case SUB         : sprintf(codestring,"code %2d SUB          ",code); break;
        case MULT        : sprintf(codestring,"code %2d MULT         ",code); break;
        case DIV         : sprintf(codestring,"code %2d DIV          ",code); break;
        case MOD         : sprintf(codestring,"code %2d MOD          ",code); break;
        case NEGATION    : sprintf(codestring,"code %2d NEGATION     ",code); break;
        case KOMPLEMENT  : sprintf(codestring,"code %2d KOMPLEMENT   ",code); break;
        case PREINC      : sprintf(codestring,"code %2d PREINC       ",code); break;
        case POSTINC     : sprintf(codestring,"code %2d POSTINC      ",code); break;
        case PREDEC      : sprintf(codestring,"code %2d PREDEC       ",code); break;
        case POSTDEC     : sprintf(codestring,"code %2d POSTDEC      ",code); break;
        case MINUS       : sprintf(codestring,"code %2d MINUS        ",code); break;
        case CONTENT     : sprintf(codestring,"code %2d CONTENT      ",code); break;
        case ADDRESS     : sprintf(codestring,"code %2d ADDRESS      ",code); break;
        case CAST        : sprintf(codestring,"code %2d CAST         ",code); break;
        case CALL        : sprintf(codestring,"code %2d CALL         ",code); break;
        case INDEX       : sprintf(codestring,"code %2d INDEX        ",code); break;
        case DPSTRUCT    : sprintf(codestring,"code %2d DPSTRUCT     ",code); break;
        case DSTRUCT     : sprintf(codestring,"code %2d DSTRUCT      ",code); break;
        case IDENTIFIER  : sprintf(codestring,"code %2d IDENTIFIER   ",code); break;
        case CEXPR       : sprintf(codestring,"code %2d CEXPR        ",code); break;
        case STRING      : sprintf(codestring,"code %2d STRING       ",code); break;
        case MEMBER      : sprintf(codestring,"code %2d MEMBER       ",code); break;
        case CONVERT     : sprintf(codestring,"code %2d CONVERT      ",code); break;

        case ADDRESSA    : sprintf(codestring,"code %2d ADDRESSA     ",code); break;
        case FIRSTELEMENT: sprintf(codestring,"code %2d FIRSTELEMENT ",code); break;
        case PMULT       : sprintf(codestring,"code %2d PMULT        ",code); break;
        case ALLOCREG    : sprintf(codestring,"code %2d ALLOCREG     ",code); break;
        case FREEREG     : sprintf(codestring,"code %2d FREEREG      ",code); break;
        case PCEXPR      : sprintf(codestring,"code %2d PCEXPR       ",code); break;
        case TEST        : sprintf(codestring,"code %2d TEST         ",code); break;
        case LABEL       : sprintf(codestring,"code %2d LABEL        ",code); break;
        case BEQ         : sprintf(codestring,"code %2d BEQ          ",code); break;
        case BNE         : sprintf(codestring,"code %2d BNE          ",code); break;
        case BLT         : sprintf(codestring,"code %2d BLT          ",code); break;
        case BGE         : sprintf(codestring,"code %2d BGE          ",code); break;
        case BLE         : sprintf(codestring,"code %2d BLE          ",code); break;
        case BGT         : sprintf(codestring,"code %2d BGT          ",code); break;
        case BRA         : sprintf(codestring,"code %2d BRA          ",code); break;
        case COMPARE     : sprintf(codestring,"code %2d COMPARE      ",code); break;
        case PUSH        : sprintf(codestring,"code %2d PUSH         ",code); break;
        case POP         : sprintf(codestring,"code %2d POP          ",code); break;
        case ADDRESSS    : sprintf(codestring,"code %2d ADDRESSS     ",code); break;
        case ADDI2P      : sprintf(codestring,"code %2d ADDI2P       ",code); break;
        case SUBIFP      : sprintf(codestring,"code %2d SUBIFP       ",code); break;
        case SUBPFP      : sprintf(codestring,"code %2d SUBPFP       ",code); break;
        case PUSHREG     : sprintf(codestring,"code %2d PUSHREG      ",code); break;
        case POPREG      : sprintf(codestring,"code %2d POPREG       ",code); break;
        case POPARGS     : sprintf(codestring,"code %2d POPARGS      ",code); break;
        case SAVEREGS    : sprintf(codestring,"code %2d SAVEREGS     ",code); break;
        case RESTOREREGS : sprintf(codestring,"code %2d RESTOREREGS  ",code); break;
        case ILABEL      : sprintf(codestring,"code %2d ILABEL       ",code); break;
        case DC          : sprintf(codestring,"code %2d DC           ",code); break;
        case ALIGN       : sprintf(codestring,"code %2d ALIGN        ",code); break;
        case COLON       : sprintf(codestring,"code %2d COLON        ",code); break;
        case GETRETURN   : sprintf(codestring,"code %2d GETRETURN    ",code); break;
        case SETRETURN   : sprintf(codestring,"code %2d SETRETURN    ",code); break;
        case MOVEFROMREG : sprintf(codestring,"code %2d MOVEFROMREG  ",code); break;
        case MOVETOREG   : sprintf(codestring,"code %2d MOVETOREG    ",code); break;
        case NOP         : sprintf(codestring,"code %2d NOP          ",code); break;
        case BITFIELD    : sprintf(codestring,"code %2d BITFIELD     ",code); break;

        default: sprintf(codestring,"code %2d UNKNOWN ",code);
    }
    return(codestring);
}


unsigned long getval(union atyps *p,int t)
{
  t&=NU;
  if(t==CHAR)               return(zm2l(zc2zm(p->vchar)));
  if(t==(UNSIGNED|CHAR))    return(zum2ul(zuc2zum(p->vuchar)));
  if(t==SHORT)              return(zm2l(zs2zm(p->vshort)));
  if(t==(UNSIGNED|SHORT))   return(zum2ul(zus2zum(p->vushort)));
  if(t==FLOAT)              ierror(0);
  if(t==DOUBLE)             ierror(0);
  if(t==LDOUBLE)            ierror(0);
  if(t==INT)                return(zm2l(zi2zm(p->vint)));
  if(t==(UNSIGNED|INT))     return(zum2ul(zui2zum(p->vuint)));
  if(t==LONG)               return(zm2l(zl2zm(p->vlong)));
  if(t==(UNSIGNED|LONG))    return(zum2ul(zul2zum(p->vulong)));
  if(t==LLONG)              return(zm2l(zll2zm(p->vllong)));
  if(t==(UNSIGNED|LLONG))   return(zum2ul(zull2zum(p->vullong)));
  if(t==MAXINT)             return(zm2l(p->vmax));
  if(t==(UNSIGNED|MAXINT))  return(zum2ul(p->vumax));
  /*FIXME*/
  if(t==POINTER)            return(zum2ul(zul2zum(p->vulong)));
}


//-------------------------------------------------------------------
static void load_imm(FILE *f,int r,unsigned short val)
{
    if(val==0)
    {
        emit(f,"\txor\t%s,%s",regnames[r],regnames[r]);
    }
    else
    {
        if(val&0x00FF)
        {
            if(val&0xFF00)
            {
                emit(f,"\tll\t%s,#0x%02X\n",regnames[r],(val>>0)&0xFF);
                emit(f,"\tlh\t%s,#0x%02X\n",regnames[r],(val>>8)&0xFF);
            }
            else
            {
                emit(f,"\tllz\t%s,#0x%02X\n",regnames[r],(val>>0)&0xFF);
            }
        }
        else
        {
            emit(f,"\tlhz\t%s,#0x%02X\n",regnames[r],(val>>8)&0xFF);
        }
    }
}
//-------------------------------------------------------------------
static void load_reg(FILE *f,int r,struct obj *o,int t)
{
    printf("load_reg o->flags 0x%0X t 0x%X %d ",o->flags,t,o->reg);
    switch(o->flags)
    {
        case KONST:
        {
            unsigned short val;
            printf("KONST ");
            val=(unsigned short)getval(&o->val,t);
            load_imm(f,r,val);
            break;
        }
        case REG:
        {
            printf("REG ");
            if(o->reg==r) { emit(f,";"); }
            //else
            {
                emit(f,"\tmov\t%s,%s\n",regnames[r],regnames[o->reg]);
            }
            break;
        }
        case VAR:
        {
            printf("VAR ");

            if(o->v->reg)
            {
                printf("reg\n");
                ierror(0);
            }
            else
            {
                if(o->flags&REG)
                {
                   printf("+%s",regnames[o->reg]);
                   ierror(0);
                }
                else if(o->v->storage_class==AUTO||o->v->storage_class==REGISTER)
                {
                    printf("+%ld(FP)",zm2l(o->v->offset));
                    ierror(0);
                }
                else
                {
                    if(o->v->storage_class==STATIC)
                    {
                        emit(f,"\tlpc\t%s,L%ld\n",regnames[r],zm2l(o->v->offset));
                    }
                    else
                    {
                        emit(f,"\tlpc\t%s,%s\n",regnames[r],o->v->identifier);
                    }
                }
            }
            break;
        }
        case REG|SCRATCH:
        {
            printf("REG|SCRATCH ");
            if(o->reg==r) { emit(f,";"); }
            //else
            {
                emit(f,"\tmov\t%s,%s\n",regnames[r],regnames[o->reg]);
            }
            break;
        }
        case REG|VAR:
        {
            printf("REG|VAR ");
            emit(f,"REG|VAR");
            break;
        }
        case KONST|DREFOBJ:
        {
            printf("KONST|DREFOBJ ");
            emit(f,"KONST|DREFOBJ");
            break;
        }
        case REG|DREFOBJ:
        {
            printf("REG|DREFOBJ ");
            emit(f,"REG|DREFOBJ");
            break;
        }
        case VAR|DREFOBJ:
        {
            printf("VAR|DREFOBJ ");
            emit(f,"VAR|DREFOBJ");
            break;
        }
        default:
        {
            printf("UNKNOWN ");
            emit(f,"UNKNOWN");
            break;
        }
    }
    printf("\n");
}
//-------------------------------------------------------------------
void store_result(FILE *f,int r,struct IC *p)
{
    int t;
    struct obj *o;

    t=ztyp(p);
    o=&(p->z);
    printf("store_result o->flags 0x%0X t 0x%X ",o->flags,t);
    switch(o->flags)
    {
        case KONST:
        {
            printf("KONST\n");
            ierror(0);
            break;
        }
        case REG:
        {
            printf("REG ");
            if(o->reg==r) { emit(f,";"); }
            //else
            {
                emit(f,"\tmov\t%s,%s\n",regnames[o->reg],regnames[r]);
            }
            break;
        }
        case VAR:
        {
            printf("VAR ");

            if(o->v->reg)
            {
                printf("reg\n");
                ierror(0);
            }
            else
            {
                if(o->flags&REG)
                {
                   printf("+%s",regnames[o->reg]);
                   ierror(0);
                }
                else if(o->v->storage_class==AUTO||o->v->storage_class==REGISTER)
                {
                    printf("+%ld(FP)",zm2l(o->v->offset));
                    ierror(0);
                }
                else
                {
                    if(o->v->storage_class==STATIC)
                    {
                        emit(f,"\tspc\tL%ld,%s\n",zm2l(o->v->offset),regnames[r]);
                    }
                    else
                    {
                        emit(f,"\tmov\t%s,%s\n",o->v->identifier,regnames[r]);
                    }
                }
            }
            break;
        }
        case REG|SCRATCH:
        {
            printf("REG|SCRATCH ");
            if(o->reg==r) { emit(f,";"); }
            //else
            {
                emit(f,"\tmov\t%s,%s\n",regnames[o->reg],regnames[r]);
            }
            break;
        }
        case REG|VAR:
        {
            printf("REG|VAR ");
            emit(f,"REG|VAR");
            break;
        }
        case KONST|DREFOBJ:
        {
            unsigned short val;
            printf("KONST|DREFOBJ ");
            val=(unsigned short)getval(&o->val,t);
            if(r==Q1REG)
            {
                load_imm(f,Q2REG,val);
                emit(f,"\tstw\t[%s],%s\n",regnames[Q2REG],regnames[r]);
            }
            else
            {
                load_imm(f,Q1REG,val);
                emit(f,"\tstw\t[%s],%s\n",regnames[Q1REG],regnames[r]);
            }
            break;
        }
        case REG|DREFOBJ:
        {
            printf("REG|DREFOBJ ");
            emit(f,"REG|DREFOBJ");
            break;
        }
        case VAR|DREFOBJ:
        {
            printf("VAR|DREFOBJ ");
            emit(f,"VAR|DREFOBJ");
            break;
        }
        default:
        {
            printf("UNKNOWN ");
            emit(f,"UNKNOWN");
            break;
        }
    }
    printf("\n");
}
//-------------------------------------------------------------------
static void use_registers(struct obj *o)
{
    unsigned int ra;
    int sa;

    printf("use_registers o->flags 0x%0X\n",o->flags);

    switch(o->flags&(~DONTREGISTERIZE))
    {
        case KONST:
        case REG:
        {
            break;
        }
        case VAR:
        {
            if(o->v->reg)
            {
                o->flags=REG;
                o->reg=o->v->reg;
printf("vreg %d\n",o->v->reg);
printf("-- is reg %s\n",regnames[o->reg]);
                return;
            }
            else
            {
                if(o->flags&REG)
                {
                }
                else if(o->v->storage_class==AUTO||o->v->storage_class==REGISTER)
                {
                    sa=(int)(zm2l(o->v->offset));
//printf("sa %d\n",sa);

                    if(sa>=0)
                    {
                        if(sa<REGLOCALS)
                        {
                            if(reglocal[sa])
                            {
                                o->flags=REG;
                                o->reg=reglocal[sa];
printf("-- already used reg %s\n",regnames[o->reg]);
                                return;
                            }
                            else
                            {
//for(ra=FIRST_GPR+GPR_ARGS;ra<=LAST_GPR;ra++)
//{
    //if(regused[ra]) printf("%s %u\n",regnames[ra],regused[ra]);
//}

                                for(ra=FIRST_GPR+GPR_ARGS;ra<=LAST_GPR;ra++)
                                {
                                    if(regused[ra]==0) break;
                                }
                                if(ra<=LAST_GPR)
                                {
                    //printf("+%ld(FP) -> %s\n",zm2l(o->v->offset),regnames[ra]);
                                    regused[ra]++;
                                    o->flags=REG;
                                    o->reg=ra;
                                    reglocal[sa]=ra;
printf("-- use reg %s\n",regnames[o->reg]);
                                    return;
                                }
                            }
                        }
                    }
                    printf("use_registers +%ld(FP)\n",zm2l(o->v->offset));
                    ierror(0);
                }
                else
                {
                    if(o->v->storage_class==STATIC)
                    {
                    }
                    else
                    {
                    }
                }
            }
            break;
        }
        case REG|SCRATCH:
        case REG|VAR:
        case KONST|DREFOBJ:
        case REG|DREFOBJ:
        case VAR|DREFOBJ:
        default:
        {
            break;
        }
    }
}
//-------------------------------------------------------------------
void one_op ( struct IC *p )
{
    q1reg=Q1REG;

//printf("p->typf 0x%X\n",p->typf);
//printf("p->q1.flags 0x%X\n",p->q1.flags);
    //this optimization is okay so long as it is limited to a register
    //to register mov.
    if(p->z.flags==REG)
    {
        switch(p->q1.flags)
        {
            case KONST:
            {
                if(p->z.reg<FIRST_HIGH_GPR)
                {
                    q1reg=p->z.reg;
                }
                break;
            }
            case REG:
            {
                q1reg=p->z.reg;
                break;
            }
        }
    }
}
//-------------------------------------------------------------------
void two_op ( struct IC *p )
{
    q1reg=Q1REG;
    q2reg=Q2REG;
    //this takes more work
    //if((p->q1.flags==REG)||(p->q1.flags==(REG|SCRATCH)))
    //{
        //if(p->z.flags==REG)
        //{
            //q1reg=p->z.reg;
        //}
        //else
        //{
            //q1reg=p->z.reg;
        //}
    //}
    //if((p->q2.flags==REG)||(p->q2.flags==(REG|SCRATCH)))
    //{
        //q2reg=p->q2.reg;
    //}
}
//-------------------------------------------------------------------
int init_cg(void)
{
    int i;

    first_gen_code=0;

    char_bit=l2zm(8L);

    for(i=0;i<=MAX_TYPE;i++)
    {
        align[i]=l2zm(2L);
    }
    align[0]=l2zm(1L);
    align[CHAR]=l2zm(1L);

    maxalign=l2zm(2L);

    for(i=0;i<=MAX_TYPE;i++)
    {
        sizetab[i]=l2zm(2L);
    }
    sizetab[0]=l2zm(1L);
    sizetab[CHAR]=l2zm(1L);

    //t_min[CHAR]=l2zm(-128L);
    t_min[CHAR]=l2zm(-32768L);
    t_min[SHORT]=l2zm(-32768L);
    t_min[INT]=t_min[SHORT];
    t_min[LONG]=t_min[SHORT];
    t_min[LLONG]=t_min[SHORT];
    t_min[MAXINT]=t_min[SHORT];

    //t_max[CHAR]=ul2zum(127L);
    t_max[CHAR]=ul2zum(32767UL);
    t_max[SHORT]=ul2zum(32767UL);
    t_max[INT]=t_max[SHORT];
    t_max[LONG]=t_max[SHORT];
    t_max[LLONG]=t_max[SHORT];
    t_max[MAXINT]=t_max[SHORT];

    //tu_max[CHAR]=ul2zum(255UL);
    tu_max[CHAR]=ul2zum(65535UL);
    tu_max[SHORT]=ul2zum(65535UL);
    tu_max[INT]=tu_max[SHORT];
    tu_max[LONG]=tu_max[SHORT];
    tu_max[LLONG]=tu_max[SHORT];
    tu_max[MAXINT]=tu_max[SHORT];


    regnames[0]="noreg";
    for(i=FIRST_GPR;i<=LAST_GPR;i++)
    {
        regnames[i]=mymalloc(16);
        sprintf(regnames[i],"r%u",(i-FIRST_GPR));
        regsize[i]=l2zm(2L);
        regtype[i]=&ltyp;
    }

    for(i=1;i<=MAXR;i++)
    {
        regsa[i]=0;
        regscratch[i]=1;
        reg_prio[i]=(MAXR+GPR_ARGS+3)-i;
    }

    for(i=FIRST_GPR;i<FIRST_BULK_GPR;i++)
    {
        regsa[i]=1;
        regscratch[i]=0;
        reg_prio[i]=1;
    }

    for(i=1;i<=GPR_ARGS;i++)
    {
//        reg_prio[i] = 2+i;
    }

    target_macros=marray;

    zzcallnum=1;
    return 1;
}
//-------------------------------------------------------------------
void init_db(FILE *f)
{
}
//-------------------------------------------------------------------
int freturn(struct Typ *t)
{
    return RETURN_GPR;
}
//-------------------------------------------------------------------
int reg_pair(int r,struct rpair *p)
{
    return 0;
}
//-------------------------------------------------------------------
//int cost_savings(struct IC *p,int r,struct obj *o)
//{
    //return(INT_MIN);
//}
//-------------------------------------------------------------------
int regok(int r,int t,int mode)
{
    if(r==0) return 0;
    t&=NQ;
    if(t==0) return 0;
    if(ISFLOAT(t)) return 0;
    if(t==POINTER&&r>=FIRST_GPR&&r<=LAST_GPR) return 1;
    if(t>=CHAR&&t<=LONG&&r>=FIRST_GPR&&r<=LAST_GPR) return 1;
    return 0;
}
//-------------------------------------------------------------------
int dangerous_IC(struct IC *p)
{
    int c;

    c=p->code;
    if(c==MULT) return(1);
    if(c==DIV) return(1);
    if(c==MOD) return(1);
    if((p->typf&NQ)==FLOAT) return(1);
    if((q1typ(p)&NQ)==FLOAT) return(1);
    if((q2typ(p)&NQ)==FLOAT) return(1);
    if((ztyp(p)&NQ)==FLOAT) return(1);

    return 0;
}
//-------------------------------------------------------------------
int must_convert(int o,int t,int const_expr)
{
    return 0;
}
//-------------------------------------------------------------------
void gen_ds(FILE *f,zmax size,struct Typ *t)
{
    unsigned int ra;
    unsigned int rb;

    printf("gen_ds()\n");

    ra=(unsigned int)zm2l(size);
    for(rb=0;rb<ra;rb+=2) emit(f,"\t.word 0\n");
}
//-------------------------------------------------------------------
void gen_dc(FILE *f,int t,struct const_list *p)
{
    printf("gen_dc()\n");

    if(!p->tree)
    {
        if(ISFLOAT(t))
        {
            ierror(0);
        }
        else
        {
            emit(f,"\t.word ");
            emitval(f,&p->val,t&NU);
            emit(f,"\n");
        }
    }
    else
    {
        ierror(0);
//        emit_obj(f,&p->tree->o,t&NU);
    }
}
//-------------------------------------------------------------------
void gen_align(FILE *f,zmax align)
{
    printf("gen_align\n");
}
//-------------------------------------------------------------------
void gen_var_head(FILE *f,struct Var *v)
{
    printf("gen_var_head()\n");

    if(v->clist) printf("is_const %d\n",is_const(v->vtyp));
    if(v->storage_class==STATIC)
    {
        if(ISFUNC(v->vtyp->flags))
        {
            printf("ISFUNC\n");
            return;
        }
        emit(f,"L%ld:\n",zm2l(v->offset));
    }
    if(v->storage_class==EXTERN)
    {
        //emit(f,"\t.globl\t%s\n",v->identifier);
        if(v->flags&(DEFINED|TENTATIVE))
        {
            emit(f,"%s:\n",v->identifier);
        }
    }
}
//-------------------------------------------------------------------
void gen_code ( FILE *f, struct IC *p, struct Var *v, zmax offset )
{
    int c,t,i;
    struct IC *m;
    int cmpsign;
    int savelr;

    cmpsign=UNSIGNED;
    savelr=0;

printf("\n\ngen_code()\n");


    memset(reglocal,0x00,sizeof(reglocal));

    if(first_gen_code==0)
    {
        first_gen_code=1;

        emit(f,"\n");
        emit(f,"\tb reset\n");
        emit(f,"\tb reset\n"); //interrupt
        emit(f,"\tb reset\n");
        emit(f,"\tb reset\n");
        emit(f,"\n");
        emit(f,"hang:\n");
        emit(f,"\thalt\n");
        emit(f,"\tb\thang\n");
        emit(f,"\n");
        emit(f,"PUT16:\n");
        emit(f,"\tstw\t[%s],%s\n",regnames[RETURN_GPR+0],regnames[RETURN_GPR+1]);
        emit(f,"\tmov\t%s,%s\n",regnames[FIRST_GPR],regnames[CALLREG]);
        emit(f,"\n");
        emit(f,"GET16:\n");
        emit(f,"\tldw\t%s,[%s]\n",regnames[RETURN_GPR+0],regnames[RETURN_GPR+0]);
        emit(f,"\tmov\t%s,%s\n",regnames[FIRST_GPR],regnames[CALLREG]);
        emit(f,"\n");
    }

    for(c=1;c<=MAXR;c++)
    {
        regs[c]=regsa[c];
    }

    for(c=1;c<=MAXR;c++)
    {
        if((regsa[c])||(regused[c]))
        {
            BSET(regs_modified,c);
        }
    }

    //print function name
    if(v->vattr)
    {
        printf("v->vattr [%s]\n",v->vattr);
    }
    else
    {
        //emit(f,"\t.text\n");
    }
    if(v->storage_class==EXTERN)
    {
        if((v->flags&(INLINEFUNC|INLINEEXT))!=INLINEFUNC)
        {
            //emit(f,"\t.global\t%s\n",v->identifier);
        }
        emit(f,"\n\n%s:\n",v->identifier);
        if(strcmp(v->identifier,"reset")==0)
        {
            emit(f,"\tlhz\tr2,#0x10\n");
        }
printf("%s:\n",v->identifier);
    }
    else
    {
        emit(f,"L%ld:\n",zm2l(v->offset));
    }

printf("----\n");
for(m=p;m;m=m->next)
{
    printf("%s",show_code(m->code));
    pric2(stdout,m);
}
printf("----\n");

    savelr=0;


//for(i=FIRST_GPR+GPR_ARGS;i<=LAST_GPR;i++)
//{
    //if(regused[i])
    //printf("%s %u\n",regnames[i],regused[i]);
//}


for(m=p;m;m=m->next)
{
    c=m->code;
    printf("%s",show_code(m->code));
    pric2(stdout,m);
    if(c==NOP) { m->z.flags=0; continue; }
    if(c==ALLOCREG) { regs[m->q1.reg]=1; continue; }
    if(c==FREEREG)  { regs[m->q1.reg]=0; continue; }
    if(c==CALL) { savelr=1; }
    use_registers(&m->q1);
    use_registers(&m->q2);
    use_registers(&m->z);

}

printf("----\n");
for(m=p;m;m=m->next)
{
    printf("%s",show_code(m->code));
    pric2(stdout,m);
}
printf("----\n");

    //peephole(p);

    ncallnames=0;

    if(savelr)
    {
        emit(f,"\tpush\t%s\n",regnames[CALLREG]);
    }
    for(c=FIRST_BULK_GPR;c<=LAST_GPR;c++)
    {
        if(regused[c])
        {
            if(c>=FIRST_HIGH_GPR)
            {
                emit(f,"\tmov\t%s,%s\n",regnames[Q1REG],regnames[c]);
                emit(f,"\tpush\t%s\n",regnames[Q1REG]);
            }
            else
            {
                emit(f,"\tpush\t%s\n",regnames[c]);
            }
        }
    }
    for(m=p;m;m=m->next)
    {
        c=m->code;
        t=m->typf;

        q1reg=0;
        q2reg=0;

        printf("%s",show_code(c));
        pric2(stdout,m);
        emit(f,"                               ;%s\n",codestring);

        if(c==NOP)
        {
            m->z.flags=0;
            continue;
        }
        if(c==ALLOCREG)
        {
            regs[m->q1.reg]=1;
            continue;
        }
        if(c==FREEREG)
        {
            regs[m->q1.reg]=0;
            continue;
        }
        if(c==LABEL)
        {
            emit(f,"L%d:\n",t);
            continue;
        }
        if(c==ASSIGN)
        {
            one_op(m);
            //Copy q1 to z. q1->z
            printf("opsize %ld\n",zm2l(opsize(m)));
            load_reg(f,q1reg,&m->q1,t);
            store_result(f,q1reg,m);
            continue;
        }
        if(c==OR)
        {
            two_op(m);
            //Bitwise boolean operations. q1,q2->z.
            //All operands are integers.
            load_reg(f,q1reg,&m->q1,t);
            load_reg(f,q2reg,&m->q2,t);
            emit(f,"\tor\t%s,%s\n",regnames[q1reg],regnames[q2reg]);
            store_result(f,q1reg,m);
            continue;
        }
        if(c==XOR)
        {
            two_op(m);
            //Bitwise boolean operations. q1,q2->z.
            //All operands are integers.
            load_reg(f,q1reg,&m->q1,t);
            load_reg(f,q2reg,&m->q2,t);
            emit(f,"\txor\t%s,%s\n",regnames[q1reg],regnames[q2reg]);
            store_result(f,q1reg,m);
            continue;
        }
        if(c==AND)
        {
            two_op(m);
            //Bitwise boolean operations. q1,q2->z.
            //All operands are integers.
            load_reg(f,q1reg,&m->q1,t);
            load_reg(f,q2reg,&m->q2,t);
            emit(f,"\tand\t%s,%s\n",regnames[q1reg],regnames[q2reg]);
            store_result(f,q1reg,m);
            continue;
        }
        if(c==LSHIFT)
        {
            two_op(m);
            load_reg(f,q1reg,&m->q1,t);
            load_reg(f,q2reg,&m->q2,t);
            emit(f,"\tlsl\t%s,%s\n",regnames[q1reg],regnames[q2reg]);
            store_result(f,q1reg,m);
            continue;
        }
        if(c==RSHIFT)
        {
            two_op(m);
            load_reg(f,q1reg,&m->q1,t);
            load_reg(f,q2reg,&m->q2,t);
            emit(f,"\tlsr\t%s,%s\n",regnames[q1reg],regnames[q2reg]);
            store_result(f,q1reg,m);
            continue;
        }
        if(c==ADD)
        {
            two_op(m);
            //Standard arithmetic operations. q1,q2->z.
            //All operands are of arithmetic types (integers or floating point).
            load_reg(f,q1reg,&m->q1,t);
            load_reg(f,q2reg,&m->q2,t);
            emit(f,"\tadd\t%s,%s\n",regnames[q1reg],regnames[q2reg]);
            store_result(f,q1reg,m);
            continue;
        }
        if(c==SUB)
        {
            two_op(m);
            //Standard arithmetic operations. q1,q2->z.
            //All operands are of arithmetic types (integers or floating point).
            load_reg(f,q1reg,&m->q1,t);
            load_reg(f,q2reg,&m->q2,t);
            emit(f,"\tsub\t%s,%s\n",regnames[q1reg],regnames[q2reg]);
            store_result(f,q1reg,m);
            continue;
        }
        if(c==MULT)
        {
            ierror(0);
            continue;
        }
        if(c==DIV)
        {
            ierror(0);
            continue;
        }
        if(c==MOD)
        {
            ierror(0);
            continue;
        }
        if(c==KOMPLEMENT)
        {
            //Bitwise complement. q1->z
            //All operands are integers.
            load_reg(f,Q1REG,&m->q1,t);
            emit(f,"\tnot\t%s,%s\n",regnames[Q1REG],regnames[Q1REG]);
            store_result(f,Q1REG,m);
            continue;
        }
        if(c==MINUS)
        {
            //Unary minus. q1->z
            load_reg(f,Q1REG,&m->q1,t);
            emit(f,"\tneg\t%s,%s\n",regnames[Q1REG],regnames[Q1REG]);
            store_result(f,Q1REG,m);
            continue;
        }
        if(c==ADDRESS)
        {
        }
        if(c==CALL)
        {
            printf("pushedargsize() %ld\n",zm2l(pushedargsize(m)));
            if(zm2l(pushedargsize(m)))
            {
                printf("PUSH SOMETHING\n");
                ierror(0);
            }
            if(m->call_cnt!=1)
            {
                int i;
                printf("call_cnt %d\n",m->call_cnt);
                for(i=0;i<m->call_cnt;i++) printf("call\t%s\n",m->call_list[i].v->identifier);
                ierror(0);
            }
            {
                unsigned int i;

                for(i=0;i<ncallnames;i++)
                {
                    if(strcmp(callname[i],m->call_list[0].v->identifier)==0) break;
                }
                if(i<ncallnames)
                {

                }
                else
                {
                    //TODO: check for limit on ncallnames
                    strcpy(callname[ncallnames],m->call_list[0].v->identifier);
                    callnum[ncallnames]=zzcallnum++;
                    i=ncallnames;
                    ncallnames++;
                }
                emit(f,"\tlpc\t%s,zz%04u_%s\n",regnames[Q1REG],callnum[i],m->call_list[0].v->identifier);
            }
            emit(f,"\tcall\t%s,%s\n",regnames[CALLREG],regnames[Q1REG]);
            continue;
        }
        if(c==CONVERT)
        {
        }
        if(c==COMPARE)
        {
            cmpsign=UNSIGNED&t; //m->typf
            load_reg(f,Q1REG,&m->q1,t);
            load_reg(f,Q2REG,&m->q2,t);
            emit(f,"\tsub\t%s,%s\n",regnames[Q1REG],regnames[Q2REG]);
            continue;
        }
        if(c==TEST)
        {
            load_reg(f,Q1REG,&m->q1,t);
            emit(f,"\tllz\t%s,#0\n",regnames[Q2REG]);
            emit(f,"\tsub\t%s,%s\n",regnames[Q1REG],regnames[Q2REG]);
            continue;
        }
        if(c==BEQ)
        {
            emit(f,"\tbz\tL%d\n",t);
            continue;
        }
        if(c==BNE)
        {
            emit(f,"\tbnz\tL%d\n",t);
            continue;
        }
        if(c==BLT)
        {
            if(cmpsign&UNSIGNED)
            {
                emit(f,"\tbnc\tL%d\n",t);
            }
            else
            {
                emit(f,"\tbsl\tL%d\n",t);
            }
            continue;
        }
        if(c==BGE)
        {
            if(cmpsign&UNSIGNED)
            {
                emit(f,"\tbc\tL%d\n",t);
            }
            else
            {
                emit(f,"\tbsg\tL%d\n",t);
                emit(f,"\tbz\tL%d\n",t);
            }
            continue;
        }
        if(c==BLE)
        {
            if(cmpsign&UNSIGNED)
            {
                emit(f,"\tbnc\tL%d\n",t);
                emit(f,"\tbz\tL%d\n",t);
            }
            else
            {
                emit(f,"\tbsl\tL%d\n",t);
                emit(f,"\tbz\tL%d\n",t);
            }
            continue;
        }
        if(c==BGT)
        {
            if(cmpsign&UNSIGNED)
            {
                emit(f,"\tbug\tL%d\n",t);
            }
            else
            {
                emit(f,"\tbsg\tL%d\n",t);
            }
            continue;
        }
        if(c==BRA)
        {
            emit(f,"\tb\tL%d\n",t);
            continue;
        }
        if(c==PUSH)
        {
        }
        if(c==ADDI2P)
        {
        }
        if(c==SUBIFP)
        {
        }
        if(SUBPFP)
        {
        }
        if(c==GETRETURN)
        {
            //Get the return value of the last function call. ->z.
            //If the return value is in a register, its number will be
            //q1.reg. Otherwise q1.reg will be 0.
            if(m->q1.reg) //should always be a reg right?
            {
                store_result(f,m->q1.reg,m);
            }
            else
            {
                ierror(0);
            }
            continue;
        }
        if(c==SETRETURN)
        {
            //Set the return value of the current function. q1.
            //If the return value is in a register, its number will be
            //z.reg. Otherwise z.reg will be 0.
            if(m->z.reg)
            {
                load_reg(f,Q1REG,&m->q1,t);
                emit(f,"\tmov\t%s,%s\n",regnames[RETURN_GPR],regnames[Q1REG]);
                BSET(regs_modified,RETURN_GPR);
            }
            else
            {
                ierror(0);
            }
            continue;
        }
        if(c==MOVEFROMREG)
        {
            one_op(m);
            load_reg(f,q1reg,&m->q1,t);
            store_result(f,q1reg,m);
            continue;
        }
        if(c==MOVETOREG)
        {
            one_op(m);
            load_reg(f,q1reg,&m->q1,t);
            store_result(f,q1reg,m);
            continue;
        }

////        pric2(stdout,m);
        printf("---\n\n");
        ierror(0);
    }

    for(c=LAST_GPR;c>=FIRST_BULK_GPR;c--)
    {
        if(regused[c])
        {
            if(c>=FIRST_HIGH_GPR)
            {
                emit(f,"\tpop\t%s\n",regnames[Q2REG]);
                emit(f,"\tmov\t%s,%s\n",regnames[c],regnames[Q2REG]);
            }
            else
            {
                emit(f,"\tpop\t%s\n",regnames[c]);
            }
        }
    }
    if(savelr)
    {
        emit(f,"\tpop\t%s\n",regnames[FIRST_GPR]);
    }
    else
    {
        emit(f,"\tmov\t%s,%s\n\n",regnames[FIRST_GPR],regnames[CALLREG]);
    }

    for(i=0;i<ncallnames;i++)
    {
        emit(f,"zz%04u_%s: .word =%s\n",callnum[i],callname[i],callname[i]);
    }
}
//-------------------------------------------------------------------
int shortcut(int code,int typ)
{
    return 2;
}
//-------------------------------------------------------------------
int reg_parm ( struct reg_handle *m, struct Typ *t, int vararg, struct Typ *d )
{
    int f;
    f=t->flags&NQ;
    switch(f)
    {
        case VOID   :
            //return 0;
        case CHAR   :
        case SHORT  :
        case INT    :
        case POINTER:
            if(m->gregs>=GPR_ARGS) break;
            return(RETURN_GPR+(m->gregs++));
        case LONG   :
        case LLONG  :
        case FLOAT  :
        case DOUBLE :
        case LDOUBLE:
        //case VOID   :
        //case POINTER:
        case ARRAY  :
        case STRUCT :
        case UNION  :
        case ENUM   :
        case FUNKT  :
        default:
            break;

    }
    printf("reg_parm t->flags 0x%X\n",t->flags);
    ierror(0);
    return 0;
}
//-------------------------------------------------------------------
int handle_pragma(const char *s)
{
}
//-------------------------------------------------------------------
void cleanup_cg(FILE *f)
{
}
//-------------------------------------------------------------------
void cleanup_db(FILE *f)
{
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
