
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//f(VCO clock) = f(PLL clock input) × (PLLN / PLLM)
//f(PLL general clock output) = f(VCO clock) / PLLP
//f(USB OTG FS, SDIO, RNG clock output) = f(VCO clock) / PLLQ



int main ( void )
{
    unsigned int pllin;
    unsigned int vcoin;
    unsigned int pllvco;
    unsigned int pllgen;
    unsigned int pllusb;
    unsigned int n,m,p,q;

    pllin=8;

    for(n=1;n<=512;n++)
    {
        for(m=1;m<=63;m++)
        {
            vcoin=(pllin*100)/m;
            if(vcoin<100) continue;
            if(vcoin>200) continue;
            pllvco=vcoin*n;
            if(pllvco<6400) continue;
            if(pllvco>43200) continue;
            for(p=2;p<=8;p+=2)
            {
                pllgen=pllvco/p;
                if(pllgen>16800) continue;
                for(q=2;q<15;q++)
                {
                    pllusb=pllvco/q;
                    /**/if(pllusb>4800) continue;
                    /**/if(pllusb!=4800) continue;

printf("Q %u P %u N %u M %u vcoin %u pllvco %u pllgen %u pllusb %u\n",
    q,p,n,m,vcoin/100,pllvco/100,pllgen/100,pllusb/100);

                }
            }
        }
    }





}

