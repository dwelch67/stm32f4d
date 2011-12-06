//http://gitorious.org/~tormod/unofficial-clones/dfuse-dfu-util
//dfu-util -d 0483:df11 -c 1 -i 0 -a 0 -s 0x08000000 -D flashblinker.bin

void PUT32 ( unsigned int, unsigned int );
void PUT16 ( unsigned int, unsigned int );
void PUT8 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
unsigned int GET16 ( unsigned int );


#define RCCBASE   0x40023800
#define RCC_CR    (RCCBASE+0x00)
#define RCC_PLLCFGR (RCCBASE+0x04)
#define RCC_CFGR  (RCCBASE+0x08)
#define RCC_AHB1ENR (RCCBASE+0x30)
#define RCC_APB1ENR (RCCBASE+0x40)
#define GPIODBASE 0x40020C00
#define TIM5BASE  0x40000C00
#define FLASH_ACR  0x40023C00
#define GPIOABASE 0x40020000
#define GPIOA_MODER (GPIOABASE+0x00)
#define USART2_BASE 0x40004400
#define USART2_BRR (USART2_BASE+0x08)
#define USART2_CR1 (USART2_BASE+0x0C)
#define USART2_CR2 (USART2_BASE+0x10)
#define USART2_CR3 (USART2_BASE+0x14)
#define USART2_DR  (USART2_BASE+0x04)
#define USART2_GTPR (USART2_BASE+0x18)
#define GPIOA_AFRL (GPIOABASE+0x20)
#define GPIOA_OTYPER (GPIOABASE+0x04)

//PA2 USART2_TX available
//PA3 USART2_RX available

//PD8 USART3_TX available
//PD9 USART3_rX available

//main clock 168MHz
//PPRE1 divide by 8 = 21MHz
//21000000/9600 = 2187.5
//  2187/16 = 136.6875
// .6875*16 = 11
// divisor 136 fractional divisor 11
// 21000000/2187 = 9602

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
    PUT32(RCC_CFGR,0x0000D801); //PPRE2 /2 PPRE1 /8 sw=hse
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
    PUT32(RCC_CFGR,0x0000D802); //PPRE2 /2 PPRE1 /8 sw=pllclk
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
        if((ra-rb)>=((168000000*2)/8)) break;
    }
}


int notmain ( void )
{
    unsigned int ra;

    ClockInit();

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<0; //enable port A
    ra|=1<<3; //enable port D
    PUT32(RCC_AHB1ENR,ra);

    ra=GET32(RCC_APB1ENR);
    ra|=1<<17;  //enable USART2
    ra|=1<<3;   //enable TIM5
    PUT32(RCC_APB1ENR,ra);

    //PA2 USART2_TX

    ra=GET32(GPIOA_MODER);
    ra|= (2<<4);
    PUT32(GPIOA_MODER,ra);
    ra=GET32(GPIOA_OTYPER);
    ra&=(1<<2);
    PUT32(GPIOA_OTYPER,0);
    ra=GET32(GPIOA_AFRL);
    ra|=(7<<8);
    PUT32(GPIOA_AFRL,ra);

    // divisor 136 fractional divisor 11
    PUT32(USART2_BRR,(136<<4)|(11<<0));
    PUT32(USART2_CR1,(1<<13)|(1<<3));

    PUT16(TIM5BASE+0x00,0x0000);
    PUT16(TIM5BASE+0x00,0x0001);

    //d12 = d15 output
    ra=GET32(GPIODBASE+0x00);
    ra&=0x00FFFFFF;
    ra|=0x55000000;
    PUT32(GPIODBASE+0x00,ra);

    while(1)
    {
        PUT32(GPIODBASE+0x18,0xE0001000);
        PUT8(USART2_DR,0x55);
        timdelay();
        PUT32(GPIODBASE+0x18,0xF0000000);
        PUT8(USART2_DR,0x56);
        timdelay();
    }

    return(0);
}
