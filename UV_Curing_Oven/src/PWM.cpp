/***************************************************
 *  C Implementation of Generic PWM Driver for
 *  UV Curing Oven
 *
 ***************************************************
 */

#include "PWM.h"

/*Private Defines************************************/

#define PWM_FREQ ((float)250) //Hz


/*Private Macros*************************************/
/*Private Typedef************************************/
/*Global Variables***********************************/

AVR_PWM* PWM_Instance;


/*Private Function Prototypes************************/
/*Callback Functions*********************************/
/*Exported Functions*********************************/

/**
 * @brief Initialise the PWM pins
*/
void PWM_Init(void)
{
    PWM_Instance = new AVR_PWM(PWM_MOTOR_PIN, PWM_FREQ, 0);

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
void PWM_Update(uint8_t pin, float duty)
{
    PWM_Instance->setPWM(pin, PWM_FREQ, duty);
}


/*Private Functions**********************************/
