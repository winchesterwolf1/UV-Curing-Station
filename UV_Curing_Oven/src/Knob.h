/***************************************************
 *  Header file for Knob C file
 *
 ***************************************************
 */

#ifndef KNOB_H // include guard
#define KNOB_H

#define BUTTON_PRESSED (LOW)
#define BUTTON_RELEASED (HIGH)


void KNOB_Init(void);
bool KNOB_PollButtonRelease(void);




#endif /* KNOB_H */