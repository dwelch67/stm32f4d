
//-----------------------------------------------------------------------------
// Copyright (C) David Welch, 2010
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ser.h"

FILE *fpout;

unsigned char rdata[5000];

//-----------------------------------------------------------------------------
int main ( int argc, char *argv[] )
{
    unsigned int ra;
    unsigned int rb;

    if(argc<3)
    {
        fprintf(stderr,"progpluto /dev/ttySOMETHING filename\n");
        return(1);
    }
    fpout=fopen(argv[2],"wb");
    if(fpout==NULL)
    {
        printf("Error creating file [%s]\n",argv[2]);
        return(1);
    }

    if(ser_open(argv[1])) return(1);


    while(1)
    {
        rb=ser_copystring(rdata);
        if(rb)
        {
            fprintf(stdout,"."); fflush(stdout);
            fwrite(rdata,1,rb,fpout); fflush(fpout);
            ser_dump(rb);
        }
    }


    ser_close();
    fclose(fpout);
    return(0);
}
//-----------------------------------------------------------------------------
// Copyright (C) David Welch, 2010
//-----------------------------------------------------------------------------

