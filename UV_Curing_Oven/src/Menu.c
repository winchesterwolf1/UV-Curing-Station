/***************************************************
 *  Implementation of Menu structure
 *
 ***************************************************
 */

#include "Menu.h"
#include "Definitions.h"

/*
typedef struct {
    PageType_t MainMenu = PT_MENU_SCROLL;
        PageType_t TimeSel = PT_SETTING_DECIMAL_RANGE_ADJUST;
        PageType_t MotorSpdSel = PT_SETTING_DECIMAL_RANGE_ADJUST;
    PageType_t RunStats = PT_RUNTIME_STATS;
}MenuStructure_t;
*/

/*Main Menu Definitions**************************************/
#define MAIN_MENU_ITEMS 3
/* UV LED Status Message */
char LED_ON[STATUS_MESSAGE_SIZE] = "UV LED ON!";
char LED_OFF[STATUS_MESSAGE_SIZE] = "UV LED OFF";

/* Main Menu Items */
char Main_Item1_txt[MAX_BUTTON_CHAR] = "UV On";
char Main_Item2_txt[MAX_BUTTON_CHAR] = "Set Time";
char Main_Item3_txt[MAX_BUTTON_CHAR] = "Motor Speed";

ScrollMenuItem_t MainMenu_Items[MAIN_MENU_ITEMS] = {
  { .name = Main_Item1_txt, .destination = MS_MAIN },
  { .name = Main_Item2_txt, .destination = MS_ON_TIME },
  { .name = Main_Item3_txt, .destination = MS_MOTOR_SPEED }
};

ScrollMenuHandle_t Menu_Main = { 
    .header = LED_OFF,
    .num_items = MAIN_MENU_ITEMS,
    .itemlist = MainMenu_Items 
};

/*Time select page Definitions**************************************/
char TimeSel_Header[MAX_BUTTON_CHAR] = "Exposure Time";
char TimeSel_Unit[2] = "s";
char TimeSel_Button[MAX_BUTTON_CHAR] = "OK";

SettingDecimalHandle_t Menu_TimeSelect = {
    .header = TimeSel_Header,
    .rangemin = 1,
    .rangemax = 60,
    .unittxt = TimeSel_Unit,
    .buttontxt = TimeSel_Button
};

/*Motor Speed page Definitions**************************************/
char MotSpd_Header[MAX_BUTTON_CHAR] = "Motor Speed";
char MotSpd_Unit[4] = "rpm";
char MotSpd_Button[MAX_BUTTON_CHAR] = "OK";

SettingDecimalHandle_t Menu_MotorSpeed = {
    .header = MotSpd_Header,
    .rangemin = 1,
    .rangemax = 30,
    .unittxt = MotSpd_Unit,
    .buttontxt = MotSpd_Button
};

/*Motor Speed page Definitions**************************************/
char Run_Header[MAX_BUTTON_CHAR] = "Baking...";



/*Menu Structure Handle*********************************************/

MenuHandleItems_t MenuHandleItems[NUM_MENUS] = {
    { .pagetype = PT_MENU_SCROLL,                   .pagehandle = &Menu_Main },
    { .pagetype = PT_SETTING_DECIMAL_RANGE_ADJUST,  .pagehandle = &Menu_TimeSelect },
    { .pagetype = PT_SETTING_DECIMAL_RANGE_ADJUST,  .pagehandle = &Menu_MotorSpeed },
    { .pagetype = PT_RUNTIME_STATS,                 .pagehandle = NULL }
};

MenuHandle_t hmenu = {.page = MenuHandleItems};

void MENU_SetMainStatusMessage(MenuHandle_t* hmenu)
{
    ScrollMenuHandle_t* hmainmenu = hmenu->page[MS_MAIN].pagehandle;
    bool currentState = digitalRead(UVLED_SW_PIN);
    
    if(currentState == HIGH) {
        hmainmenu->header = LED_ON; 
    } else {
        hmainmenu->header = LED_OFF;  
    }
}
