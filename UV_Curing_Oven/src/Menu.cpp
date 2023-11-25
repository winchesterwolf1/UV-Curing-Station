/***************************************************
 *  Implementation of Menu structure
 *
 ***************************************************
 */

#include "Menu.h"
#include "Definitions.h"
#include "Knob.h"
#include "Main.h"

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

ScrollMenuItemHandle_t MainMenu_Items[MAIN_MENU_ITEMS] = {
  { .name = Main_Item1_txt, .destination = MS_MAIN, .callback = MENU_Main_UVOnCallback },
  { .name = Main_Item2_txt, .destination = MS_ON_TIME, .callback = MENU_Main_SetTimeCallback },
  { .name = Main_Item3_txt, .destination = MS_MOTOR_SPEED, .callback = MENU_Main_SetMotorCallback }
};

ScrollMenuPageHandle_t Menu_Main = { 
    .header = LED_OFF,
    .itemlist = MainMenu_Items 
};

/*Time select page Definitions**************************************/
char TimeSel_Header[MAX_BUTTON_CHAR] = "Exposure Time";
char TimeSel_Unit[2] = "s";
char TimeSel_Button[MAX_BUTTON_CHAR] = "OK";

DecimalSettingPageHandle_t Menu_TimeSelect = {
    .header = TimeSel_Header,
    .unittxt = TimeSel_Unit,
    .buttontxt = TimeSel_Button,
    .destination = MS_MAIN,
    .callback = MENU_SetTime_OKCallback
};

/*Motor Speed page Definitions**************************************/
char MotSpd_Header[MAX_BUTTON_CHAR] = "Motor Speed";
char MotSpd_Unit[4] = "rpm";
char MotSpd_Button[MAX_BUTTON_CHAR] = "OK";

DecimalSettingPageHandle_t Menu_MotorSpeed = {
    .header = MotSpd_Header,
    .unittxt = MotSpd_Unit,
    .buttontxt = MotSpd_Button,
    .destination = MS_MAIN,
    .callback = MENU_SetMotor_OKCallback
};

/*Runtime page Definitions**************************************/
char Run_Header[MAX_BUTTON_CHAR] = "Baking...";

RuntimePageHandle_t Menu_Runtime = {
    .header = Run_Header,
    .destination = MS_MAIN,
    .callback = MENU_Runtime_OKCallback
};

/*Menu Structure Handle*********************************************/

MenuPageListHandle_t MenuHandleItems[NUM_MENUS] = {
    { .pagetype = PT_MENU_SCROLL,                   .pagehandle = &Menu_Main,       .range = { .low = 0, .high = MAIN_MENU_ITEMS-1 } },
    { .pagetype = PT_SETTING_DECIMAL_RANGE_ADJUST,  .pagehandle = &Menu_TimeSelect, .range = { .low = 1, .high = 60 } },
    { .pagetype = PT_SETTING_DECIMAL_RANGE_ADJUST,  .pagehandle = &Menu_MotorSpeed, .range = { .low = 1, .high = 30 } },
    { .pagetype = PT_RUNTIME_STATS,                 .pagehandle = &Menu_Runtime,    .range = { .low = 1, .high = 100 } }
};

MenuHandle_t hmenu1 = {.state = MS_MAIN, .page = MenuHandleItems};

PageType_t GetPageType(MenuHandle_t* hmenu);
void* GetPageHandle(MenuHandle_t* hmenu);
void SetNewMenuState(MenuHandle_t* hmenu, MenuState_t state);

void ServiceScrollMenu(MenuHandle_t* hmenu, int sel_counter);
void ServciceDecimalRange(MenuHandle_t* hmenu, int sel_counter);
void ServiceOptionSelect(MenuHandle_t* hmenu, int sel_counter);
void ServiceRuntime(MenuHandle_t* hmenu, int sel_counter);




void MENU_Runtime_OKCallback(int arg)
{
    return; //un-used  callback at the moment
}

void MENU_Init(MenuHandle_t* hmenu)
{
    SetNewMenuState(hmenu, MS_MAIN);
}


void MENU_SetMainStatusMessage(MenuHandle_t* hmenu)
{
    ScrollMenuPageHandle_t* hpage; 
    hpage = (ScrollMenuPageHandle_t*) hmenu->page[MS_MAIN].pagehandle;
    bool currentState = digitalRead(UVLED_SW_PIN);
    
    if(currentState == HIGH) {
        hpage->header = LED_ON; 
    } else {
        hpage->header = LED_OFF;  
    }
}

void MENU_ServiceSelected(MenuHandle_t* hmenu, int sel_counter)
{
    switch (GetPageType(hmenu))
    {
    case PT_MENU_SCROLL:
        ServiceScrollMenu(hmenu, sel_counter);
        break;
    case PT_SETTING_DECIMAL_RANGE_ADJUST:
        ServciceDecimalRange(hmenu, sel_counter);
        break;
    case PT_SETTING_OPTION_SELECT:
        ServiceOptionSelect(hmenu, sel_counter);
        break;
    case PT_RUNTIME_STATS:
        ServiceRuntime(hmenu, sel_counter);
        break;
    }
}

void ServiceScrollMenu(MenuHandle_t* hmenu, int sel_counter)
{
    ScrollMenuPageHandle_t* hpage;
    hpage = (ScrollMenuPageHandle_t*) GetPageHandle(hmenu);

    if(hpage == NULL)
    {
        return;
    }

    if(hpage->itemlist[sel_counter].callback != NULL)
    {
        hpage->itemlist[sel_counter].callback(sel_counter);
    }

    SetNewMenuState(hmenu, hpage->itemlist[sel_counter].destination);
}

void ServciceDecimalRange(MenuHandle_t* hmenu, int sel_counter)
{
    DecimalSettingPageHandle_t* hpage;
    hpage = (DecimalSettingPageHandle_t*) GetPageHandle(hmenu);

    if(hpage == NULL)
    {
        return;
    }

    if(hpage->callback != NULL)
    {
        hpage->callback(sel_counter);
    }

    SetNewMenuState(hmenu, hpage->destination);
}

void ServiceOptionSelect(MenuHandle_t* hmenu, int sel_counter)
{
    OptionSettingPageHandle_t* hpage;
    hpage = (OptionSettingPageHandle_t*) GetPageHandle(hmenu);

    if(hpage == NULL)
    {
        return;
    }

    if(hpage->callback != NULL)
    {
        hpage->callback(hpage->itemlist[sel_counter].val);
    }

    SetNewMenuState(hmenu, hpage->destination);
}

void ServiceRuntime(MenuHandle_t* hmenu, int sel_counter)
{
    RuntimePageHandle_t* hpage;
    hpage = (RuntimePageHandle_t*) GetPageHandle(hmenu);
    
    if(hpage == NULL)
    {
        return;
    }

    if(hpage->callback != NULL)
    {
        hpage->callback(sel_counter);
    }

    SetNewMenuState(hmenu, hpage->destination);
}   


PageType_t GetPageType(MenuHandle_t* hmenu)
{
    return hmenu->page[hmenu->state].pagetype;
}

void* GetPageHandle(MenuHandle_t* hmenu)
{
    return hmenu->page[hmenu->state].pagehandle;
}

void SetNewMenuState(MenuHandle_t* hmenu, MenuState_t state)
{
    hmenu->state = state;
    KNOB_SetCounterRange(hmenu->page[state].range);
}
