
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
unsigned int ASM_DELAY ( unsigned int );
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
#define CS_HIGH     PUT32(GPIOE_BSRR,(1<<3)<<0)
#define CS_LOW      PUT32(GPIOE_BSRR,(1<<3)<<16)
#define SCK_HIGH    PUT32(GPIOA_BSRR,(1<<5)<<0)
#define SCK_LOW     PUT32(GPIOA_BSRR,(1<<5)<<16)
#define MOSI_HIGH   PUT32(GPIOA_BSRR,(1<<7)<<0)
#define MOSI_LOW    PUT32(GPIOA_BSRR,(1<<7)<<16)
//-------------------------------------------------------------------
//PA2 USART2_TX available
//PA3 USART2_RX available

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
    PUT32(RCC_CFGR,0x00000001);
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

    //8000000/16 = 500000
    //500000/9600 = 52.08333
    PUT32(USART2_BRR,(52<<4)|(1<<0));
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
    //while(1)
    //{
        //if(GET32(TIM5BASE+0x10)&1) break;
    //}
    //PUT32(TIM5BASE+0x10,0);
    ASM_DELAY(20);
}
//-------------------------------------------------------------------
unsigned int spi_read_register ( unsigned int reg )
{
    unsigned int spi_shift_out;
    unsigned int spi_shift_in;
    unsigned int spi_shift_count;

    spi_shift_out=0x80|reg;

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
        timdelay();
    }
    CS_HIGH;
    return(spi_shift_in);
}
//-------------------------------------------------------------------

int notmain ( void )
{
    unsigned int ra;
    unsigned int spi_shift_out;
    unsigned int spi_shift_in;
    unsigned int spi_shift_count;

ASM_DELAY(10000);
    clock_init();
    uart_init();

    //ra=GET32(RCC_APB1ENR);
    //ra|=1<<3; //enable TIM5
    //PUT32(RCC_APB1ENR,ra);

    //PUT32(TIM5BASE+0x00,0x00000000);
    //PUT32(TIM5BASE+0x2C,50);
    //PUT32(TIM5BASE+0x00,0x00000001);

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<0; //enable port A
    ra|=1<<4; //enable port E
    PUT32(RCC_AHB1ENR,ra);

    //PA5 SCK out
    //PA6 MISO in
    //PA7 MOSI out
    //PE3 CS out

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
    ra|= (2<<(6<<1)); //PA6 pull down
    //ra|= (0<<(6<<1)); //PA6
    PUT32(GPIOA_PUPDR,ra);

    ra=GET32(GPIOE_MODER);
    ra&=~(3<<(3<<1)); //PE3
    ra|= (1<<(3<<1)); //PE3 output
    PUT32(GPIOE_MODER,ra);

    ra=GET32(GPIOE_OTYPER);
    ra&=~(1<<3); //PE3 push-pull
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

    hexstring(0x1234,1);

    spi_shift_out=0x2047;

    CS_LOW;
    timdelay();
    timdelay();
    spi_shift_count=16;
    while(spi_shift_count--)
    {
        SCK_LOW;
        timdelay();
        if(spi_shift_out&0x8000) MOSI_HIGH; else MOSI_LOW;
        timdelay();
        SCK_HIGH;
        timdelay();
        spi_shift_out<<=1;
        timdelay();
    }
    CS_HIGH;

    hexstring(0x1234,1);


    ASM_DELAY(250000);


    spi_shift_out=0x8F;

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
        timdelay();
    }
    CS_HIGH;

    hexstring(0x1234,1);
    hexstring(spi_shift_in,1);



    spi_shift_out=0xA0;

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
        timdelay();
    }
    CS_HIGH;

    hexstring(0x1234,1);
    hexstring(spi_shift_in,1);

    hexstring(spi_read_register(0x23),1);


    while(1)
    {
        hexstring(spi_read_register(0x27),0);
        hexstring(spi_read_register(0x0F),0);
        hexstring(spi_read_register(0x29),0);
        hexstring(spi_read_register(0x2B),0);
        hexstring(spi_read_register(0x2D),1);
    }
    //uart_string("\nHello World!\n");
    //hexstring(spi_shift_in,1);

    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
