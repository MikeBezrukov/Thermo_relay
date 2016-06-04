#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "avr_compiler.h"

//Timeout constants
#define DISPLAY_REFRESH_TO            5     //10ms
#define DISPLAY_BLINK_TO              100   //100 periods of REFRESH_TO (0,5Hz)
#define DISPLAY_DOT_BLINK_TO_RLY_ON   170   //3Hz - dot blink TO,when Relay ON
#define DISPLAY_DOT_BLINK_TO_RLY_OFF  1000  //0,5Hz - dot blink TO, when - OFF

//Display's Sign indicator mask
#define MINUS     0xBF
#define PLUS      0xFF

//Display's Decimal point (right dot) mask
#define DOT_ON    0x7F
#define DOT_OFF   0xFF

typedef struct display_interface_
{
    uint8_t  refresh_timeout;
    uint8_t  blink_timeout;
    uint16_t dot_blink_timeout;   
  
    int8_t   value;           //The decimal value of the temperature.
    uint16_t led_word;        //BCD converted & LED seg. translated word.
    uint8_t  dec_point;       //LED decimal point ON/OFF control.
    uint8_t  blink;           //LED blinking ON/OFF control.
    uint8_t  aux_register;    //MSB register, 4 lsbits control.
} display_t;


uint16_t display_bcd(int8_t x);

void display_update(void);
void display_refresh(void);
void display_dec_point(void);

#endif /*DISPLAY_H_*/