#include "Definitions.h"
#include "Menu.h"
#include "Knob.h"
#include "OLED.h"
#include "PWM.h"
#include "Timing.h"


/*Private Defines**********************************/

/*Private Typedef**********************************/

typedef struct {
  unsigned long on_time;
  int motor_speed;
}SelectedSettings_t;


/*Global Variables*********************************/

extern int KnobCounter;
int old_count;
unsigned long UV_OnTime = 0;
SelectedSettings_t settings;
extern MenuHandle_t hmenu;
TimerTime_t UVLED_OnTimer = 0;


/*Private Function Prototypes**********************/

void UpdateOnButton(MenuHandle_t* hmenu, SelectedSettings_t* settings);
bool SetStatusMessage(MenuHandle_t* hmenu);
void UpdateCounterRange(MenuHandle_t* hmenu);


void setup(void) 
{
  pinMode(UVLED_SW_PIN, OUTPUT);
  digitalWrite(UVLED_SW_PIN, LOW);
  KNOB_Init();
  OLED_Init();  
  PWM_Init();
  Serial.begin(9600);
  Serial.write("Starting!\r\n");
  KNOB_SetCounterRange(0, 2);
}

void loop(void) 
{
  static bool updateScreen = true;
  if(KNOB_PollButtonRelease() == true)
  {
    UpdateOnButton(&hmenu, &settings);
    UpdateCounterRange(&hmenu);
    //MENU_SetMainStatusMessage(&hmenu);
    updateScreen = true;
  }
  
  if((TimeElapsed_s(&UVLED_OnTimer) >= settings.on_time) && (digitalRead(UVLED_SW_PIN) == HIGH))
  {
    digitalWrite(UVLED_SW_PIN, LOW);
    updateScreen = true;
    //MENU_SetMainStatusMessage(&hmenu);
  }

  if(KnobCounter != old_count) 
  {
    updateScreen = true;
    old_count = KnobCounter;
  }
  

  if(updateScreen) 
  {
    OLED_Update(&hmenu);
    updateScreen = false;
  }
  
  delay(10);
}

void UpdateOnButton(MenuHandle_t* hmenu, SelectedSettings_t* settings)
{
  switch(hmenu->state) 
    {
    case MS_MAIN:
      ScrollMenuHandle_t* hmain;
      hmain = (ScrollMenuHandle_t*)hmenu->page[MS_MAIN].pagehandle;

      if (KnobCounter == MAIN_UV_ON) 
      {
        digitalWrite(UVLED_SW_PIN, HIGH);
        UpdateTime(&UVLED_OnTimer);
        hmenu->state = hmain->itemlist[KnobCounter].destination;
        KNOB_SetCounterRange(0, hmain->num_items-1);
      } 
      else if (KnobCounter == MAIN_SET_TIME) 
      {
        hmenu->state = hmain->itemlist[KnobCounter].destination;
        KnobCounter = settings->on_time;
        KNOB_SetCounterRange(0, 60);
      } else if (KnobCounter == MAIN_MOTOR_SPEED) 
      {
        hmenu->state = hmain->itemlist[KnobCounter].destination;
        KnobCounter = settings->motor_speed;
        KNOB_SetCounterRange(0, 30);
      }
      break;
    case MS_ON_TIME:
      hmenu->state = MS_MAIN;
      settings->on_time = KnobCounter;
      KnobCounter = 0;
      KNOB_SetCounterRange(0, hmain->num_items-1);
      break;
    case MS_MOTOR_SPEED:
      hmenu->state = MS_MAIN;
      settings->motor_speed = KnobCounter;
      KnobCounter = 0;
      KNOB_SetCounterRange(0, hmain->num_items-1);
      break;
    case MS_RUNNING:

      break;
    }
}




void UpdateCounterRange(MenuHandle_t* hmenu)
{
  switch(hmenu->state) 
  {
  case MS_MAIN:
    ScrollMenuHandle_t* hmain;
    hmain = (ScrollMenuHandle_t*)hmenu->page[MS_MAIN].pagehandle;
    KNOB_SetCounterRange(0,hmain->num_items-1);
    break;
  case MS_ON_TIME:
    SettingDecimalHandle_t* hontime;
    hontime = (SettingDecimalHandle_t*)hmenu->page[MS_ON_TIME].pagehandle;
    KNOB_SetCounterRange(hontime->rangemin,hontime->rangemax);
    break;
  case MS_MOTOR_SPEED:
    SettingDecimalHandle_t* hmotspd;
    hmotspd = (SettingDecimalHandle_t*)hmenu->page[MS_MOTOR_SPEED].pagehandle;
    KNOB_SetCounterRange(hmotspd->rangemin,hmotspd->rangemax);
    break;
    case MS_RUNNING:

      break;
  }
}