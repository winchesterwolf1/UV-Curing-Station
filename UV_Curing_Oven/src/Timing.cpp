/***************************************************
 *  C Implementation for basic timers
 *
 ***************************************************
 */


#include "Timing.h"
#include "Definitions.h"

/*Private Defines************************************/
/*Private Macros*************************************/
/*Private Typedef************************************/
/*Global Variables***********************************/
/*Private Function Prototypes************************/
/*Callback Functions*********************************/
/*Exported Functions*********************************/

/**
 * @brief Get the elapsed time in milliseconds since calling TIM_UpdateTime for particular timer
 * 
 * @param timer Timer to check.
*/
unsigned long TIM_TimeElapsed_ms(TimerTime_t* timer)
{
    return (millis()-*timer);
}

/**
 * @brief Get the elapsed time in seconds since calling TIM_UpdateTime for particular timer
 * 
 * @param timer Timer to check.
*/
unsigned long TIM_TimeElapsed_s(TimerTime_t* timer)
{
    return (millis()-*timer)/1000U;
}

/**
 * @brief Set timer context to current time
 * 
 * @param timer Timer update.
*/
void TIM_UpdateTime(TimerTime_t* timer)
{
    *timer = millis();
}

/*Private Functions**********************************/


