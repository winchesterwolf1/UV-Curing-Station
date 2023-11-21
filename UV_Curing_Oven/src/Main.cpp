#include "Definitions.h"
#include "Knob.h"
#include "OLED.h"
#include "PWM.h"

int counter = 0;
unsigned long UV_OnTime = 0;
int menu_state = 0;
int selected_time = 0;

struct Selected_Settings {
  int on_time;
  int brightness;
  int motor_speed;
};

Selected_Settings settings;

void setup() {
  pinMode(UVLED, OUTPUT);
  digitalWrite(UVLED, LOW);
  init_knob();
  init_OLED();  
  init_PWM();
  Serial.begin(9600);
  
}

void loop() {
  if(poll_button()){
    switch(menu_state) {
    case 0:
      if (counter == 0) {
        digitalWrite(UVLED, HIGH);
        UV_OnTime = millis();
      } else if (counter == 1) {
        digitalWrite(UVLED, LOW);
        UV_OnTime = 0;
      } else if (counter == 2) {
        menu_state = 1;
        counter = settings.on_time;
      } else if (counter == 3) {
        menu_state = 2;
        counter = settings.brightness;
      }
      break;
    case 1:
      menu_state = 0;
      settings.on_time = counter;
      counter = 0;
      break;
    case 2:
      menu_state = 0;
      settings.brightness = counter;
      counter = 0;
      break;
    }

  }
  
  if((millis() - UV_OnTime > 1000UL*settings.on_time)&&UV_OnTime != 0){
    digitalWrite(UVLED, LOW);
    UV_OnTime = 0;
  }

  update_OLED(menu_state);
  //update_LED();
}
