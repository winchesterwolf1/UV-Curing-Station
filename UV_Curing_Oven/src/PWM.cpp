/***************************************************
 *  C Implementation of Generic PWM Driver for
 *  UV Curing Oven
 *
 ***************************************************
 */

#include "PWM.h"

AVR_PWM* PWM_Instance;

uint8_t PWM_Pins = 3;
float freq = 250;
float dutyCycle = 15;



/**
 * @brief Initialise the PWM pins
*/
void PWM_Init(void)
{
    PWM_Instance = new AVR_PWM(PWM_Pins, freq, dutyCycle);

    if (PWM_Instance)
    {
    PWM_Instance->setPWM();
    }
}

/**
 * @brief update the dutycycle of a PWM pin
 * 
 * @param pin Pin to change duty cycle of
 * @param duty new dutycycle of pin
*/
void PWM_Update(uint8_t pin, uint8_t duty)
{

}