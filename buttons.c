#include "buttons.h"
#include "display.h"
#include <ioavr.h>
#include "operation.h"



extern display_t display;
extern operation_t op;
extern __root __eeprom signed char e_temperature;

btn_t btn;


/*Checking the physical state of the buttons*/
void btn_check(void)
{
    if(btn.timeout ==0)
    {
        btn.timeout = BTN_DEBOUNCE_TO;
        btn.current_state = (~BTN_UP_PORT & BTN_UP_MASK) ? 0x01 : 0x00;
        btn.current_state |= (~BTN_DN_PORT & BTN_DN_MASK) ? 0x02 : 0x00;
        
        //When sth has been changed - wait
        if(btn.current_state != btn.previous_state)    
            btn.previous_state = btn.current_state;
        //When stable - update btn.status
        else    
            btn.status = btn.current_state;             
    }
}


/*Analyze buttons state and make decision which OPERATION MODE to select*/
//void btn_set_op_mode(void)
void btn_procedure(void)
{
    static unsigned char s;
    switch (s)
    {
    //Wait for a button to be pushed.
    case 0:
    if(btn.status)                        
    {
        btn.hold_timeout = BTN_HOLD_TO;   //2sec. timeout.
        s++;
    }
    break;
    //Wait for button(s) hold interval to elapsed.
    case 1:
    if(btn.hold_timeout == 0)    
    {
        if(btn.status)         //Set The Operation mode
        {    
            op.mode=btn.status;  /*!!! Do Making decision procedure !!! maybe*/
            s++;  /*WHEN s==3(STOP mode) no need to go stage 2 (store EEPROM)*/
        }         
        op.mode_timeout = BTN_OP_MODE_TO;  //10 sec
    }
    if(btn.status == 0) s--;
    break;
    //Change operation mode if there's no buttons activity. 
    //If set temperature has been changed - save the value into the EEPROM.
    case 2:                
    if(op.mode_timeout==0)
    {
        if(e_temperature != op.temperature) e_temperature = op.temperature; //save into EEPROM
        s=0;        
        /*if(operation_mode != 3)*/ op.mode = 0;
     }
    //When a button is pushed (while adjusting) - update the timeout.
    if(btn.status) op.mode_timeout = BTN_OP_MODE_TO;
    
    break; 
                                                        
    default:
    s=0; 
    break;
    }
}

