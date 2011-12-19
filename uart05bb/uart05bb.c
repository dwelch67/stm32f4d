
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
#define GPIODBASE 0x40020C00
#define TIM5BASE  0x40000C00
#define FLASH_ACR  0x40023C00

#define GPIOABASE 0x40020000
#define GPIOA_MODER (GPIOABASE+0x00)
#define GPIOA_AFRL (GPIOABASE+0x20)
#define GPIOA_OTYPER (GPIOABASE+0x04)

#define GPIOE_BASE      0x40021000
#define GPIOE_MODER     (GPIOE_BASE+0x00)
#define GPIOE_AFRL      (GPIOE_BASE+0x20)
#define GPIOE_OTYPER    (GPIOE_BASE+0x04)
#define GPIOE_BSRR      (GPIOE_BASE+0x18)
#define GPIOE_IDR       (GPIOE_BASE+0x10)
#define GPIOE_PUPDR     (GPIOE_BASE+0x0C)


#define USART2_BASE 0x40004400
#define USART2_SR  (USART2_BASE+0x00)
#define USART2_DR  (USART2_BASE+0x04)
#define USART2_BRR (USART2_BASE+0x08)
#define USART2_CR1 (USART2_BASE+0x0C)
#define USART2_CR2 (USART2_BASE+0x10)
#define USART2_CR3 (USART2_BASE+0x14)
#define USART2_GTPR (USART2_BASE+0x18)
//-------------------------------------------------------------------
#define TX_HIGH     PUT32(GPIOE_BSRR,(1<<9)<<0)
#define TX_LOW      PUT32(GPIOE_BSRR,(1<<9)<<16)
//-------------------------------------------------------------------
//PE9  USART2_TX
//PE11 USART2_RX
//-------------------------------------------------------------------
void clock_init  ( void )
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

    //slow flash accesses down otherwise it will crash
    PUT32(FLASH_ACR,0x00000100);

    //8MHz HSE, 168MHz pllgen 48MHz pll usb
    //Q 7 P 2 N 210 M 5 vcoin 1 pllvco 336 pllgen 168 pllusb 48
    //ra=(7<<24)|(1<<22)|(((2>>1)-1)<<16)|(210<<6)|(5<<0);
    //Q 5 P 8 N 150 M 5 vcoin 1 pllvco 240 pllgen 30 pllusb 48
    ra=(5<<24)|(1<<22)|(((8>>1)-1)<<16)|(150<<6)|(5<<0);
    //Q 5 P 8 N 240 M 8 vcoin 1 pllvco 240 pllgen 30 pllusb 48
    //ra=(5<<24)|(1<<22)|(((8>>1)-1)<<16)|(240<<6)|(8<<0);
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
    PUT32(RCC_CFGR,0x00000002); // sw=pllclk

    //if you didnt set the flash wait states you may crash here

    //wait for it to use the pll
    while(1)
    {
        if((GET32(RCC_CFGR)&0xC)==0x8) break;
    }
    PUT32(FLASH_ACR,0x00000700);
}
//30000000 / 115200 = 260.4
#define TIMER_VALUE 260
#define HALF_VALUE 130
//-------------------------------------------------------------------
void uart_putc ( unsigned int c )
{
    unsigned int sa;
    unsigned int sb;
    unsigned int then,now;

    sa=c<<1;
    sa|=1<<9;
    sb=10;
    then=GET32(TIM5BASE+0x24);
    while(sb--)
    {
        if(sa&1) TX_HIGH; else TX_LOW;
        sa>>=1;
        while(1)
        {
            now=GET32(TIM5BASE+0x24);
            if((now-then)>=TIMER_VALUE) break;
        }
        then+=TIMER_VALUE;
    }
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
int notmain ( void )
{
    unsigned int ra;
    unsigned int sb;
    unsigned int then,now;
    unsigned int bitin;

    clock_init();

    ra=GET32(RCC_APB1ENR);
    ra|=1<<3; //enable TIM5
    PUT32(RCC_APB1ENR,ra);

    PUT32(TIM5BASE+0x00,0x00000000);
    PUT32(TIM5BASE+0x00,0x00000001);

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<4; //enable port E
    PUT32(RCC_AHB1ENR,ra);


    ra=GET32(GPIOE_MODER);
    ra&=~(3<<(9<<1)); //PE9
    ra|= (1<<(9<<1)); //PE9 output
    ra&=~(3<<(11<<1)); //PE11 input
    PUT32(GPIOE_MODER,ra);

    ra=GET32(GPIOE_OTYPER);
    ra&=~(1<<9); //PE9 push-pull
    PUT32(GPIOE_OTYPER,ra);

    ra=GET32(GPIOE_PUPDR);
    ra&=~(3<<(11<<1)); //PE11 no pull up or pull down
    PUT32(GPIOE_PUPDR,ra);

    uart_string("\nHello World!\n");

    hexstring(0x12345678,1);

    while(1)
    {
        bitin=0;
        while(1) if((GET32(GPIOE_IDR)&(1<<11))==0) break;
        then=GET32(TIM5BASE+0x24);
        for(sb=0;sb<10;sb++)
        {
            while(1)
            {
                now=GET32(TIM5BASE+0x24);
                if((now-then)>=HALF_VALUE) break;
            }
            if(GET32(GPIOE_IDR)&(1<<11))
            {
                bitin>>=1;
                bitin|=1<<9;
            }
            else
            {
                bitin>>=1;
            }
            then+=HALF_VALUE;
            while(1)
            {
                now=GET32(TIM5BASE+0x24);
                if((now-then)>=HALF_VALUE) break;
            }
            then+=HALF_VALUE;
        }
        //hexstring(bitin,0);  hexstring(bitin>>1,1);
        if((bitin&0x201)==0x200)
        {
            uart_putc((bitin>>1)&0xFF);
        }
    }
    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
