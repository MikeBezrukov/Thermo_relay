#include "display.h"
#include "usi.h"
#include "bcd.h"
#include "relay.h"

__flash uint8_t digit[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};

display_t display;


/*
Binary (byte) -> Decimal(2 bytes) -> (2dig)7-segment LED indicator conversion.
Input data range: x = -19 ... 99
*/
uint16_t display_bcd(int8_t x)
{                                                                       
  uint8_t  sign=PLUS; 
  uint16_t led_word;
  if(x<0) {sign = MINUS; x = ~x + 1;}
  //led_word =  (((x<10?0xFF:digit[x/10]) & sign)<<8) | digit[x%10]; 
  led_word =   (((x<10?0xFF:digit[bin2bcd(x)>>4]) & sign)<<8) | digit[bin2bcd(x)&0x0F]; 
    return led_word;  
}

//Update display value
void display_update(void)
{
    display.led_word = display_bcd(display.value);
}  

//Refresh display    - RENAME to UPDATE - Update ^^^  - remove -(not good idea;)
void display_refresh(void)
{
 static uint8_t digit_no;
 static uint8_t blink_flg, blink_cnt;
 uint16_t reg;
 
   
    if(display.refresh_timeout == 0)
    {   
     display.refresh_timeout = DISPLAY_REFRESH_TO;
     digit_no = ~digit_no;            //Toggle digits (Left / Right)
    
     if(display.blink)
     {
       if(blink_cnt == 0)
       {
        blink_flg = ~blink_flg;
        blink_cnt = DISPLAY_BLINK_TO;
       }
       else 
         blink_cnt--; 
     }
     else 
       blink_flg=0;
     
     
     reg = display.led_word; 
     reg = blink_flg ? 0xFFFF : reg;  
     
     /*
     reg = digit_no ? (reg&display.dec_point | 0x4000) : ((reg>>8) | 0x8000);
     reg = (reg & 0xF0FF) | ((int)(display.aux_register & 0x0F)<<8); 
     write_rg(reg); 
     */
     PORTA &= ~0x06;
     reg = digit_no ? (reg&display.dec_point) : (reg>>8);
     write_rg(reg);
     PORTA |= digit_no ? 0x04 : 0x02;
  
    }  
}


void display_dec_point(void)
{   //Blank right decimal point, when display is blinking.
    if(display.blink) {display.dec_point=DOT_OFF; return;}       
    
    if(display.dot_blink_timeout==0)
    {
        display.dec_point = (display.dec_point==DOT_OFF) ? DOT_ON : DOT_OFF; 
        
        if(display.aux_register & RELAY_ON) 
          display.dot_blink_timeout = DISPLAY_DOT_BLINK_TO_RLY_ON;
        else 
          display.dot_blink_timeout = DISPLAY_DOT_BLINK_TO_RLY_OFF;
    }
}