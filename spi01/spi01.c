
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
#define RCCBASE         0x40023800
#define RCC_CR          (RCCBASE+0x00)
#define RCC_PLLCFGR     (RCCBASE+0x04)
#define RCC_CFGR        (RCCBASE+0x08)
#define RCC_AHB1ENR     (RCCBASE+0x30)
#define RCC_APB1ENR     (RCCBASE+0x40)
#define GPIODBASE       0x40020C00
#define TIM5BASE        0x40000C00
#define FLASH_ACR       0x40023C00
#define GPIOA_BASE      0x40020000
#define GPIOA_MODER     (GPIOA_BASE+0x00)
#define GPIOA_AFRL      (GPIOA_BASE+0x20)
#define GPIOA_OTYPER    (GPIOA_BASE+0x04)
#define GPIOA_BSRR      (GPIOA_BASE+0x18)
#define GPIOA_IDR       (GPIOA_BASE+0x10)
#define GPIOA_PUPDR     (GPIOA_BASE+0x0C)
#define GPIOE_BASE      0x40021000
#define GPIOE_MODER     (GPIOE_BASE+0x00)
#define GPIOE_AFRL      (GPIOE_BASE+0x20)
#define GPIOE_OTYPER    (GPIOE_BASE+0x04)
#define GPIOE_BSRR      (GPIOE_BASE+0x18)
#define USART2_BASE     0x40004400
#define USART2_SR       (USART2_BASE+0x00)
#define USART2_DR       (USART2_BASE+0x04)
#define USART2_BRR      (USART2_BASE+0x08)
#define USART2_CR1      (USART2_BASE+0x0C)
#define USART2_CR2      (USART2_BASE+0x10)
#define USART2_CR3      (USART2_BASE+0x14)
#define USART2_GTPR     (USART2_BASE+0x18)
//-------------------------------------------------------------------
#define CS_HIGH     PUT32(GPIOE_BSRR,(1<<2)<<0)
#define CS_LOW      PUT32(GPIOE_BSRR,(1<<2)<<16)
#define SCK_HIGH    PUT32(GPIOA_BSRR,(1<<5)<<0)
#define SCK_LOW     PUT32(GPIOA_BSRR,(1<<5)<<16)
#define MOSI_HIGH   PUT32(GPIOA_BSRR,(1<<7)<<0)
#define MOSI_LOW    PUT32(GPIOA_BSRR,(1<<7)<<16)
//-------------------------------------------------------------------
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

    //42000000/16 = 2625000
    //2625000/115200 = 22.786...
    //.786... * 16 = 12.58
    //12/16 = 0.75
    //13/16 = 0.8125
    //2625000/22.75 = 115384 184  .16%
    //2625000/22.8125 = 115068 132  .11%
    PUT32(USART2_BRR,(22<<4)|(13<<0));
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
    while(1)
    {
        if(GET32(TIM5BASE+0x10)&1) break;
    }
    PUT32(TIM5BASE+0x10,0);
}
//-------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    unsigned int spi_shift_out;
    unsigned int spi_shift_in;
    unsigned int spi_shift_count;
    clock_init();
    uart_init();

    ra=GET32(RCC_APB1ENR);
    ra|=1<<3; //enable TIM5
    PUT32(RCC_APB1ENR,ra);

    //timer reference clock 84MHz.
    //spi clock must be at or less than 10Mhz.
    //a divisor of 8.4 if possible would be right at 10Mhz
    //so divide by 9 is a little slower than 10Mhz
    //to sample on the mid bit cell want to be twice as fast
    //so divide by 4.2 would be ideal, divide by 5 would keep us
    //from going too fast
    PUT32(TIM5BASE+0x00,0x00000000);
//    PUT32(TIM5BASE+0x2C,5-1);
    PUT32(TIM5BASE+0x2C,50);
    PUT32(TIM5BASE+0x00,0x00000001);

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<0; //enable port A
    ra|=1<<4; //enable port E
    PUT32(RCC_AHB1ENR,ra);

    //PA5 SCK out
    //PA6 MISO in
    //PA7 MOSI out
    //PE2 CS out

    ra=GET32(GPIOA_MODER);
    ra&=~(3<<(5<<1)); //PA5
    ra|= (1<<(5<<1)); //PA5 output
    ra&=~(3<<(6<<1)); //PA6
    ra|= (0<<(6<<1)); //PA6 input
    ra&=~(3<<(7<<1)); //PA7
    ra|= (1<<(7<<1)); //PA7 output
    PUT32(GPIOA_MODER,ra);

    ra=GET32(GPIOA_OTYPER);
    ra&=~(1<<5); //PA5 push-pull
    ra&=~(1<<7); //PA7 push-pull
    PUT32(GPIOA_OTYPER,ra);

    ra=GET32(GPIOA_PUPDR);
    ra&=~(3<<(6<<1)); //PA6
    ra|= (0<<(6<<1)); //PA6 no pull up, no pull down
    //ra|= (1<<(6<<1)); //PA6 pull up
    PUT32(GPIOA_PUPDR,ra);

    ra=GET32(GPIOE_MODER);
    ra&=~(3<<(2<<1)); //PE2
    ra|= (1<<(2<<1)); //PE2 output
    PUT32(GPIOE_MODER,ra);

    ra=GET32(GPIOE_OTYPER);
    ra&=~(1<<2); //PE2 push-pull
    PUT32(GPIOE_OTYPER,ra);

    hexstring(GET32(GPIOA_MODER),1);
    hexstring(GET32(GPIOA_OTYPER),1);
    hexstring(GET32(GPIOA_PUPDR),1);
    hexstring(GET32(GPIOE_MODER),1);
    hexstring(GET32(GPIOE_OTYPER),1);

    CS_HIGH;
    SCK_HIGH;
    MOSI_HIGH;

//mosi is sampled on rising edge
//miso changes on falling edge

// a read the r/w bit is 1, single word so msbit is 0 then 6 bits of
//address. reading register/address 0xF should give a dummy/fixed pattern
//of 0x3B.


    spi_shift_out=0x8F;

    CS_HIGH;
    SCK_HIGH;
    timdelay();
    timdelay();
    CS_LOW;
    timdelay();
    timdelay();
    spi_shift_count=8;
    while(spi_shift_count--)
    {
        SCK_LOW;
        timdelay();
        if(spi_shift_out&0x80) MOSI_HIGH; else MOSI_LOW;
        timdelay();
        SCK_HIGH;
        timdelay();
        spi_shift_out<<=1;
        timdelay();
    }
    spi_shift_in=0;
    spi_shift_count=8;
    while(spi_shift_count--)
    {
        SCK_LOW;
        timdelay();
        spi_shift_in<<=1;
        if(GET32(GPIOA_IDR)&(1<<6)) spi_shift_in|=1;
        timdelay();
        SCK_HIGH;
        timdelay();
        spi_shift_in<<=1;
        if(GET32(GPIOA_IDR)&(1<<6)) spi_shift_in|=1;
        timdelay();
    }
    CS_HIGH;
    MOSI_HIGH;

    uart_string("\nHello World!\n");
    hexstring(spi_shift_in,1);

    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
