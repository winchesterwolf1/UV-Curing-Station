/***************************************************
 *  Header File for the Generic PWM Implementation
 *
 ***************************************************
 */

#ifndef PWM_H // include guard
#define PWM_H

#include "Definitions.h"


void PWM_Init(void);
void PWM_Update(uint8_t pin, uint8_t duty);

#endif /* PWM_H */