/***************************************************
 *  Header File for the OLED C Implementation
 *
 ***************************************************
 */

#ifndef OLED_H // include guard
#define OLED_H

#include "Menu.h"
#include "Definitions.h"

void OLED_Init(void);
void OLED_Update(MenuHandle_t* hmenu);

#endif /* OLED_H */