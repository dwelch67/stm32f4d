//dfu-util -d 0483:df11 -c 1 -i 0 -a 0 -s 0x08000000 -D flashblinker.bin

void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );

#define RCCBASE   0x40023800
#define GPIOABASE 0x40020000

int notmain ( void )
{
    volatile unsigned int ra;
    unsigned int rx;

    ra=GET32(RCCBASE+0x30);
    ra|=1<<0; //enable port A
    PUT32(RCCBASE+0x30,ra);

    //d12 = d15 output
    ra=GET32(GPIOABASE+0x00);
    ra&=0xFFFC3FFF;
    ra|=0x00014000;
    PUT32(GPIOABASE+0x00,ra);
    //push pull
    ra=GET32(GPIOABASE+0x04);
    ra&=0xFFFE7FFF;
    PUT32(GPIOABASE+0x04,ra);
    PUT32(GPIOABASE+0x18,0x00800000); //make PA7 low/ground
    for(rx=0;;rx++)
    {
        PUT32(GPIOABASE+0x18,0x00000100);
        for(ra=0;ra<2000000;ra++) continue;
        PUT32(GPIOABASE+0x18,0x01000000);
        for(ra=0;ra<2000000;ra++) continue;
    }
    return(0);
}
