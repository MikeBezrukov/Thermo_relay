#include "voltagectrl.h"
#include "buttons.h"
#include "display.h"
#include "ds18b20.h"
#include "relay.h"
#include "operation.h"
#include "to.h"
#include "init.h"
#include "usi.h"

extern display_t display;
extern relay_t relay;
extern operation_t op;
extern __root __eeprom int8_t e_temperature;

void main( void )
{
  mcu_init();
  
  op.mode =0;
  relay.event1=0;     //Change with macro OFF//
  relay.event2=1;     //Manual switch ON/OFF
  //relay.event3=1;
  
  //Initial delay between Power ON and Switched ON the relay.
  relay.operation_timeout = RELAY_PWR_ON_TO; 
  relay.state=3;      //Wait for 10 sec delay elapsed.
  
  //op.temperature = e_temperature; //Get the temperature from the EEPROM
  //display.value = op.temperature; //Update LED indicator //???does it necessary???
  //display_update();          

  while (1)
  { 
    to();                     //Time-out service.
    voltage_check();          //Checking mains voltage.
        
    display_refresh();        //Display dynamic indication service.
    display_dec_point();      //Decimal point operation & display blinking.      
        
    btn_check();              //Checking the buttons state service.
    btn_procedure();          //Make decision which op. mode to choose.
    operation_procedure();    //Do an operation for specified op. mode.
               
    temperature_measurement();              //Temperature measurement service.         
    temperature_measurement_autotrigger();  //Measurement autostart, T=2sec.
        
    relay_operation();        //Relay operation service.  
  }
}
