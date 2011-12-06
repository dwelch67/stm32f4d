
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


unsigned int pclk;

void udiv ( unsigned int des )
{
    unsigned int ra,rb,rc,rd;

    ra=pclk/des;

    rb=ra*des;

    rc=(pclk-rb)*10000;
    rc/=pclk;

    rd=pclk/ra;

    printf("%6u %6u .%02u %4u\n",des,rd,rc,ra);

}



void trypclk ( unsigned int px )
{
    printf("pclk %u\n",px);
    pclk = px;

    udiv(31250);
    udiv(38400);
}

int main ( void )
{
    unsigned int ra;

    for(ra=1;ra<5;ra++)
    {
        trypclk(ra*8000000);
    }
    trypclk(168000000/4);
    trypclk(168000000/8);
    trypclk(168000000/16);
    return(0);
}

