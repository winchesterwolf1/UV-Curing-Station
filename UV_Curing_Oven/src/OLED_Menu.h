/***************************************************
 *  Header File to configure the Menu
 *
 ***************************************************
 */

#ifndef _MENU_H // include guard to prevent recursion
#define _MENU_H

#include "Definitions.h"
#include "Knob.h"

/*-----------------USER CODE BEGIN-----------------*/

/*Exported Defines**********************************/

#define STATUS_MESSAGE_SIZE MAX_BUTTON_CHAR
#define NUM_PAGES 4


/*Exported Macros***********************************/
/*Exported Typedefs*********************************/

/* Index of all Menu Pages */
typedef enum {
    PI_MAIN,
    PI_ON_TIME,
    PI_MOTOR_SPEED,
    PI_RUNNING,
}PageIndex_t;

enum MainScrollMenuButtonNameIndex{
    MAININDEX_UVON,
    MAININDEX_SET_TIME,
    MAININDEX_SET_MOTOR
};


/*Exported Function Prototypes**********************/

/*Menu Callback Function prototypes*/

void MENU_Main_UVOnCallback(int arg);
void MENU_Main_SetTimeCallback(int arg);
void MENU_Main_SetMotorCallback(int arg);
void MENU_SetTime_OKCallback(int arg);
void MENU_SetMotor_OKCallback(int arg);
void MENU_Runtime_OKCallback(int arg);


/*-----------------USER CODE END-------------------*/


/*Exported Defines**********************************/
/*Exported Macros***********************************/
/*Exported Typedefs*********************************/

/* Index of Implemented Menu Page Types */
typedef enum {
    PT_MENU_SCROLL,                     /* Scrolling Page Menu */
    PT_SETTING_DECIMAL_RANGE_ADJUST,    /* Decimal Variable Adjust Page */
    PT_SETTING_OPTION_SELECT,           /* Option Variable Adjust Page */
    PT_RUNTIME_STATS,                   /* Runtime Stats Screen */
}PageType_t;

/***********PAGE TYPE STRUCTURES BEGIN**************/
/**** PT_MENU_SCROLL Page Type Structures ****/
typedef struct 
{
    char* name;                 //name string to be shown on the button in the menu
    PageIndex_t destination;    //desination page if clicked
    void (*callback)(int arg);  //callback function for when clicked (arg will be button index value(counter))
}ScrollMenuItemHandle_t;

typedef struct 
{
    char* header;                       //Menu Header string
    ScrollMenuItemHandle_t* itemlist;   //list of items to show in the scroll menu
}ScrollMenuPageHandle_t;

/**** PT_SETTING_DECIMAL_RANGE_ADJUST Page Type Structures ****/
typedef struct 
{
    char* header;               //header string to show on top of page
    char* unittxt;              //string of unit to be shown after the variable count
    char* buttontxt;            //string of text to be shown on the accept button
    PageIndex_t destination;    //destination after clicking button
    void (*callback)(int arg);  //callback function to do something with the value shown on screen (arg = counter)
}DecimalSettingPageHandle_t;

/**** PT_SETTING_OPTION_SELECT Page Structures ****/
typedef struct 
{
    char* name;     //name of the option to be put into the variable
    int val;        //value of the option to be put into the variable
}OptionSettingItemHandle_t;

typedef struct 
{
    char* header;                           //header string to show at top of page
    OptionSettingItemHandle_t* itemlist;    //list of options to be shown
    PageIndex_t destination;                //destination after clicking OK
    void (*callback)(int arg);              //callback function to do something with variable (arg wil be the value of the option selected)
}OptionSettingPageHandle_t;

/**** PT_RUNTIME_STATS Page Structures ****/
typedef struct 
{   //To be implemented, Placeholder Struct
    char* header;                           
    PageIndex_t destination;
    void (*callback)(int arg);
}RuntimePageHandle_t;

/************PAGE TYPE STRUCTURES END***************/

/****FULL MENU HANDLE STRUCTURES****/
/* Page List Item Strtucture */
typedef struct 
{
    PageType_t pagetype;    //Page type of the pagehandle in the list
    void* pagehandle;       //void pointer to page handle. needs to be cast to correct structure based on page type
    CounterRange_t range;   //valid range of the counter for this page 
} MenuPageListHandle_t;

typedef struct
{
    PageIndex_t p_index;          //current index of the menu (indicates which page to show)
    MenuPageListHandle_t* page; //list of pages
}MenuHandle_t;


/*Exported Function Prototypes**********************/

void MENU_SetMainStatusMessage(MenuHandle_t* hmenu);
void MENU_ServiceSelected(MenuHandle_t* hmenu, int sel_counter);
void MENU_Init(MenuHandle_t* hmenu);

#endif /*_MENU_H*/
