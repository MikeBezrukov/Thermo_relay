#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "avr_compiler.h"

#define BTN_DEBOUNCE_TO 10    //20ms
#define BTN_HOLD_TO     2     //2sec
#define BTN_AUTOADJ_TO  200   //400ms (automatic dec/increment timeout)  
#define BTN_OP_MODE_TO  10    //10sec (Operation mode change timeout)

#define BTN_UP_MASK     0x04
#define BTN_DN_MASK     0x80
#define BTN_UP_PORT     PINB
#define BTN_DN_PORT     PINA

typedef struct buttons_status
{
    uint8_t timeout; 
    uint8_t hold_timeout; 
    uint8_t adj_timeout;
    uint8_t current_state;
    uint8_t previous_state;

    uint8_t status;
} btn_t;

void btn_check(void);
void btn_procedure(void);

#endif /*BUTTONS_H_*/
