

#define NUM_GPRS 64
#define GPR_ARGS 16


#define RESERVED_GPRS 6

#define FIRST_GPR 1
#define LAST_GPR (FIRST_GPR+NUM_GPRS-1)
#define RETURN_GPR (FIRST_GPR+RESERVED_GPRS)
#define FIRST_BULK_GPR (FIRST_GPR+RESERVED_GPRS+GPR_ARGS)
#define FIRST_HIGH_GPR (FIRST_GPR+16)

#include "dt.h"

struct AddressingMode{
    int never_used;
};
#define MAXR NUM_GPRS
#define MAXGF 1
#define USEQ2ASZ 1
#define MINADDI2P CHAR
#define BIGENDIAN 0
#define LITTLEENDIAN 1
#define SWITCHSUBS 0
#define INLINEMEMCPY 1024
#define ORDERED_PUSH 1
#define HAVE_REGPARMS 1
struct reg_handle{
    unsigned long gregs;
};
#undef  HAVE_REGPAIRS
#define HAVE_INT_SIZET 1
#define EMIT_BUF_LEN 1024
#define EMIT_BUF_DEPTH 4
#define HAVE_TARGET_PEEPHOLE 0
#undef HAVE_TARGET_ATTRIBUTES
#undef HAVE_TARGET_PRAGMAS
#define HAVE_REGS_MODIFIED 1
#undef HAVE_TARGET_RALLOC
#undef HAVE_TARGET_EFF_IC
#undef HAVE_EXT_IC
#undef HAVE_EXT_TYPES
#undef HAVE_TGT_PRINTVAL

