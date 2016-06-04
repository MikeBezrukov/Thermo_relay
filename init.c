
#include <ioavr.h>
#include "init.h"
#include "voltagectrl.h"
#include "usi.h"

void mcu_init(void)
{
// Crystal Oscillator division factor: 1
CLKPR=0x80;
CLKPR=0x00;

// IO - Ports initialization
PORTA=0xF0;
DDRA=0x70;

 //DDRA=0x71; //Activate U_Debug probe.
//PORTB=0x0D;
PORTB=0x0F;
DDRB=0x01;

// Timer/Counter 0 initialization
/* This timer will generate a time-out 2ms.*/
// Clock source: System Clock
// Clock value: 125,000 kHz
// Mode: CTC top=OCR0A
TCCR0A=0x02;
TCCR0B=0x03;
TCNT0=0x00;
OCR0A=0xF9;
OCR0B=0x00;


// External Interrupt(s) initialization
// INT0: Off
MCUCR=0x00;
GIMSK=0x00;

// Timer/Counters Interrupt(s) initialization
//TIMSK0=0x00;
//TIMSK1=0x00;

voltage_ctrl_init();

// Watchdog Timer initialization
// Watchdog Timer Prescaler: OSC/1024k
// Watchdog Timer interrupt: Off
asm("wdr");
WDTCSR=0x39;
WDTCSR=0x29;

// Universal Serial Interface initialization
// Mode: Three Wire (SPI)
v_spi_init();
//Clear registers (In order to ensure STOP state)
write_rg(0x0000);           
}