//http://gitorious.org/~tormod/unofficial-clones/dfuse-dfu-util
//dfu-util -d 0483:df11 -c 1 -i 0 -a 0 -s 0x08000000 -D flashblinker.bin

void PUT32 ( unsigned int, unsigned int );
void PUT16 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
unsigned int GET16 ( unsigned int );


#define RCCBASE   0x40023800
#define GPIODBASE 0x40020C00
#define TIM5BASE  0x40000C00


void timdelay ( void )
{
    unsigned int ra;

    for(ra=1000;ra;ra--)
    {
        while((GET16(TIM5BASE+0x24)&0x8000)==0x0000) continue;
        while((GET16(TIM5BASE+0x24)&0x8000)==0x8000) continue;
    }
}


int notmain ( void )
{
    volatile unsigned int ra;
    unsigned int rx;

    ra=GET32(RCCBASE+0x00);
    ra&=~(0xF<<16);
    PUT32(RCCBASE+0x00,ra);
    ra|=1<<16;
    PUT32(RCCBASE+0x00,ra);
    while(1)
    {
        if(GET32(RCCBASE+0x00)&(1<<17)) break;
    }
    PUT32(RCCBASE+0x08,0x00000001);

    ra=GET32(RCCBASE+0x30);
    ra|=1<<3; //enable port D
    PUT32(RCCBASE+0x30,ra);

    ra=GET32(RCCBASE+0x40);
    ra|=1<<3; //enable TIM5
    PUT32(RCCBASE+0x40,ra);

    PUT16(TIM5BASE+0x00,0x0000);
    PUT16(TIM5BASE+0x00,0x0001);

    //d12 = d15 output
    ra=GET32(GPIODBASE+0x00);
    ra&=0x00FFFFFF;
    ra|=0x55000000;
    PUT32(GPIODBASE+0x00,ra);
    //push pull
    ra=GET32(GPIODBASE+0x04);
    ra&=0xFFFF0FFF;
    PUT32(GPIODBASE+0x04,ra);

    for(rx=0;;rx++)
    {
        PUT32(GPIODBASE+0x18,0xE0001000);
        timdelay();
        PUT32(GPIODBASE+0x18,0xF0000000);
        timdelay();
    }
    return(0);
}
