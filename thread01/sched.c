
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
#define RCC_CFGR    (RCCBASE+0x08)
#define RCC_AHB1ENR (RCCBASE+0x30)
#define RCC_APB1ENR (RCCBASE+0x40)
#define GPIODBASE 0x40020C00
#define TIM5BASE  0x40000C00
#define GPIOABASE 0x40020000
#define GPIOA_MODER (GPIOABASE+0x00)
#define GPIOA_AFRL  (GPIOABASE+0x20)
#define GPIOA_OTYPER (GPIOABASE+0x04)
#define GPIOD_MODER (GPIODBASE+0x00)
#define GPIOD_AFRL  (GPIODBASE+0x20)
#define GPIOD_OTYPER (GPIODBASE+0x04)
#define USART2_BASE 0x40004400
#define USART2_SR  (USART2_BASE+0x00)
#define USART2_DR  (USART2_BASE+0x04)
#define USART2_BRR (USART2_BASE+0x08)
#define USART2_CR1 (USART2_BASE+0x0C)
#define USART2_CR2 (USART2_BASE+0x10)
#define USART2_CR3 (USART2_BASE+0x14)
#define USART2_GTPR (USART2_BASE+0x18)

#define FLASH_BASE  0x40023C00
#define FLASH_ACR   (FLASH_BASE+0x00)
#define FLASH_KEYR  (FLASH_BASE+0x04)
#define FLASH_OPTKEYR  (FLASH_BASE+0x08)
#define FLASH_SR    (FLASH_BASE+0x0C)
#define FLASH_CR    (FLASH_BASE+0x10)
#define FLASH_OPTCR (FLASH_BASE+0x14)
//-------------------------------------------------------------------
#include "common.h"
//-------------------------------------------------------------------
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
unsigned int uart_tx_if_ready ( unsigned int x )
{
    if(GET32(USART2_SR) & (1<<7))
    {
        PUT32(USART2_DR,x);
        return(1);
    }
    return(0);
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
//------------------------------------------------------------------------
unsigned int get_timer_tick ( void )
{
    return(GET32(TIM5BASE+0x24));
}
//------------------------------------------------------------------------
void change_led_state ( unsigned int led, unsigned int state )
{
    led&=3;
    led=1<<(12+led);
    if(state==0)  led<<=16;
    PUT32(GPIODBASE+0x18,led);
}
//------------------------------------------------------------------------
unsigned int next_prand ( unsigned int x )
{
    if(x&1)
    {
        x=x>>1;
        x=x^0xBF9EC099;
    }
    else
    {
        x=x>>1;
    }
    return(x);
}
//-------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;

    clock_init();
    uart_init();

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<3; //enable port D
    PUT32(RCC_AHB1ENR,ra);

    ra=GET32(RCC_APB1ENR);
    ra|=1<<3; //enable TIM5
    PUT32(RCC_APB1ENR,ra);

    //d12 = d15 output
    ra=GET32(GPIOD_MODER);
    ra&=0x00FFFFFF;
    ra|=0x55000000;
    PUT32(GPIOD_MODER,ra);
    //push pull
    ra=GET32(GPIOD_OTYPER);
    ra&=0xFFFF0FFF;
    PUT32(GPIOD_OTYPER,ra);
    PUT32(GPIODBASE+0x18,0xF0000000);

    PUT32(TIM5BASE+0x00,0x00000000);
    PUT32(TIM5BASE+0x2C,0xFFFFFFFF);
    PUT32(TIM5BASE+0x00,0x00000001);

    hexstring(0x12345678,1);

    threada(INIT);
    threadb(INIT);
    threadc(INIT);
    while(1)
    {
        threada(HEARTBEAT);
        threadb(HEARTBEAT);
        threadc(HEARTBEAT);
    }

    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
