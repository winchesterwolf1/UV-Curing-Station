/***************************************************
 *  Header file for Knob C file
 *
 ***************************************************
 */

#ifndef KNOB_H // include guard
#define KNOB_H

/*Exported Defines**********************************/

#define BUTTON_PRESSED (LOW)
#define BUTTON_RELEASED (HIGH)


/*Exported Macros***********************************/
/*Exported Typedefs*********************************/

//struct for current counter range
typedef struct 
{
  int low;
  int high;
}CounterRange_t;


/*Exported Function Prototypes**********************/

void KNOB_Init(void);
bool KNOB_PollButtonRelease(void);
void KNOB_SetCounterRange(CounterRange_t range);




#endif /* KNOB_H */