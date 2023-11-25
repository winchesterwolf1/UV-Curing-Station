#include "Definitions.h"
#include "Menu.h"
#include "Knob.h"
#include "OLED.h"
#include "PWM.h"
#include "Timing.h"
#include "Main.h"


/*Private Defines**********************************/

/*Private Typedef**********************************/




/*Global Variables*********************************/

extern int KnobCounter;
int old_count;
SelectedSettings_t settings;
extern MenuHandle_t hmenu1;
TimerTime_t UVLED_OnTimer = 0;


/*Private Function Prototypes**********************/

void setup(void) 
{
  pinMode(UVLED_SW_PIN, OUTPUT);
  digitalWrite(UVLED_SW_PIN, LOW);
  KNOB_Init();
  OLED_Init();  
  PWM_Init();
  MENU_Init(&hmenu1);
  Serial.begin(9600);
  Serial.write("Starting!\r\n");
}

void loop(void) 
{
  static bool updateScreen = true;
  if(KNOB_PollButtonRelease() == true)
  {
    MENU_ServiceSelected(&hmenu1, KnobCounter);
    MENU_SetMainStatusMessage(&hmenu1);
    updateScreen = true;
  }
  
  if((TimeElapsed_s(&UVLED_OnTimer) >= settings.on_time) && (digitalRead(UVLED_SW_PIN) == HIGH))
  {
    digitalWrite(UVLED_SW_PIN, LOW);
    updateScreen = true;
    MENU_SetMainStatusMessage(&hmenu1);
  }

  if(KnobCounter != old_count) 
  {
    updateScreen = true;
    old_count = KnobCounter;
  }
  

  if(updateScreen) 
  {
    OLED_Update(&hmenu1);
    updateScreen = false;
  }
  
  delay(10);
}

SelectedSettings_t APP_GetSettings(void)
{
  return settings;
}

void APP_SetSetings(SelectedSettings_t settings_n)
{
  settings = settings_n;
}

void MENU_Main_UVOnCallback(int arg)
{
    digitalWrite(UVLED_SW_PIN, HIGH);
    UpdateTime(&UVLED_OnTimer);
}

void MENU_Main_SetTimeCallback(int arg)
{
  KnobCounter = settings.on_time;
}
void MENU_Main_SetMotorCallback(int arg)
{
  KnobCounter = settings.motor_speed;
}
void MENU_SetTime_OKCallback(int arg)
{
  settings.on_time = KnobCounter;
  KnobCounter = 0;
}
void MENU_SetMotor_OKCallback(int arg)
{
  settings.motor_speed = KnobCounter;
  KnobCounter = 0;
}

