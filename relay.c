#include <ioavr.h>
#include "relay.h"
#include "display.h"

relay_t relay;
extern display_t display;

void relay_operation(void)
{
    //All values must be more than 0.
    relay.activity = relay.event1 && relay.event2 && relay.event3; 
    
    switch(relay.state)
    { 
    //STOP state. Wait for "RELAY ON command".
    case 0:
    {    
        if(relay.activity) relay.state++;        
    }
    break;   
    //Do "RELAY ON command" or return.
    case 1:
    {
        if(!relay.activity) 
          relay.state--;
        else               
        {      
          display.aux_register = RELAY_ON;
          display.refresh_timeout = 0;    
          relay.operation_timeout = RELAY_ON_TO;
          relay.state++;
        } 
    }
    break; 
    
    //Wait for "RELAY OFF command" or timeout. 
    case 2:
    {
        if(!relay.activity || relay.operation_timeout==0)
        {
          display.aux_register = RELAY_OFF;
          display.refresh_timeout = 0;      
          relay.operation_timeout=RELAY_OFF_TO;
          relay.state++;
        }              
    }  
    break;
                                       
    //Wait for RELAY OFF timeout to elapsed.
    case 3:
    {         
        if(relay.operation_timeout == 0)
        relay.state = 0;   
    }
    break;
    
    default: 
      relay.state = 0; 
    break;        
    }
}
