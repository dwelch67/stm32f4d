
//-------------------------------------------------------------------
//-------------------------------------------------------------------
#define VALIDATE_FLASH
//#define FLASH_FASTER
//-------------------------------------------------------------------
void PUT32 ( unsigned int, unsigned int );
void PUT16 ( unsigned int, unsigned int );
void PUT8 ( unsigned int, unsigned int );
void PUT64X ( unsigned int, unsigned int, unsigned int );
void PUT64M ( unsigned int, unsigned int, unsigned int );
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

#define FLASH_BASE  0x40023C00
#define FLASH_ACR   (FLASH_BASE+0x00)
#define FLASH_KEYR  (FLASH_BASE+0x04)
#define FLASH_OPTKEYR  (FLASH_BASE+0x08)
#define FLASH_SR    (FLASH_BASE+0x0C)
#define FLASH_CR    (FLASH_BASE+0x10)
#define FLASH_OPTCR (FLASH_BASE+0x14)
//-------------------------------------------------------------------
//extern const unsigned int rom_size;
//extern const unsigned int rom_data[];
#include "flashme-bin.c"
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
int wait_on_busy_bit ( void )
{
    unsigned int ra;

    while(1)
    {
        ra=GET32(FLASH_SR);
        if(ra&0x000000F2)
        {
            hexstring(0xBADBAD,0); hexstring(ra,1);
            return(1);
        }
        if(ra&0x00010000) continue;
        break;
    }
    return(0);
}
//-------------------------------------------------------------------
int erase_sector ( unsigned int addr )
{
    unsigned int sectnum;
    //might not be a sector, they are not uniform for this part
    switch(addr)
    {
        case 0x08000000: sectnum=0; break;
        case 0x08004000: sectnum=1; break;
        case 0x08008000: sectnum=2; break;
        case 0x0800C000: sectnum=3; break;
        case 0x08010000: sectnum=4; break;
        //this code is limited to 0x20000 bytes for data and flash
        //program cannot get to 0x08020000
        default: return(0); //this is not an error, code designed for this
    }
    PUT32(FLASH_CR,0x00000002|(sectnum<<3));
    PUT32(FLASH_CR,0x00010002|(sectnum<<3));
    if(wait_on_busy_bit()) return(1);
    //erase_sector actually needs to be called before writing to any
    //flash so that it can be left in program mode:
    //why doesnt x64 mode work?  setting for x32
    PUT32(FLASH_CR,0x00000200);
    PUT32(FLASH_CR,0x00000201);
    return(0);
}
//-------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int prand;
    unsigned int errors;
    volatile unsigned int beg,end;

    clock_init();
    uart_init();

    ra=GET32(RCC_APB1ENR);
    ra|=1<<3; //enable TIM5
    PUT32(RCC_APB1ENR,ra);
    PUT32(TIM5BASE+0x00,0x00000000);
    PUT32(TIM5BASE+0x2C,0xFFFFFFFF);
    PUT32(TIM5BASE+0x00,0x00000001);

    uart_string("\nHello World!\n");
    for(ra=0x00;ra<0x20;ra+=4) hexstring(GET32(0x08000000+ra),1);

    //wait for no busy bit before touching FLASH_CR
    hexstring(GET32(FLASH_SR),1);
    wait_on_busy_bit();
    ra=GET32(FLASH_CR);
    hexstring(ra,1);
    if(ra&0x80000000)
    {
        //if locked, unlock
        PUT32(FLASH_KEYR,0x45670123);
        PUT32(FLASH_KEYR,0xCDEF89AB);
    }
    hexstring(GET32(FLASH_CR),1);

    beg=GET32(TIM5BASE+0x24);
    //choose start value so that erase_sector() is called on first pass
    if(rom_size==0)
    {
        prand=0xAABBCCDD;
        for(ra=0;ra<0x10000;ra+=4)
        {
            rb=0x08000000|ra;
            if((rb&0xFFF)==0) erase_sector(rb);
            if(ra==0) continue; //dont crash it with something that resembles a real instruction
            prand=next_prand(prand);
            PUT32(rb,prand);
#ifndef FLASH_FASTER
            if(wait_on_busy_bit()) return(1);
#endif
        }
#ifdef FLASH_FASTER
        if(wait_on_busy_bit()) return(1);
#endif
    }
    else
    {
        for(ra=0;ra<rom_size;ra++)
        {
            rb=0x08000000|(ra<<2);
            if((rb&0xFFF)==0) erase_sector(rb);
            if(rom_data[ra]==0xFFFFFFFF) continue;
            PUT32(rb,rom_data[ra]);
#ifndef FLASH_FASTER
            if(wait_on_busy_bit()) return(1);
#endif
        }
#ifdef FLASH_FASTER
        if(wait_on_busy_bit()) return(1);
#endif
    }
    //lock flash
    PUT32(FLASH_CR,0x80000000);
    end=GET32(TIM5BASE+0x24);
    hexstring(0x11111111,1);
    hexstring(beg,1);
    hexstring(end,1);
    hexstring(end-beg,1);
    hexstring(0x22222222,1);
    for(ra=0x00;ra<0x20;ra+=4) hexstring(GET32(0x08000000+ra),1);
#ifdef VALIDATE_FLASH
    if(rom_size==0)
    {
        prand=0xAABBCCDD;
        errors=0;
        for(ra=0;ra<0x10000;ra+=4)
        {
            rb=0x08000000|ra;
            if(ra==0) continue;
            prand=next_prand(prand);
            rc=GET32(rb);
            if(rc!=prand)
            {
                errors++;
                if(errors<20)
                {
                    hexstring(rb,0); hexstring(rc,0); hexstring(prand,1);
                }
            }
        }
    }
    else
    {
        errors=0;
        for(ra=0;ra<rom_size;ra++)
        {
            rb=0x08000000|(ra<<2);
            rc=GET32(rb);
            if(rc!=rom_data[ra])
            {
                errors++;
                if(errors<20)
                {
                    hexstring(rb,0); hexstring(rc,0); hexstring(rom_data[ra],1);
                }
            }
        }
    }
    hexstring(errors,1);
    if(errors) hexstring(0xBADBAD,1);

#endif
    hexstring(0x12345678,1);
    uart_string("\nDone.\n");
    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
