/***************************************************
 *  Header File to configure the Menu
 *
 ***************************************************
 */

#ifndef _MENU_H // include guard to prevent recursion
#define _MENU_H

#include "Definitions.h"
#include "Knob.h"

#define STATUS_MESSAGE_SIZE MAX_BUTTON_CHAR
#define NUM_MENUS 4

typedef enum {
    //menu page structure
    MS_MAIN,
    MS_ON_TIME,
    MS_MOTOR_SPEED,
    MS_RUNNING,
}MenuState_t;

typedef enum {
    PT_MENU_SCROLL,
    PT_SETTING_DECIMAL_RANGE_ADJUST,
    PT_SETTING_OPTION_SELECT,
    PT_RUNTIME_STATS,
}PageType_t;

/* Scroll Menu Structures */
typedef struct 
{
    char* name;
    MenuState_t destination;
    void (*callback)(int arg);
}ScrollMenuItemHandle_t;

typedef struct 
{
    char* header;
    ScrollMenuItemHandle_t* itemlist;
}ScrollMenuPageHandle_t;

/* Setting Decimal Page Structures */
typedef struct 
{
    char* header;
    char* unittxt;
    char* buttontxt;
    MenuState_t destination;
    void (*callback)(int arg);
}DecimalSettingPageHandle_t;

/* Setting Option Page Structures */
typedef struct 
{
    char* name;
    int val;
}OptionSettingItemHandle_t;

typedef struct 
{
    char* header;
    OptionSettingItemHandle_t* itemlist;
    MenuState_t destination;
    void (*callback)(int arg);
}OptionSettingPageHandle_t;

/* Runtime Screen Structures */

typedef struct 
{
    char* header;
    MenuState_t destination;
    void (*callback)(int arg);
}RuntimePageHandle_t;

/* Menu Structure Item Strtucture */
typedef struct 
{
    PageType_t pagetype;
    void* pagehandle;
    CounterRange_t range;
} MenuPageListHandle_t;

typedef struct
{
    MenuState_t state;
    MenuPageListHandle_t* page;
}MenuHandle_t;


/*Menu Callback Function prototypes*/
void MENU_Main_UVOnCallback(int arg);
void MENU_Main_SetTimeCallback(int arg);
void MENU_Main_SetMotorCallback(int arg);
void MENU_SetTime_OKCallback(int arg);
void MENU_SetMotor_OKCallback(int arg);
void MENU_Runtime_OKCallback(int arg);

/*Exported Menu Functon prototypes*/
void MENU_SetMainStatusMessage(MenuHandle_t* hmenu);
void MENU_ServiceSelected(MenuHandle_t* hmenu, int sel_counter);
void MENU_Init(MenuHandle_t* hmenu);

#endif /*_MENU_H*/
