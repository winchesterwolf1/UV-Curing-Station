#include "Definitions.h"
#include "Knob.h"
#include "OLED.h"
#include "PWM.h"

extern int KnobCounter;
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
  pinMode(UVLED_SW_PIN, OUTPUT);
  digitalWrite(UVLED_SW_PIN, LOW);
  KNOB_Init();
  init_OLED();  
  init_PWM();
  Serial.begin(9600);
  
}

void loop() 
{
  if(KNOB_PollButtonRelease() == true)
  {
    switch(menu_state) 
    {
    case 0:
      if (KnobCounter == 0) {
        digitalWrite(UVLED_SW_PIN, HIGH);
        UV_OnTime = millis();
      } else if (KnobCounter == 1) 
      {
        digitalWrite(UVLED_SW_PIN, LOW);
        UV_OnTime = 0;
      } else if (KnobCounter == 2) 
      {
        menu_state = 1;
        KnobCounter = settings.on_time;
      } else if (KnobCounter == 3) 
      {
        menu_state = 2;
        KnobCounter = settings.brightness;
      }
      break;
    case 1:
      menu_state = 0;
      settings.on_time = KnobCounter;
      KnobCounter = 0;
      break;
    case 2:
      menu_state = 0;
      settings.brightness = KnobCounter;
      KnobCounter = 0;
      break;
    }

  }
  
  if((millis() - UV_OnTime > 1000UL*settings.on_time)&&UV_OnTime != 0)
  {
    digitalWrite(UVLED_SW_PIN, LOW);
    UV_OnTime = 0;
  }

  update_OLED(menu_state);
  //update_LED();
}
