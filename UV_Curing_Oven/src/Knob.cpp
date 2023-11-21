#include "Knob.h"
#include "Definitions.h"

extern int counter;
int currentStateS1;
int lastStateS1;
bool pressed_flag = 0;
bool button_pressed = LOW;


ISR (PCINT1_vect)
{
  updateEncoder();
}

void init_knob() {
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(Key, INPUT);
  //create pin change interupts on pins A0,1
  PCICR |= (1<<PCIE1);
  PCMSK1 |= ((1<<PCINT8)|(1<<PCINT9)); 
  lastStateS1 = digitalRead(S1);
}

bool poll_button() {
  if(digitalRead(Key) == button_pressed){
    delay(1);
    if(digitalRead(Key) == button_pressed){
      pressed_flag = 1;
    }      
  } 
  if(pressed_flag == 1 && (digitalRead(Key) == !button_pressed)){
    pressed_flag = 0;
    return 1;
  } else {
    return 0;
  }    
}

void updateEncoder(){
	currentStateS1 = digitalRead(S1);
	// If last and current state of S1 are different, then pulse occurred
	// React to only 1 state change to avoid double count
	if (currentStateS1 != lastStateS1  && currentStateS1 == 1){
		// If the S1 state is different than the S2 state then
		// the encoder is rotating CCW so decrement
		if (digitalRead(S2) != currentStateS1) {
			counter --;
		} else {
			// Encoder is rotating CW so increment
			counter ++;
		}
		//Serial.println(counter);
	}
	// Remember last CLK state
	lastStateS1 = currentStateS1;
}