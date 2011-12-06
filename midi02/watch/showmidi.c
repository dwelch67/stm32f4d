
//-----------------------------------------------------------------------------
// Copyright (C) David Welch, 2010
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ser.h"

unsigned char rdata[5000];

//-----------------------------------------------------------------------------
int main ( int argc, char *argv[] )
{
    unsigned int ra;
    unsigned int rb;

    if(argc<2)
    {
        fprintf(stderr,"progpluto /dev/ttySOMETHING\n");
        return(1);
    }
    if(ser_open(argv[1])) return(1);


    while(1)
    {
        rb=ser_copystring(rdata);
        for(ra=0;ra<rb;ra++)
        {
            //if(rdata[ra]==0xFF) printf("\n");
            if(rdata[ra]==0xFE) printf("\n");
            fprintf(stdout,"0x%02X ",rdata[ra]); fflush(stdout);
        }
        ser_dump(rb);
    }


    ser_close();
    return(0);
}
//-----------------------------------------------------------------------------
// Copyright (C) David Welch, 2010
//-----------------------------------------------------------------------------

