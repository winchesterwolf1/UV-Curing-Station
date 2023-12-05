/***************************************************
 *  Header File for the Generic PWM Implementation
 *
 ***************************************************
 */

#ifndef PWM_H // include guard
#define PWM_H

#include "Definitions.h"
#include "AVR_PWM.h"

/*Exported Defines**********************************/
/*Exported Macros***********************************/
/*Exported Typedefs*********************************/
/*Exported Function Prototypes**********************/

void PWM_Init(void);
void PWM_Update(uint8_t pin, float duty);

#endif /* PWM_H */