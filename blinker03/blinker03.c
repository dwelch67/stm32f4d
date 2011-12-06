//http://gitorious.org/~tormod/unofficial-clones/dfuse-dfu-util
//dfu-util -d 0483:df11 -c 1 -i 0 -a 0 -s 0x08000000 -D flashblinker.bin

void PUT32 ( unsigned int, unsigned int );
void PUT16 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
unsigned int GET16 ( unsigned int );


#define RCCBASE   0x40023800
#define RCC_CR    (RCCBASE+0x00)
#define RCC_PLLCFGR (RCCBASE+0x04)
#define RCC_CFGR  (RCCBASE+0x08)
#define GPIODBASE 0x40020C00
#define TIM5BASE  0x40000C00
#define FLASH_ACR  0x40023C00

void ClockInit ( void )
{
    unsigned int ra;

    //enable HSE
    ra=GET32(RCC_CR);
    ra&=~(0xF<<16);
    PUT32(RCC_CR,ra);
    ra|=1<<16;
    PUT32(RCC_CR,ra);
    while(1)
    {
        if(GET32(RCC_CR)&(1<<17)) break;
    }
    PUT32(RCC_CFGR,0x00009401); //PPRE2 /2 PPRE1 /4 sw=hse

    //slow flash accesses down otherwise it will crash
    PUT32(FLASH_ACR,0x00000105);

    //8MHz HSE, 168MHz pllgen 48MHz pll usb
    //Q 7 P 2 N 210 M 5 vcoin 1 pllvco 336 pllgen 168 pllusb 48
    ra=(7<<24)|(1<<22)|(((2>>1)-1)<<16)|(210<<6)|(5<<0);
    PUT32(RCC_PLLCFGR,ra);

    // enable pll
    ra=GET32(RCC_CR);
    ra|=(1<<24);
    PUT32(RCC_CR,ra);

    //wait for pll lock
    while(1)
    {
        if(GET32(RCC_CR)&(1<<25)) break;
    }

    //select pll
    PUT32(RCC_CFGR,0x00009402); //PPRE2 /2 PPRE1 /4 sw=pllclk

    //if you didnt set the flash wait states you may crash here

    //wait for it to use the pll
    while(1)
    {
        if((GET32(RCC_CFGR)&0xC)==0x8) break;
    }
 }


void timdelay ( void )
{
    unsigned int ra;
    unsigned int rb;

    rb=GET32(TIM5BASE+0x24);
    while(1)
    {
        ra=GET32(TIM5BASE+0x24);
        if((ra-rb)>=(168000000*4)) break;
    }
}


int notmain ( void )
{
    volatile unsigned int ra;
    unsigned int rx;

    ClockInit();


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
