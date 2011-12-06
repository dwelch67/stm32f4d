
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
#define CS_HIGH     PUT32(GPIOE_BSRR,(1<<7)<<0)
#define CS_LOW      PUT32(GPIOE_BSRR,(1<<7)<<16)
#define RES_HIGH    PUT32(GPIOE_BSRR,(1<<9)<<0)
#define RES_LOW     PUT32(GPIOE_BSRR,(1<<9)<<16)
#define SDAT_HIGH   PUT32(GPIOE_BSRR,(1<<11)<<0)
#define SDAT_LOW    PUT32(GPIOE_BSRR,(1<<11)<<16)
#define SCLK_HIGH   PUT32(GPIOE_BSRR,(1<<13)<<0)
#define SCLK_LOW    PUT32(GPIOE_BSRR,(1<<13)<<16)
#define DC_HIGH     PUT32(GPIOE_BSRR,(1<<15)<<0)
#define DC_LOW      PUT32(GPIOE_BSRR,(1<<15)<<16)
#define DC_COMMAND DC_LOW
#define DC_DATA DC_HIGH
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
void nhdata ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    //assume SCLK_HIGH;
    DC_DATA;
    for(rb=0;rb<20;rb++)
    {
        timdelay();
        timdelay();
        CS_LOW;
        timdelay();
        timdelay();
        rc=rb;
        for(ra=0;ra<8;ra++)
        {
            SCLK_LOW;
            timdelay();
            if(rc&0x80) SDAT_HIGH; else SDAT_LOW;
            timdelay();
            SCLK_HIGH;
            timdelay();
            rc<<=1;
            timdelay();
        }
        timdelay();
        timdelay();
        CS_HIGH;
    }
//    DC_DATA;
}
//-------------------------------------------------------------------
void nhwrite ( unsigned int cmd, unsigned char *data, unsigned int len )
{
    unsigned int ra;
    unsigned int rb;

    //assume SCLK_HIGH;
    CS_LOW;
    DC_COMMAND;
    timdelay();
    timdelay();
    for(ra=0;ra<8;ra++)
    {
        SCLK_LOW;
        timdelay();
        if(cmd&0x80) SDAT_HIGH; else SDAT_LOW;
        timdelay();
        SCLK_HIGH;
        timdelay();
        cmd<<=1;
        timdelay();
    }
    timdelay();
    timdelay();
    CS_HIGH;
//    DC_DATA;
    for(rb=0;rb<len;rb++)
    {
        timdelay();
        timdelay();
        CS_LOW;
        timdelay();
        timdelay();
        for(ra=0;ra<8;ra++)
        {
            SCLK_LOW;
            timdelay();
            if(data[rb]&0x80) SDAT_HIGH; else SDAT_LOW;
            timdelay();
            SCLK_HIGH;
            timdelay();
            data[rb]<<=1;
            timdelay();
        }
        timdelay();
        timdelay();
        CS_HIGH;
    }
}
//-------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    unsigned char data[16];

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
    ra|=1<<4; //enable port E
    PUT32(RCC_AHB1ENR,ra);

    //PE7 /CS
    //PE9 /RESET
    //PE11 SDATA
    //PE13 SCLOCK
    //PE15 D/C

    ra=GET32(GPIOE_MODER);
    ra&=~(3<<( 7<<1)); //PE7
    ra|= (1<<( 7<<1)); //PE7 output
    ra&=~(3<<( 9<<1)); //PE9
    ra|= (1<<( 9<<1)); //PE9 output
    ra&=~(3<<(11<<1)); //PE11
    ra|= (1<<(11<<1)); //PE11 output
    ra&=~(3<<(13<<1)); //PE13
    ra|= (1<<(13<<1)); //PE13 output
    ra&=~(3<<(15<<1)); //PE15
    ra|= (1<<(15<<1)); //PE15 output
    PUT32(GPIOE_MODER,ra);

    ra=GET32(GPIOE_OTYPER);
    ra&=~(1<<7); //PE7 push-pull
    ra&=~(1<<9); //PE9 push-pull
    ra&=~(1<<11); //PE11 push-pull
    ra&=~(1<<13); //PE13 push-pull
    ra&=~(1<<15); //PE15 push-pull
    PUT32(GPIOE_OTYPER,ra);

    hexstring(GET32(GPIOE_MODER),1);
    hexstring(GET32(GPIOE_OTYPER),1);

    RES_LOW; //put in reset
    CS_HIGH;
    SDAT_LOW;
    SCLK_HIGH;
    DC_HIGH;

    for(ra=0;ra<10;ra++) timdelay();
    RES_HIGH;
    for(ra=0;ra<100;ra++) timdelay();


    uart_string("\nHello World!\n");

////Set_Display_On_Off_12864(0x00); Display Off (0x00/0x01)
    //nhwrite(0xAE,data,0);
////Set_Display_Clock_12864(0x91); Set Clock as 135 Frames/Sec
    //data[0]=0x91; data[1]=0x91;
    //nhwrite(0xB3,data,2);
////Set_Multiplex_Ratio_12864(0x3F); 1/64 Duty (0x0F~0x5F)
    //data[0]=0x3F;
    //nhwrite(0xA8,data,1);
////Set_Display_Offset_12864(0x4C); Shift Mapping RAM Counter (0x00~0x5F)
    //data[0]=0x4C;
    //nhwrite(0xA2,data,1);
////Set_Start_Line_12864(0x00); Set Mapping RAM Display Start Line (0x00~0x5F)
    //data[0]=0x00;
    //nhwrite(0xA1,data,1);
////Set_Master_Config_12864(0x00); Disable Embedded DC/DC Converter (0x00/0x01)
    //data[0]=0x00;
    //nhwrite(0xAD,data,1);
////Set_Remap_Format_12864(0x50); Set Column Address 0 Mapped to SEG0
    //data[0]=0x00;
    //nhwrite(0xA0,data,1);
////     Disable Nibble Remap
////     Horizontal Address Increment
////     Scan from COM[N‐1] to COM0
////     Enable COM Split Odd Even
////Set_Current_Range_12864(0x02); Set Full Current Range
    //nhwrite(0x86,data,0);
////Set_Gray_Scale_Table_12864(); Set Pulse Width for Gray Scale Table
    //for(ra=0;ra<15;ra++) data[ra]=1;
    //nhwrite(0xB8,data,15);
////Set_Contrast_Current_12864(brightness); Set Scale Factor of Segment Output Current Control
    //data[0]=0x40;
    //nhwrite(0x81,data,1);
////Set_Frame_Frequency_12864(0x51); Set Frame Frequency
    //data[0]=0x51;
    //nhwrite(0xB2,data,1);
////Set_Phase_Length_12864(0x55); Set Phase 1 as 5 Clocks & Phase 2 as 5 Clocks
    //data[0]=0x55; data[1]=0x55;
    //nhwrite(0xB1,data,2);
////Set_Precharge_Voltage_12864(0x10); Set Pre‐Charge Voltage Level
    //data[0]=0x10;
    //nhwrite(0xBC,data,1);
////Set_Precharge_Compensation_12864(0x20,0x02); Set Pre‐Charge Compensation
    //data[0]=0x02;
    //nhwrite(0xB4,data,1);
////Set_VCOMH_12864(0x1C); Set High Voltage Level of COM Pin
    //data[0]=0x1C;
    //nhwrite(0xB3,data,1);
////Set_VSL_12864(0x0D); Set Low Voltage Level of SEG Pin
    //data[0]=0x0D;
    //nhwrite(0xBF,data,1);
////Set_Display_Mode_12864(0x00); Normal Display Mode (0x00/0x01/0x02/0x03)
    //nhwrite(0xA4,data,0);
////Fill_RAM_12864(0x00); Clear Screen


////Set_Display_On_Off_12864(0x01); Display On (0x00/0x01)
    //nhwrite(0xAF,data,0);


   nhwrite(0xAE,data,0);
   nhdata();
   nhwrite(0xAF,data,0);


    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
