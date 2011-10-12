
//-----------------------------------------------------------------------------
// Copyright (C) David Welch, 2010
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ser.h"

#define RBFMAXSIZE  100000
unsigned char rbfdata[RBFMAXSIZE];
#define COOKEDRBF   (RBFMAXSIZE<<3)
unsigned char data[COOKEDRBF];

unsigned int rbfdatalen;

FILE *fp;

//-----------------------------------------------------------------------------
int main ( int argc, char *argv[] )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int slen;

    if(argc<3)
    {
        fprintf(stderr,"progpluto /dev/ttySOMETHING file.rbf\n");
        return(1);
    }
    fp=fopen(argv[2],"rb");
    if(fp==NULL)
    {
        fprintf(stderr,"Error opening file [%s]\n",argv[2]);
        return(1);
    }
    rbfdatalen=fread(rbfdata,1,sizeof(rbfdata),fp);
    fclose(fp);
    printf("%u bytes\n",rbfdatalen);
    slen=0;
    for(ra=0;ra<rbfdatalen;ra++)
    {
        for(rb=1;rb<0x100;rb<<=1)
        {
            if(rbfdata[ra]&rb) data[slen]=0xFF; else data[slen]=0xFE;
            slen++;
        }
    }
    printf("slen %u\n",slen);
    if(ser_open(argv[1]))
    {
        printf("ser_open(%s) failed\n",argv[1]);
        return(1);
    }
    printf("port opened\n");

    for(ra=0;ra<slen;ra++)
    {
        if((ra&0xFFF)==0) { printf("."); fflush(stdout); }
        ser_senddata(&data[ra],1);
    }
    printf("\n");

    ser_close();
    return(0);
}
//-----------------------------------------------------------------------------
// Copyright (C) David Welch, 2010
//-----------------------------------------------------------------------------

