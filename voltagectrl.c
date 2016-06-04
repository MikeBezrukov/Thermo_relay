#include "voltagectrl.h"
#include "relay.h"

extern relay_t relay;

static uint8_t smpl_max_val;         //The instantaneous amplitude.
static uint8_t inactive_per_flg;     //Inactive half of period flag. (Time to average max_val.)

static uint8_t avr_buff [AVR_FACTOR];//The averaging buffer.
static uint8_t avr_idx;              //An item index in the averaging buffer.
static uint16_t avr_acc;             //The averaging accumulator.

voltage_t voltage_event;


#pragma vector = TIM1_COMPB_vect
__interrupt __root void TC1_COMPB_ISR( void ) 
{
//PORTA ^= 1;
}

#pragma vector = ADC_vect
__interrupt __root void ADC_CONV_COMPLETE_ISR( void )        
{
  uint8_t adc_val;
  static uint8_t inactive_per_flg_set;
         
    adc_val = ADCH;
    //Positive Half of period - serching for max_val.
    if (adc_val > ZERRO_LEVEL)
    {      
      if (adc_val > smpl_max_val) smpl_max_val = adc_val;
      
      inactive_per_flg_set = 0;
    }
    //Negative Half of period - averaging max_val and checking thresholds.
    else
    {             
        if (!inactive_per_flg_set)
        {
            inactive_per_flg_set = 1;
            inactive_per_flg = 1;
        }
    }          
}

void voltage_check (void)
{
      if (!inactive_per_flg) return;
             
      /* 1. Accumulate/Average max_val*/ 
      avr_acc -= avr_buff[avr_idx];
      avr_acc += smpl_max_val;
      avr_buff[avr_idx++] = smpl_max_val;
      if (avr_idx >= AVR_FACTOR) avr_idx = 0;
      
      /* 2. Thresholds check */ 
      voltage_event.ov = voltage_event.uv = 0;
      
      if (avr_acc > UP_VOLTAGE_THRESHOLD) voltage_event.ov = 1;
      if (avr_acc < LOW_VOLTAGE_THRESHOLD) voltage_event.uv = 1;
      relay.event3 = (voltage_event.ov | voltage_event.uv)?0:1;
       
      /* 3. Do resets */
      smpl_max_val = 0;
      inactive_per_flg = 0; 
}


void voltage_ctrl_init(void)
{
/* Timer/Counter 1 initialization */
// Clock source: System Clock
// Clock value: 125,000 kHz
// Mode: CTC top = ICR1
// Generate interrupt on OCR1B compare match
TCCR1A=0x00;
TCCR1B=0x1B;
//TCNT1H=0x00;
//TCNT1L=0x00;
ICR1H=0x00;
//ICR1L=0xF9;   // 0,5kHz - is inaccurate
ICR1L=0x7C;     // 1kHz

//OCR1AH=0x00;
//OCR1AL=0x00;
OCR1BH=0x00;
//OCR1BL=0xF0;  // 0,5kHz
OCR1BL=0x70;    // 1kHz


/* TC-1 Interrupt(s) init */
// Generate interrupt on OCR1B compare match
TIMSK1=0x04;

/* Analog Comparator init */
// Analog Comparator: Off
ACSR=0x80;
//ADCSRB=0x00;
//DIDR0=0x00;

/* ADC initialization */
// ADC Clock frequency: 250,000 kHz
// ADC Auto Trigger Source: Timer1 Compare Match B
// Only the 8 most significant bits of the AD conversion result are used
// Digital input buffers on ADC0: On, ADC1: On, ADC2: On, ADC3: Off
// ADC4: On, ADC5: On, ADC6: On, ADC7: On

// Analog Input: ADC3 - PA3

DIDR0=0x08;
ADMUX=ADC_VREF_TYPE | 0x03;
//ADCSRA=0xAD;    //ADC Clock frequency: 250,000 kHz
//ADCSRA=0xAE;  //ADC Clock frequency: 125,000 kHz
ADCSRA=0xAF;  //ADC Clock frequency: 62,500 kHz
ADCSRB&=0x68;
ADCSRB|=0x15;

asm("sei");
}