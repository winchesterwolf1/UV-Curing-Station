/***************************************************
 *  Header File for Basic Timer Functionality
 *
 ***************************************************
 */

#ifndef TIMING_H // include guard
#define TIMING_H

typedef unsigned long TimerTime_t;

unsigned long TimeElapsed_ms(TimerTime_t* timer);
unsigned long TimeElapsed_s(TimerTime_t* timer);
void UpdateTime(TimerTime_t* timer);


#endif /* TIMING_H */