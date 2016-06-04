/* The Virtual SPI function library.  
***********************************************************************   
It's used the USI (universal serial interface) hardware as SPI (mode 0).
FSCK max = Fmcu / 2;
No interrupts are used.
*/     

#include <ioavr.h>  
#include "usi.h"             


void v_spi_init(void)
{
V_SPI_PORT &= ~(1<<SCK)|(1<<MOSI);
V_SPI_DDR  |= (1<<MOSI)|(1<<SCK)|(1<<SSn); 
// Mode: Three Wire (SPI). Clock source: Register & Counter=USICLK 
USICR=0x12;
}


  /* USICR */ 
#define USIWM0 4
#define USICS0 2
#define USICLK 1
#define USITC  0

void v_spi_tx(uint8_t data)
{
 uint8_t i; 
    USIDR = data;
    for(i=8;i--;)
    {
    USICR = (1<<USIWM0)|(0<<USICS0)|(1<<USITC);
    USICR = (1<<USIWM0)|(0<<USICS0)|(1<<USITC)|(1<<USICLK);
    }
}
                       
void write_rg(uint16_t data)
{
    CLR_SS();
  
    v_spi_tx((uint8_t)(data >> 8));   
    v_spi_tx((uint8_t)(data & 0x00FF)); 
 
    SET_SS(); 
    SET_SS(); 
    CLR_SS();
}  
