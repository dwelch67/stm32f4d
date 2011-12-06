
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
void hexstring ( unsigned int d, unsigned int cr );

#include "testdata.h"

unsigned int edata[TESTDATALEN];

/* take 64 bits of data in v[0] and v[1] and 128 bits of key[0] - key[3] */

//-------------------------------------------------------------------
void encipher(unsigned int num_rounds, unsigned int v[2], unsigned int const key[4]) {
    unsigned int i;
    unsigned int v0=v[0], v1=v[1], sum=0, delta=0x9E3779B9;
    for (i=0; i < num_rounds; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
    }
    v[0]=v0; v[1]=v1;
}

//-------------------------------------------------------------------
void decipher(unsigned int num_rounds, unsigned int v[2], unsigned int const key[4]) {
    unsigned int i;
    unsigned int v0=v[0], v1=v[1], delta=0x9E3779B9, sum=delta*num_rounds;
    for (i=0; i < num_rounds; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
    }
    v[0]=v0; v[1]=v1;
}
//-------------------------------------------------------------------
int run_tea_test ( void )
{
    unsigned int ra;
    unsigned int errors;
    unsigned int data[2];
    unsigned int key[4];

    errors=0;

    key[0]=0x11111111;
    key[1]=0x22222222;
    key[2]=0x33333333;
    key[3]=0x44444444;

    if((TESTDATALEN&1))
    {
        hexstring(0xBADBAD00,1);
        return(1);
    }

    for(ra=0;ra<TESTDATALEN;ra+=2)
    {
        data[0]=testdata[ra+0];
        data[1]=testdata[ra+1];
        encipher(32,data,key);
        edata[ra+0]=data[0];
        edata[ra+1]=data[1];
    }

    for(ra=0;ra<TESTDATALEN;ra+=2)
    {
        data[0]=edata[ra+0];
        data[1]=edata[ra+1];
        decipher(32,data,key);
        if(data[0]!=testdata[ra+0])
        {
            errors++;
            hexstring(ra,0); hexstring(data[0],0); hexstring(testdata[ra+0],1);
        }
        if(data[1]!=testdata[ra+1])
        {
            errors++;
            hexstring(ra,0); hexstring(data[1],0); hexstring(testdata[ra+1],1);
        }
        if(errors>20) break;
    }
    if(errors)
    {
        hexstring(errors,1);
        hexstring(0xBADBAD99,1);
        return(1);
    }
    return(0);
}


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
    PUT32(FLASH_ACR,0x00000005);
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

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<0; //enable port A
    PUT32(RCC_AHB1ENR,ra);

    ra=GET32(RCC_APB1ENR);
    ra|=1<<17;  //enable USART2
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
    return(0);
}
//-------------------------------------------------------------------
void uart_putc ( unsigned int x )
{
    while (( GET32(USART2_SR) & (1<<7)) == 0) continue;
    PUT32(USART2_DR,x);
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

//-------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    unsigned int beg,end;

    clock_init();
    uart_init();

    ra=GET32(RCC_APB1ENR);
    ra|=1<<3;   //enable TIM5
    PUT32(RCC_APB1ENR,ra);
    PUT16(TIM5BASE+0x00,0x0000);
    PUT16(TIM5BASE+0x00,0x0001);

    uart_string("\nHello World!\n");


    //no caches or prefetch
    ra=GET32(FLASH_ACR);
    ra&=~(0x1F00);
    PUT32(FLASH_ACR,ra);

    beg=GET32(TIM5BASE+0x24);
    run_tea_test();
    end=GET32(TIM5BASE+0x24);
    end-=beg;
    hexstring(end,1);

    beg=GET32(TIM5BASE+0x24);
    run_tea_test();
    end=GET32(TIM5BASE+0x24);
    end-=beg;
    hexstring(end,1);

    //prefetch only
    ra=GET32(FLASH_ACR);
    ra&=~(0x1F00);
    PUT32(FLASH_ACR,ra|0x100);

    beg=GET32(TIM5BASE+0x24);
    run_tea_test();
    end=GET32(TIM5BASE+0x24);
    end-=beg;
    hexstring(end,1);

    beg=GET32(TIM5BASE+0x24);
    run_tea_test();
    end=GET32(TIM5BASE+0x24);
    end-=beg;
    hexstring(end,1);

    //Try instruction cache only
    ra=GET32(FLASH_ACR);
    ra&=~(0x1F00);
    PUT32(FLASH_ACR,ra|0x200);

    beg=GET32(TIM5BASE+0x24);
    run_tea_test();
    end=GET32(TIM5BASE+0x24);
    end-=beg;
    hexstring(end,1);

    beg=GET32(TIM5BASE+0x24);
    run_tea_test();
    end=GET32(TIM5BASE+0x24);
    end-=beg;
    hexstring(end,1);

    //Try data cache only
    ra=GET32(FLASH_ACR);
    ra&=~(0x1F00);
    PUT32(FLASH_ACR,ra|0x400);

    beg=GET32(TIM5BASE+0x24);
    run_tea_test();
    end=GET32(TIM5BASE+0x24);
    end-=beg;
    hexstring(end,1);

    beg=GET32(TIM5BASE+0x24);
    run_tea_test();
    end=GET32(TIM5BASE+0x24);
    end-=beg;
    hexstring(end,1);

    //Try both instruction and data cache
    //not sure if this is how you invalidate and/or reset the caches
    ra=GET32(FLASH_ACR);
    ra&=~(0x1F00);
    PUT32(FLASH_ACR,ra);
    PUT32(FLASH_ACR,ra|0x1800);
    PUT32(FLASH_ACR,ra);
    PUT32(FLASH_ACR,ra|0x600);

    beg=GET32(TIM5BASE+0x24);
    run_tea_test();
    end=GET32(TIM5BASE+0x24);
    end-=beg;
    hexstring(end,1);

    beg=GET32(TIM5BASE+0x24);
    run_tea_test();
    end=GET32(TIM5BASE+0x24);
    end-=beg;
    hexstring(end,1);

    //instruction cache, data cache and prefetch
    ra=GET32(FLASH_ACR);
    ra&=~(0x1F00);
    PUT32(FLASH_ACR,ra);
    PUT32(FLASH_ACR,ra|0x1800);
    PUT32(FLASH_ACR,ra);
    PUT32(FLASH_ACR,ra|0x700);

    beg=GET32(TIM5BASE+0x24);
    run_tea_test();
    end=GET32(TIM5BASE+0x24);
    end-=beg;
    hexstring(end,1);

    beg=GET32(TIM5BASE+0x24);
    run_tea_test();
    end=GET32(TIM5BASE+0x24);
    end-=beg;
    hexstring(end,1);


    hexstring(0x87654321,1);

    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
