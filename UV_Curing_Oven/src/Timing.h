/***************************************************
 *  Header File for Basic Timer Functionality
 *
 ***************************************************
 */

#ifndef TIMING_H // include guard
#define TIMING_H

/*Exported Defines**********************************/
/*Exported Macros***********************************/
/*Exported Typedefs*********************************/

typedef unsigned long TimerTime_t;


/*Exported Function Prototypes**********************/

unsigned long TIM_TimeElapsed_ms(TimerTime_t* timer);
unsigned long TIM_TimeElapsed_s(TimerTime_t* timer);
void TIM_UpdateTime(TimerTime_t* timer);


#endif /* TIMING_H */