/***************************************************
 *  Program file for Knob
 *
 ***************************************************
 */

#include "Knob.h"
#include "Definitions.h"


/*Private Defines************************************/

#define COUNTER_MAX 100
#define COUNTER_MIN 0


/*Private Macros*************************************/
/*Private Typedef************************************/
/*Global Variables***********************************/

CounterRange_t CounterRange = {COUNTER_MIN, COUNTER_MAX};
volatile int KnobCounter = 0; 
volatile int currentStateKNOB_S1_PIN;
volatile int lastStateKNOB_S1_PIN;
bool save_key_state = BUTTON_RELEASED;


/*Private Function Prototypes************************/

bool DebounceButton(bool old_state, uint8_t pin);
void UpdateEncoderCallback(void);


/*Callback Functions*****************************/

ISR (PCINT1_vect)
{
  UpdateEncoderCallback();
}

/** 
 * @brief IRQ Callback Function for the Encoder count. to be called on S1 or S2 pin change interrupt
*/
void UpdateEncoderCallback(void)
{
  
	currentStateKNOB_S1_PIN = digitalRead(KNOB_S1_PIN);
	// If last and current state of KNOB_S1_PIN are different, then pulse occurred
	// React to only 1 state change to avoid double count
	if (currentStateKNOB_S1_PIN != lastStateKNOB_S1_PIN  && currentStateKNOB_S1_PIN == 1){
		// If the KNOB_S1_PIN state is different than the KNOB_S2_PIN state then
		// the encoder is rotating CCW so decrement
		if (digitalRead(KNOB_S2_PIN) != currentStateKNOB_S1_PIN) {
			// Encoder is rotating ACW so increment if not at limit
      if(KnobCounter > CounterRange.low)  KnobCounter--;
		} else {
			// Encoder is rotating CW so increment if not at limit
      if(KnobCounter < CounterRange.high)	KnobCounter ++;
		}
		//Serial.println(KnobCounter);
	}
	// Remember last CLK state
	lastStateKNOB_S1_PIN = currentStateKNOB_S1_PIN;
}


/*Exported Functions*********************************/

/** 
 * @brief Initialisation for the Digital Encoder Knob 
*/
void KNOB_Init(void) 
{
  //Initialise Pins
  pinMode(KNOB_S1_PIN, INPUT);
  pinMode(KNOB_S2_PIN, INPUT);
  pinMode(KNOB_KEY_PIN, INPUT);

  //We Cannot use arduino interrupt functions, because they do not support these pins as external Interrupt pins
  //create pin change interupts on pins A0,1
  PCICR |= (1<<PCIE1);
  PCMSK1 |= ((1<<PCINT8)|(1<<PCINT9)); 
  lastStateKNOB_S1_PIN = digitalRead(KNOB_S1_PIN);
}


/** 
 * @brief Function used to poll button for if it has been released since it was last called 
 * @return True if button has been released
*/
bool KNOB_PollButtonRelease(void) 
{
  bool new_state = digitalRead(KNOB_KEY_PIN);
  bool ret = false;

  if(DebounceButton(new_state, KNOB_KEY_PIN))
  {
    if((save_key_state == BUTTON_PRESSED) && (new_state == BUTTON_RELEASED))
    {
      ret = true;
    } 
    save_key_state = new_state;
  }
  
  return ret;
}

/**
 * @brief Function used to set the valid range of the knob counter when changing menu state
 * 
 * @param low The low value of the valid range
 * @param high The high value of the valid range
*/
void KNOB_SetCounterRange(CounterRange_t range)
{
  //check inputs are a valid range
  if(range.low < COUNTER_MIN) range.low = COUNTER_MIN;
  if(range.high > COUNTER_MAX) range.high = COUNTER_MAX;

  //set counterRange limits before changing KnobCounter
  //so that knob counter does not get changed be to an invalid
  //value between these two steps
  CounterRange = range;
  if (KnobCounter <= CounterRange.low){ 
    KnobCounter = CounterRange.low;
  } else if (KnobCounter >= CounterRange.high){
    KnobCounter = CounterRange.high;
  }
}


/*Private Functions**********************************/

/** 
 * @brief Blocking Debounce function for a pin input 
 * @param old_state Current state of the pin
 * @param pin The pin number
 * @return true if the pin remained consistent, false if not.
*/
bool DebounceButton(bool old_state, uint8_t pin)
{
  bool ret = false;
  delay(1);
  bool current_state = digitalRead(pin);
  if(current_state == old_state)
  {
    ret = true;
  }
  return ret;
}


