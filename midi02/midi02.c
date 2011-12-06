
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//http://gitorious.org/~tormod/unofficial-clones/dfuse-dfu-util
//dfu-util -d 0483:df11 -c 1 -i 0 -a 0 -s 0x08000000 -D flashblinker.bin
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
#define TIM5BASE  0x40000C00
#define FLASH_ACR  0x40023C00

#define GPIOABASE 0x40020000
#define GPIOA_MODER (GPIOABASE+0x00)
#define GPIOA_AFRL (GPIOABASE+0x20)
#define GPIOA_AFRH (GPIOABASE+0x24)
#define GPIOA_OTYPER (GPIOABASE+0x04)

#define GPIODBASE 0x40020C00
#define GPIOD_MODER (GPIODBASE+0x00)
#define GPIOD_AFRL (GPIODBASE+0x20)
#define GPIOD_AFRH (GPIODBASE+0x24)
#define GPIOD_OTYPER (GPIODBASE+0x04)

#define USART2_BASE 0x40004400
#define USART2_SR  (USART2_BASE+0x00)
#define USART2_DR  (USART2_BASE+0x04)
#define USART2_BRR (USART2_BASE+0x08)
#define USART2_CR1 (USART2_BASE+0x0C)
#define USART2_CR2 (USART2_BASE+0x10)
#define USART2_CR3 (USART2_BASE+0x14)
#define USART2_GTPR (USART2_BASE+0x18)

#define USART3_BASE 0x40004800
#define USART3_SR  (USART3_BASE+0x00)
#define USART3_DR  (USART3_BASE+0x04)
#define USART3_BRR (USART3_BASE+0x08)
#define USART3_CR1 (USART3_BASE+0x0C)
#define USART3_CR2 (USART3_BASE+0x10)
#define USART3_CR3 (USART3_BASE+0x14)
#define USART3_GTPR (USART3_BASE+0x18)

//-------------------------------------------------------------------
//PA2 USART2_TX available
//PA3 USART2_RX available

//PD8 USART3_TX available
//PD9 USART3_rX available

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
    PUT32(RCC_CFGR,0x0000B401); //PPRE2 /4 PPRE1 /4 sw=hse
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
    PUT32(RCC_CFGR,0x0000B402); //PPRE2 /4 PPRE1 /4 sw=pllclk
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

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<0; //enable port A midi 31250
    ra|=1<<3; //enable port D host 38400
    PUT32(RCC_AHB1ENR,ra);

    ra=GET32(RCC_APB1ENR);
    ra|=1<<17;  //enable USART2 midi
    ra|=1<<18;  //enable USART3 host
    PUT32(RCC_APB1ENR,ra);

    //PA2 USART2_TX
    //PA3 USART2_RX
//PD8 USART3_TX available
//PD9 USART3_rX available

// 31250  31250 .00 1344
// 38400  38426 .06 1093

    ra=GET32(GPIOA_MODER);
    ra|= (2<<(2<<1));
    ra|= (2<<(3<<1));
    PUT32(GPIOA_MODER,ra);
    ra=GET32(GPIOD_MODER);
    ra|= (2<<(8<<1));
    ra|= (2<<(9<<1));
    PUT32(GPIOD_MODER,ra);

    ra=GET32(GPIOA_OTYPER);
    ra&=(1<<2);
    ra&=(1<<3);
    PUT32(GPIOA_OTYPER,ra);
    ra=GET32(GPIOD_OTYPER);
    ra&=(1<<8);
    ra&=(1<<9);
    PUT32(GPIOD_OTYPER,ra);

    ra=GET32(GPIOA_AFRL);
    ra|=(7<<(2<<2));
    ra|=(7<<(3<<2));
    PUT32(GPIOA_AFRL,ra);
    ra=GET32(GPIOD_AFRH);
    ra|=(7<<((8-8)<<2));
    ra|=(7<<((9-8)<<2));
    PUT32(GPIOD_AFRH,ra);


    //42000000/16 = 2625000
    //2625000/31250 = 84

    // divisor 84 fractional divisor 0
    PUT32(USART2_BRR,(84<<4)|(0<<0));
    PUT32(USART2_CR1,(1<<13)|(1<<3)|(1<<2));

    //42000000/16 = 2625000
    //2625000/115200 = 22.786...
    //.786... * 16 = 12.58
    //12/16 = 0.75
    //13/16 = 0.8125
    //2625000/22.75 = 115384 184  .16%
    //2625000/22.8125 = 115068 132  .11%
    PUT32(USART3_BRR,(22<<4)|(13<<0));
    PUT32(USART3_CR1,(1<<13)|(1<<3)|(1<<2));
    return(0);
}
//-------------------------------------------------------------------
void uart_putc ( unsigned int x )
{
    while ((( GET32(USART3_SR)>>6) & 3) != 3) continue;
    PUT32(USART3_DR,x);
}
//-------------------------------------------------------------------
unsigned int uart_getc ( void )
{
    while (( GET32(USART3_SR) & (1<<5)) == 0) continue;
    return(GET32(USART3_DR));
}
//-------------------------------------------------------------------
void midi_putc ( unsigned int x )
{
    while (( GET32(USART2_SR) & (1<<6)) == 0) continue;
    PUT32(USART2_DR,x);
}
//-------------------------------------------------------------------
unsigned int midi_getc ( void )
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
void uart_string ( const char *s )
{
    for(;*s;s++)
    {
        if(*s==0x0A) uart_putc(0x0D);
        uart_putc(*s);
    }
}
//-------------------------------------------------------------------
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
//-------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int timx;
    clock_init();
    uart_init();

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<3; //enable port D
    PUT32(RCC_AHB1ENR,ra);

    //d12 = d15 output
    ra=GET32(GPIOD_MODER);
    ra|= (1<<(12<<1));
    ra|= (1<<(13<<1));
    ra|= (1<<(14<<1));
    ra|= (1<<(15<<1));
    PUT32(GPIOD_MODER,ra);

    ra=GET32(RCC_APB1ENR);
    ra|=1<<3;   //enable TIM5
    PUT32(RCC_APB1ENR,ra);

    PUT16(TIM5BASE+0x00,0x0000);
    PUT16(TIM5BASE+0x00,0x0001);

    PUT32(GPIODBASE+0x18,0xE0001000);

    rb=0;
    while(1)
    {
        ra=midi_getc();
        timx=GET32(TIM5BASE+0x24);
        uart_putc(ra);
        //uart_putc((timx>>24)&0xFF);
        //uart_putc((timx>>16)&0xFF);
        uart_putc((timx>>20)&0xFF);
        uart_putc((timx>>12)&0xFF);
        rb++;
        switch(rb&1)
        {
            case 0: PUT32(GPIODBASE+0x18,0xE0001000); break;
            case 1: PUT32(GPIODBASE+0x18,0xD0002000); break;
        }
    }

    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
