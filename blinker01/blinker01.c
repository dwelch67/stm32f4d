//http://gitorious.org/~tormod/unofficial-clones/dfuse-dfu-util
//dfu-util -d 0483:df11 -c 1 -i 0 -a 0 -s 0x08000000 -D flashblinker.bin

void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );

#define RCCBASE   0x40023800
#define GPIODBASE 0x40020C00

int notmain ( void )
{
    volatile unsigned int ra;
    unsigned int rx;

    ra=GET32(RCCBASE+0x30);
    ra|=1<<3; //enable port D
    PUT32(RCCBASE+0x30,ra);

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
        for(ra=0;ra<200000;ra++) continue;
        PUT32(GPIODBASE+0x18,0xD0002000);
        for(ra=0;ra<200000;ra++) continue;
        PUT32(GPIODBASE+0x18,0xB0004000);
        for(ra=0;ra<200000;ra++) continue;
        PUT32(GPIODBASE+0x18,0x70008000);
        for(ra=0;ra<200000;ra++) continue;
    }
    return(0);
}
