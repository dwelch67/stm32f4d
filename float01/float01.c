//http://gitorious.org/~tormod/unofficial-clones/dfuse-dfu-util
//dfu-util -d 0483:df11 -c 1 -i 0 -a 0 -s 0x08000000 -D flashblinker.bin

unsigned int fun ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );

#define RCCBASE   0x40023800
#define GPIODBASE 0x40020C00
#define CPACR  0xE000ED88
int notmain ( void )
{
    volatile unsigned int va;
    unsigned int ra;
    unsigned int rb;

    PUT32(CPACR,GET32(CPACR)|(0xF<<20));

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

    PUT32(GPIODBASE+0x18,0xF0000000);

    rb=fun(200,1000);
    while(1)
    {
        PUT32(GPIODBASE+0x18,0xE0001000);
        for(va=0;va<rb;va++) continue;
        PUT32(GPIODBASE+0x18,0xD0002000);
        for(va=0;va<rb;va++) continue;
        PUT32(GPIODBASE+0x18,0xB0004000);
        for(va=0;va<rb;va++) continue;
        PUT32(GPIODBASE+0x18,0x70008000);
        for(va=0;va<rb;va++) continue;
    }
    return(0);
}
