#ifndef USI_H_
#define USI_H_

#include "avr_compiler.h"

#define V_SPI_PORT  PORTA
#define V_SPI_DDR   DDRA    
#define SSn         6
//#define MISO        5
#define MOSI        5
#define SCK         4 

#define SET_SS()    V_SPI_PORT |= (1<<SSn)
#define CLR_SS()    V_SPI_PORT &=~(1<<SSn) 

void v_spi_init(void);
void v_spi_tx(uint8_t data);
void write_rg(uint16_t data);

#endif /*USI_H_*/