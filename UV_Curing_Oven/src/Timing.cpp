/***************************************************
 *  C Implementation for basic timers
 *
 ***************************************************
 */


#include "Timing.h"
#include "Definitions.h"


unsigned long TimeElapsed_ms(TimerTime_t* timer)
{
    return (millis()-*timer);
}

unsigned long TimeElapsed_s(TimerTime_t* timer)
{
    return (millis()-*timer)/1000U;
}

void UpdateTime(TimerTime_t* timer)
{
    *timer = millis();
}

