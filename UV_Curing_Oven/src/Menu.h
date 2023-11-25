/***************************************************
 *  Header File to configure the Menu
 *
 ***************************************************
 */

#ifndef _MENU_H // include guard to prevent recursion
#define _MENU_H

#include "Definitions.h"

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
}ScrollMenuItem_t;

typedef struct 
{
    char* header;
    int num_items;
    ScrollMenuItem_t* itemlist;
}ScrollMenuHandle_t;

/* Setting Decimal Page Structures */
typedef struct 
{
    char* header;
    int rangemin;
    int rangemax;
    char* unittxt;
    char* buttontxt;
}SettingDecimalHandle_t;

/* Setting Option Page Structures */
typedef struct 
{
    char* name;
    int val;
}SettingOptionItem_t;

typedef struct 
{
    char* header;
    SettingOptionItem_t* itemlist;
}SettingOptionHandle_t;

/* Runtime Screen Structures */

/* Menu Structure Item Strtucture */
typedef struct 
{
    PageType_t pagetype;
    void* pagehandle;
}MenuHandleItems_t;

typedef struct
{
    MenuState_t state;
    MenuHandleItems_t* page;
}MenuHandle_t;


enum MainMenu_Items{
    MAIN_UV_ON,
    MAIN_SET_TIME,
    MAIN_MOTOR_SPEED
};

void MENU_SetMainStatusMessage(MenuHandle_t* hmenu);

#endif /*_MENU_H*/
