#ifndef DEFINITIONS_H // include guard
#define DEFINITIONS_H

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/* Pin Definitions */
#define UVLED_SW_PIN 5
#define KNOB_KEY_PIN 16 //Push switch Active Low
#define KNOB_S1_PIN 15 //ACW rotation first low
#define KNOB_S2_PIN 14 //ACW rotation second low

#define MAX_ON_TIME_S 300

#define MAX_BUTTON_CHAR 14
#define MAX_BUTTONS_ON_SCREEN 3




#endif /* DEFINITIONS_H */