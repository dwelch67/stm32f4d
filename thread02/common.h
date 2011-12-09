
extern unsigned int threada ( unsigned int event );
extern unsigned int threadb ( unsigned int event );
extern unsigned int threadc ( unsigned int event );
extern unsigned int get_timer_tick ( void );
extern unsigned int uart_tx_if_ready ( unsigned int x );
extern void change_led_state ( unsigned int led, unsigned int state );

#define TIMER_TICK_MASK 0xFFFFFFFF

#define INIT        1
#define LOOP        2
#define HEARTBEAT   3
#define TIMER       4

