
#include "common.h"

static unsigned int tb_tick_last;
static unsigned int tb_send_char;
static unsigned int tb_char_to_send;
static unsigned int tb_led_state;

unsigned int threadb ( unsigned int event )
{
    unsigned int nowtick;

    switch(event)
    {
        case INIT:
        {
            tb_send_char=1;
            tb_char_to_send=0x32;
            tb_tick_last=get_timer_tick();
            tb_led_state=0;
            change_led_state(1,tb_led_state);
            return(0);
        }
        case HEARTBEAT:
        {
            if(tb_send_char)
            {
                if(uart_tx_if_ready(tb_char_to_send)) tb_send_char=0;
            }
            nowtick=get_timer_tick();
            if((nowtick-tb_tick_last)>=5000000)
            {
                tb_tick_last+=5000000;
                tb_send_char=1;
                //tb_char_to_send=
                tb_led_state=(tb_led_state+1)&1;
                change_led_state(1,tb_led_state);
            }
            return(0);
        }
        default:
        {
            break;
        }
    }
    return(0);
}
