
#include "common.h"

static unsigned int ta_tick_last;
static unsigned int ta_send_char;
static unsigned int ta_char_to_send;
static unsigned int ta_led_state;

unsigned int threada ( unsigned int event )
{
    unsigned int nowtick;

    switch(event)
    {
        case INIT:
        {
            ta_send_char=1;
            ta_char_to_send=0x31;
            ta_tick_last=get_timer_tick();
            ta_led_state=0;
            change_led_state(0,ta_led_state);
            return(0);
        }
        case HEARTBEAT:
        {
            if(ta_send_char)
            {
                if(uart_tx_if_ready(ta_char_to_send)) ta_send_char=0;
            }
            nowtick=get_timer_tick();
            if((nowtick-ta_tick_last)>=10000000)
            {
                ta_tick_last+=10000000;
                ta_send_char=1;
                //ta_char_to_send=
                ta_led_state=(ta_led_state+1)&1;
                change_led_state(0,ta_led_state);
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
