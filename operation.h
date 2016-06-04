#ifndef OPERATION_H_
#define OPERATION_H_

#include "avr_compiler.h"

/*Set up the measurement parameters*/
#define MEASUREMENT_TO  2 //2sec. DS18b20 have no result - measur-t failure.
#define AUTOTRIGGER_TO  2 //2sec. Automatic trigger of temperature measuremnt.

/*Set up the temperature adjustment range*/
#define TOP_TEMP        30
#define BTM_TEMP        (-19)

/*Set up the temperature Down and Up hysteresis*/
#define TEMP_DN_HYSTERESIS  (-1)
#define TEMP_UP_HYSTERESIS   2

typedef struct operation_
{
    uint8_t mode;
    uint8_t mode_timeout;
    int8_t  temperature;          //A temperature which is set by user.
    //uint8_t temperature_up_hyst;  //Upper hysteresis(when relay switches ON)
    //uint8_t temperature_low_hyst; //Lower hysteresis(when relay switches OFF)
} operation_t;

typedef struct temperature_measurement_
{
    uint8_t state;                //0-idle; 1-start; 4-complete successfully; 255-error
    uint8_t timeout;
    uint8_t autotrigger_timeout;
    int8_t  temperature;          //measured temperature - value(rename)
} measurement_t;


void operation_procedure(void);   //operation_fsm
void temperature_measurement(void);
void temperature_measurement_autotrigger(void);
void temperature_thresholds_check(void);


#endif /*OPERATION_H_*/