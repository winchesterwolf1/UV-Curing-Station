/***************************************************
 *  Main Application File
 *
 ***************************************************
 */

#include "Definitions.h"
#include "OLED_Menu.h"
#include "Knob.h"
#include "OLED.h"
#include "PWM.h"
#include "Timing.h"
#include "Main.h"


/*Private Defines************************************/
/*Private Macros*************************************/

#define _CONVERT_TO_DUTY(spd) ((float)(spd * 3))


/*Private Typedef************************************/
/*Global Variables***********************************/

extern int KnobCounter;
int old_count;
ApplicationSettings_t ApplicationSetings = {1, 1};
extern MenuHandle_t hmenu1;
TimerTime_t UVLED_OnTimer = 0;


/*Private Function Prototypes************************/
/*Callback Functions*********************************/

/**
 * @brief Callback function for the UV On button in the Main menu.
 * Turns on UVLED and updates UVLED timer context
 * 
 * @param arg - unused
*/
void MENU_Main_UVOnCallback(int arg)
{
    digitalWrite(UVLED_SW_PIN, HIGH);
    PWM_Update(PWM_MOTOR_PIN, _CONVERT_TO_DUTY(ApplicationSetings.motor_speed));
    TIM_UpdateTime(&UVLED_OnTimer);
}

/**
 * @brief Callback function for the Set Time button in the main menu.
 * Sets Encoder Knob counter to the on_time saved in the application
 * so that the user can see what the setting currently is.
*/
void MENU_Main_SetTimeCallback(int arg)
{
  KnobCounter = ApplicationSetings.on_time;
}

/**
 * @brief Callback function for the Motor Speed button in the main menu.
 * Sets Encoder Knob counter to the motor_speed saved in the application
 * so that the user can see what the setting currently is.
*/
void MENU_Main_SetMotorCallback(int arg)
{
  KnobCounter = ApplicationSetings.motor_speed;
}

/**
 * @brief Callback function for accpting the configured setting in the
 * set time page. sets the application on_time setting to current encoder knob counter
 * value, and changes the counter to the index of this page in the main menu
 * so that when the user goes back to that page, they are still on the option
 * they had selected.
*/
void MENU_SetTime_OKCallback(int arg)
{
  ApplicationSetings.on_time = arg;
  KnobCounter = MAININDEX_SET_TIME;
}

/**
 * @brief Callback function for accpting the configured setting in the
 * motor speed page. sets the application motor_speed setting to current encoder knob counter
 * value, and changes the counter to the index of this page in the main menu
 * so that when the user goes back to that page, they are still on the option
 * they had selected.
*/
void MENU_SetMotor_OKCallback(int arg)
{
  ApplicationSetings.motor_speed = arg;
  KnobCounter = MAININDEX_SET_MOTOR;
}


/*Exported Functions*********************************/

/**
 * @brief Setup function for application
*/
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
  tone(SPEAKER_PIN,2000,1000);
}

/**
 * @brief Recurring loop function for application
*/
void loop(void) 
{
  static bool updateScreen = true;
  //Poll button for if it has been released. When it gets released update the menu based on current KnobCounter
  if(KNOB_PollButtonRelease() == true)
  {
    MENU_ServiceSelected(&hmenu1, KnobCounter);
    MENU_SetMainStatusMessage(&hmenu1);
    tone(SPEAKER_PIN,1750,30);
    updateScreen = true;
  }
  
  //Check the elapsed time for how long the UVLED has been on for.
  if((TIM_TimeElapsed_s(&UVLED_OnTimer) >= ApplicationSetings.on_time) && (digitalRead(UVLED_SW_PIN) == HIGH))
  {
    digitalWrite(UVLED_SW_PIN, LOW);
    PWM_Update(PWM_MOTOR_PIN, 0);
    updateScreen = true;
    MENU_SetMainStatusMessage(&hmenu1);
  }
  
  //Update screen if KnobCounter changes
  if(KnobCounter != old_count) 
  {
    updateScreen = true;
    old_count = KnobCounter;
  }
  
  //if we need to updae screen, do so now.
  if(updateScreen) 
  {
    OLED_Update(&hmenu1);
    updateScreen = false;
  }
}

/**
 * @brief Get Current settings for the application
 * 
 * @return the structure of the current application settings
*/
ApplicationSettings_t APP_GetSettings(void)
{
  return ApplicationSetings;
}

/**
 * @brief Set the current settings for the application
 * 
 * @param settings_n New settings to be saved
*/
void APP_SetSetings(ApplicationSettings_t settings)
{
  ApplicationSetings = settings;
}

/*Private Functions**********************************/
