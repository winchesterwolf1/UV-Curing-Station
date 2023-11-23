/***************************************************
 *  Program file for Knob
 *
 ***************************************************
 */

#include "Knob.h"
#include "Definitions.h"

int KnobCounter = 0; 
int currentStateKNOB_S1_PIN;
int lastStateKNOB_S1_PIN;
bool pressed_flag = 0;


ISR (PCINT1_vect)
{
  UpdateEncoderCallback();
}

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
 * @brief Blocking Debounce function for a pin input 
 * @param old_state Current state of the pin
 * @param pin The pin number
 * @return true if the pin remained consistent, false if not.
*/
bool DebounceButton(bool old_state, uint8_t pin)
{
  delay(1);
  bool current_state = digitalRead(pin);
  if(current_state == old_state)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/** 
 * @brief Function used to poll button for if it has been released since it was last called 
 * @return True if button has been released
*/
bool KNOB_PollButtonRelease(void) 
{
  static bool save_state = BUTTON_RELEASED;
  bool new_state = digitalRead(KNOB_KEY_PIN);
  bool ret = false;

  if((save_state == BUTTON_PRESSED) && (new_state == BUTTON_RELEASED) && DebounceButton(new_state, KNOB_KEY_PIN))
  {
    return true;
  } 
  save_state = new_state;
  return ret;
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
			KnobCounter --;
		} else {
			// Encoder is rotating CW so increment
			KnobCounter ++;
		}
		//Serial.println(KnobCounter);
	}
	// Remember last CLK state
	lastStateKNOB_S1_PIN = currentStateKNOB_S1_PIN;
}