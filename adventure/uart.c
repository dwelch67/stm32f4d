
//-------------------------------------------------------------------
//-------------------------------------------------------------------
void PUT32 ( unsigned int, unsigned int );
void PUT16 ( unsigned int, unsigned int );
void PUT8 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
unsigned int GET16 ( unsigned int );
//-------------------------------------------------------------------
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
#define USART2_SR  (USART2_BASE+0x00)
#define USART2_DR  (USART2_BASE+0x04)
#define USART2_BRR (USART2_BASE+0x08)
#define USART2_CR1 (USART2_BASE+0x0C)
#define USART2_CR2 (USART2_BASE+0x10)
#define USART2_CR3 (USART2_BASE+0x14)
#define USART2_GTPR (USART2_BASE+0x18)
#define GPIOA_AFRL (GPIOABASE+0x20)
#define GPIOA_OTYPER (GPIOABASE+0x04)
//-------------------------------------------------------------------
void clock_init ( void )
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
//-------------------------------------------------------------------
int uart_init ( void )
{
    unsigned int ra;

    clock_init();

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<0; //enable port A
    PUT32(RCC_AHB1ENR,ra);

    ra=GET32(RCC_APB1ENR);
    ra|=1<<17;  //enable USART2
    PUT32(RCC_APB1ENR,ra);

    //PA2 USART2_TX
    //PA3 USART2_RX

    ra=GET32(GPIOA_MODER);
    ra|= (2<<4);
    ra|= (2<<6);
    PUT32(GPIOA_MODER,ra);
    ra=GET32(GPIOA_OTYPER);
    ra&=(1<<2);
    ra&=(1<<3);
    PUT32(GPIOA_OTYPER,ra);
    ra=GET32(GPIOA_AFRL);
    ra|=(7<<8);
    ra|=(7<<12);
    PUT32(GPIOA_AFRL,ra);

    // divisor 136 fractional divisor 11
    PUT32(USART2_BRR,(136<<4)|(11<<0));
    PUT32(USART2_CR1,(1<<13)|(1<<3)|(1<<2));
    return(0);
}
//-------------------------------------------------------------------
void uart_putc ( unsigned int x )
{
    while (( GET32(USART2_SR) & (1<<7)) == 0) continue;
    PUT32(USART2_DR,x);
}
//-------------------------------------------------------------------
unsigned int uart_getc ( void )
{
    while (( GET32(USART2_SR) & (1<<5)) == 0) continue;
    return(GET32(USART2_DR));
}
//-------------------------------------------------------------------
void hexstring ( unsigned int d, unsigned int cr )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_putc(rc);
        if(rb==0) break;
    }
    if(cr)
    {
        uart_putc(0x0D);
        uart_putc(0x0A);
    }
    else
    {
        uart_putc(0x20);
    }
}




//-------------------------------------------------------------------
//-------------------------------------------------------------------
