
#include "common.h"

#define MY_LED 2
#define MY_CHAR 0x32

unsigned int threadc ( unsigned int event )
{
static unsigned int send_char;
static unsigned int char_to_send;
static unsigned int led_state;
static unsigned int timer_count;

    switch(event)
    {
        case INIT:
        {
            send_char=1;
            char_to_send=MY_CHAR;
            led_state=0;
            change_led_state(MY_LED,led_state);
            return(0);
        }
        case LOOP:
        {
            if(send_char)
            {
                if(uart_tx_if_ready(char_to_send)) send_char=0;
            }
            return(0);
        }
        case TIMER:
        {
            send_char=1;
            char_to_send=MY_CHAR;
            timer_count=4;
            return(0);
        }
        case HEARTBEAT:
        {
            if(timer_count)
            {
                timer_count--;
                led_state=(led_state+1)&1;
                change_led_state(MY_LED,led_state);
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
