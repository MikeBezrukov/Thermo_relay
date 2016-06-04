#include <ioavr.h>
#include "to.h"
#include "buttons.h"
#include "display.h"
#include "relay.h"
#include "operation.h"

extern btn_t btn;
extern display_t display;
extern relay_t relay;
extern operation_t op;
extern measurement_t measurement;

void to(void)
{
 static uint16_t seconds_timeout=0; 
 
        /*Reset watchdog timer*/
        asm("wdr");
        
        /*Time-outs count procedure*/  
        if(TIFR0&0x02)
        {
        TIFR0=0x02;
        if(btn.timeout) btn.timeout--;
        if(btn.adj_timeout) btn.adj_timeout--;
        if(display.refresh_timeout) display.refresh_timeout--; 
        if(display.dot_blink_timeout) display.dot_blink_timeout--;  

        if(seconds_timeout) seconds_timeout--;
        else
            {
             seconds_timeout = 500;
             if(btn.hold_timeout) btn.hold_timeout--;
             if(relay.operation_timeout) relay.operation_timeout--;
             if(op.mode_timeout) op.mode_timeout--;
                
             if(measurement.timeout) measurement.timeout--;
             if(measurement.autotrigger_timeout) measurement.autotrigger_timeout--;   
            }
        } 
}

