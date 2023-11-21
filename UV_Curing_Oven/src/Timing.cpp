#include "Timing.h"
#include "Definitions.h"

//pg78
//pg84
//TC0
//Normal Mode
//TCCR0 control register A and B
//clock runs at 16MHz
//TCNT0 T/C register
//TIMSK0  interupt mask
//TIFR0 interupt register
//SREG status register bit7 0b10000000 global interupt enable
//Timer 0 overflow vector adress = 0x0020 
// long timing = 0;

// ISR(TIMER0_OVF_vect){
//   timing++;
//   Serial.print(timing);
// }

// void init_timers(){
//   SREG |= 0b10000000;
//   TCCR0A |= 0b00000000;
//   TCCR0B |= 0b00000101;
//   TIMSK0 |= 0b00000001;
// }

