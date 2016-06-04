#ifndef VOLTAGECTRL_H_
#define VOLTAGECTRL_H_

#include <ioavr.h>
#include "avr_compiler.h"
#include "init.h"

/*
Schematic: R3+R5 = 660kOhm; R9 = 9,1kOhm. => devision factor=73,5
ADConversion:
U_RMS   Ampl.    Uin_ADC   ADC_val(Uref=5V)
250     352      4,78      243     
220     310      4,21      214
198     279      3,79      193
176     248      3,37      171

Scale factor:
SCALE = ADC_val/U_RMS = 0.972
*/

//#define UP_VOLTAGE      242   //Up limit in Volts
//#define LOW_VOLTAGE     176   //Low limit in Volts
#define UP_VOLTAGE      243   //Up limit in Volts
#define LOW_VOLTAGE     165   //Low limit in Volts

#define AVR_FACTOR      12     //Number of periods within which Umax is averaging.
#define ZERRO_LEVEL     10    //U_rms_zerro_lvl_ = ZERRO_LEVEL / SCALE.

//#define SCALE           0.972 //Scale factor, it's used for callibration.
#define SCALE           1.004
#define UP_VOLTAGE_THRESHOLD    ((uint16_t)(UP_VOLTAGE * SCALE * AVR_FACTOR))
#define LOW_VOLTAGE_THRESHOLD   ((uint16_t)(LOW_VOLTAGE * SCALE * AVR_FACTOR))

typedef struct voltage_events_
{
  uint8_t ov;   //OverVoltage event flag.
  uint8_t uv;   //UnderVoltage event flag.
}voltage_t;

void voltage_check (void);

#define ADC_VREF_TYPE   0x00
void voltage_ctrl_init(void);

#endif /*VOLTAGECTRL_H_*/