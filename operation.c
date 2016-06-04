#include "operation.h" //-rename to measurement.h
#include "ds18b20.h"
#include "buttons.h"
#include "display.h"
#include "relay.h"
#include "voltagectrl.h"

extern btn_t btn;
extern display_t display;
measurement_t measurement;
operation_t op;
extern relay_t relay;
extern voltage_t voltage_event;

#pragma location = 0x0010
__root __eeprom signed char e_temperature;


void operation_procedure(void) //button (service) operation_fsm - move to buttnons.c
{
  if (!relay.event3) {op.mode = 4;}
    
    switch (op.mode)
    {   

    case 0:     //Normal mode:
                //Display ambient temperature, Check the thresholds, Manage the relay state.
        {  
            if(measurement.autotrigger_timeout == 0) 
            {
            display.value=measurement.temperature; //update displayed temperature
            display_update();
            //display.led_word = display_bcd(display.value);
            }
        }
        break;

    case 1:    //Set Temperature - Increasing
    case 2:    //Set Temperature - Decreasing
        {
            if(btn.adj_timeout==0)
            {    
                if(btn.status == 0x01 && op.temperature < TOP_TEMP) op.temperature++;
                if(btn.status == 0x02 && op.temperature > BTM_TEMP) op.temperature--; 
     
                display.value = op.temperature;
                display_update();                
                btn.adj_timeout = 200;  //400ms - the period of increment/decrement
            }

              //if(btn.status == 0) btn.adj_timeout=0;    //Need for the quick pushes sequence    
        }
        break;

    case 3: //Switch ON/OFF the relay. (maybe make extra function)
        {
            //Indicate "Stop" state by blinking the display 1Hz;
            if (relay.event2) 
            {
              relay.event2 = 0; 
              display.blink = 1;
            } 
            else 
            {
              relay.event2 = 1;  
              display.blink = 0;
            }    
            op.mode=0;
        }
        break; 
        
    case 4: //Alarm mode - Mains Voltage is out of working range
        { 
          display.blink = 1;
          if (voltage_event.ov) display.led_word = 0x9CC1; //OverVoltage "oU"
          if (voltage_event.uv) display.led_word = 0xE3C1; //UnderVoltage "uU" 
          if (relay.event3) {relay.event2 = 1; op.mode=0; display.blink = 0;}
        }
        break;
        
    default:
        op.mode=0;
        break;        
    }
}



void temperature_measurement(void)
{
  uint8_t temperature_l;
  uint8_t temperature_h;
            
    switch (measurement.state)
    {
    case 0:
        break;
    case 1:
        {
            #if DS18B20_STOPINTERRUPT
	    cli();
	    #endif
            ds18b20_reset();                            //reset
	    ds18b20_writebyte(DS18B20_CMD_SKIPROM);     //skip ROM
	    ds18b20_writebyte(DS18B20_CMD_CONVERTTEMP); //start temperature conversion  
        
            measurement.timeout= 2;                     //2 sec measurement failure timeout
            measurement.state++;
            #if DS18B20_STOPINTERRUPT
	    sei();
	    #endif
        }
        break;
    case 2:
        {
            if(ds18b20_readbit())           //wait until conversion is complete
              measurement.state++; 
            
            if(measurement.timeout == 0)
              measurement.state = 255;    //timeout elapsed - error
        }
        break;
    case 3:
        { 
            #if DS18B20_STOPINTERRUPT
	    cli();
	    #endif
            ds18b20_reset();
	    ds18b20_writebyte(DS18B20_CMD_SKIPROM);
	    ds18b20_writebyte(DS18B20_CMD_RSCRATCHPAD);

	    //Read 2 byte from scratchpad
	    temperature_l = ds18b20_readbyte();
	    temperature_h = ds18b20_readbyte();

            //Prepare the result, fractional part is removed
	    measurement.temperature = (( temperature_h<<4 ) | (temperature_l>>4));       
            measurement.state++; 
            
            #if DS18B20_STOPINTERRUPT
	    sei();
	    #endif
        }
        break;     
        
    case 4: break;
    case 255:break;
    default:
        measurement.state = 0;
        break;
    }
}

void temperature_measurement_autotrigger(void)
{
        if(measurement.autotrigger_timeout == 0) 
        {             
            measurement.state = 1;      //start temperature measurement
            measurement.autotrigger_timeout = 2;    //2 sec auto trigger period 
            
            /*be sure that there aren't any errors*/
            temperature_thresholds_check();
            
        }
}


void temperature_thresholds_check(void)
{
  /* MUST be checked if there's no errors: measurement.state !=255 or == 4 !!!*/
  
    if(measurement.temperature <= e_temperature + TEMP_DN_HYSTERESIS) 
        relay.event1 = 0;
    else 
      if (measurement.temperature >= e_temperature + TEMP_UP_HYSTERESIS) 
        relay.event1 = 1;
}