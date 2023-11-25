/***************************************************
 *  Implementation of Menu structure
 *
 ***************************************************
 */

#include "Menu.h"
#include "Definitions.h"
#include "Knob.h"
#include "Main.h"

/*Private Defines************************************/
#define MAIN_MENU_ITEMS 3


/*Private Macros*************************************/
/*Private Typedef************************************/
/*Global Variables***********************************/

/*-------- Main Scroll Menu Definitions --------*/
/* UV LED Status Message */
char LED_ON[STATUS_MESSAGE_SIZE] = "UV LED ON!";
char LED_OFF[STATUS_MESSAGE_SIZE] = "UV LED OFF";

/* Main Menu Items */
char Main_Item1_txt[MAX_BUTTON_CHAR] = "UV On";
char Main_Item2_txt[MAX_BUTTON_CHAR] = "Set Time";
char Main_Item3_txt[MAX_BUTTON_CHAR] = "Motor Speed";

ScrollMenuItemHandle_t MainMenu_Items[MAIN_MENU_ITEMS] = {
  { .name = Main_Item1_txt, .destination = PI_MAIN,         .callback = MENU_Main_UVOnCallback      },
  { .name = Main_Item2_txt, .destination = PI_ON_TIME,      .callback = MENU_Main_SetTimeCallback   },
  { .name = Main_Item3_txt, .destination = PI_MOTOR_SPEED,  .callback = MENU_Main_SetMotorCallback  }
};

ScrollMenuPageHandle_t Menu_Main = { 
    .header = LED_OFF,
    .itemlist = MainMenu_Items 
};

/*-------- Time select page Definitions --------*/
char TimeSel_Header[MAX_BUTTON_CHAR] = "Exposure Time";
char TimeSel_Unit[2] = "s";
char TimeSel_Button[MAX_BUTTON_CHAR] = "OK";

DecimalSettingPageHandle_t Menu_TimeSelect = {
    .header = TimeSel_Header,
    .unittxt = TimeSel_Unit,
    .buttontxt = TimeSel_Button,
    .destination = PI_MAIN,
    .callback = MENU_SetTime_OKCallback
};

/*-------- Motor Speed page Definitions --------*/
char MotSpd_Header[MAX_BUTTON_CHAR] = "Motor Speed";
char MotSpd_Unit[4] = "rpm";
char MotSpd_Button[MAX_BUTTON_CHAR] = "OK";

DecimalSettingPageHandle_t Menu_MotorSpeed = {
    .header = MotSpd_Header,
    .unittxt = MotSpd_Unit,
    .buttontxt = MotSpd_Button,
    .destination = PI_MAIN,
    .callback = MENU_SetMotor_OKCallback
};

/*-------- Runtime page Definitions --------*/
char Run_Header[MAX_BUTTON_CHAR] = "Baking...";

RuntimePageHandle_t Menu_Runtime = {
    .header = Run_Header,
    .destination = PI_MAIN,
    .callback = MENU_Runtime_OKCallback
};

/*-------- Menu Structure Handle --------*/
MenuPageListHandle_t MenuHandleItems[NUM_PAGES] = {
    { .pagetype = PT_MENU_SCROLL,                   .pagehandle = &Menu_Main,       .range = { .low = 0, .high = MAIN_MENU_ITEMS-1 }},
    { .pagetype = PT_SETTING_DECIMAL_RANGE_ADJUST,  .pagehandle = &Menu_TimeSelect, .range = { .low = 1, .high = 60 }               },
    { .pagetype = PT_SETTING_DECIMAL_RANGE_ADJUST,  .pagehandle = &Menu_MotorSpeed, .range = { .low = 1, .high = 30 }               },
    { .pagetype = PT_RUNTIME_STATS,                 .pagehandle = &Menu_Runtime,    .range = { .low = 1, .high = 100 }              }
};

MenuHandle_t hmenu1 = { .p_index = PI_MAIN, .page = MenuHandleItems };


/*Private Function Prototypes************************/

PageType_t GetPageType(MenuHandle_t* hmenu);
void* GetPageHandle(MenuHandle_t* hmenu);
void SetNewMenuState(MenuHandle_t* hmenu, PageIndex_t p_index);

void ServiceScrollMenu(MenuHandle_t* hmenu, int sel_counter);
void ServciceDecimalRange(MenuHandle_t* hmenu, int value);
void ServiceOptionSelect(MenuHandle_t* hmenu, int sel_counter);
void ServiceRuntime(MenuHandle_t* hmenu, int sel_counter);


/*Callback Functions*********************************/

/**
 * @brief Callback Function for when the OK button is pressed on the Runtime screen
*/
void MENU_Runtime_OKCallback(int arg)
{
    return; //un-used  callback at the moment
}


/*Exported Functions*********************************/

/**
 * @brief Initialisation Function for the Menu Variables.
 * Sets the starting page of the screen and knob counter range
 * 
 * @param hmenu Menu Handle to be initalised
*/
void MENU_Init(MenuHandle_t* hmenu)
{
    SetNewMenuState(hmenu, PI_MAIN);
}

/**
 * @brief Sets the status message of the main menu for when the UVLED is on or off.
 * @note Will stop using eventually once runtime stats page is up and running
*/
void MENU_SetMainStatusMessage(MenuHandle_t* hmenu)
{
    ScrollMenuPageHandle_t* hpage; 
    hpage = (ScrollMenuPageHandle_t*) hmenu->page[PI_MAIN].pagehandle;
    bool currentState = digitalRead(UVLED_SW_PIN);
    
    if(currentState == HIGH) {
        hpage->header = LED_ON; 
    } else {
        hpage->header = LED_OFF;  
    }
}

/**
 * @brief Service the selected option on the current page.
 * Automatically sets counter range, the new current page index, and calls
 * the callback function associated with the selected option on the
 * current page.
 * 
 * @param hmenu Menu Handle to be serviced
 * @param sel_counter Current counter value of encoder knob
*/
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


/*Private Functions**********************************/

/**
 * @brief Service function of the scroll menu page type.
 * Calls callback function of selected menu option,
 * sets new current page index and valid counter range for encoder knob.
 * 
 * @param hmenu Menu Handle
 * @param sel_counter Current digital encoder knob value
*/
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

/**
 * @brief Service function of the Decimal variable page type.
 * Calls callback function of page, sets new current page index
 * and valid counter range for encoder knob.
 * 
 * @param hmenu Menu Handle
 * @param value Current digital encoder knob value
*/
void ServciceDecimalRange(MenuHandle_t* hmenu, int value)
{
    DecimalSettingPageHandle_t* hpage;
    hpage = (DecimalSettingPageHandle_t*) GetPageHandle(hmenu);

    if(hpage == NULL)
    {
        return;
    }

    if(hpage->callback != NULL)
    {
        hpage->callback(value);
    }

    SetNewMenuState(hmenu, hpage->destination);
}

/**
 * @brief Service function of the option variable page type.
 * Calls callback function of selected variable option,
 * sets new current page index and valid counter range for encoder knob.
 * 
 * @param hmenu Menu Handle
 * @param sel_counter Current digital encoder knob value
*/
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

/**
 * @brief Service function of the runtime page.
 * Calls callback function of page, sets new current page index 
 * and valid counter range for encoder knob.
 * 
 * @param hmenu Menu Handle
 * @param sel_counter Current digital encoder knob value
*/
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

/**
 * @brief get the page type of the currently active page
 * 
 * @param hmenu Menu Handle
 * @return Page type of current page
*/
PageType_t GetPageType(MenuHandle_t* hmenu)
{
    return hmenu->page[hmenu->p_index].pagetype;
}

/**
 * @brief get the address of the selected page handle initialisation struct
 * 
 * @param hmenu Menu Handle
 * @return Pointer to current page handle (requires casting)
*/
void* GetPageHandle(MenuHandle_t* hmenu)
{
    return hmenu->page[hmenu->p_index].pagehandle;
}

/**
 * @brief Sets the new page index and valid encoder knob counter range for selected page
 * 
 * @param hmenu Menu Handle
 * @param p_index The page index to switch to 
*/
void SetNewMenuState(MenuHandle_t* hmenu, PageIndex_t p_index)
{
    hmenu->p_index = p_index;
    KNOB_SetCounterRange(hmenu->page[p_index].range);
}
