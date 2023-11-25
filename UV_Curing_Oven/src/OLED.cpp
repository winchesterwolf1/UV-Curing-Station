/***************************************************
 *  C Implementation of OLED Screen Menu for the
 *  UV Curing Oven
 *
 ***************************************************
 */

#include "OLED.h"
#include "Definitions.h"
#include "Knob.h"
#include "Timing.h"
#include <SPI.h>
#include <U8g2lib.h>

/*Private Defines*******************************************/

/*Private Typedef*******************************************/

/*Global Variables******************************************/
char KnobCounter_str[10];
TimerTime_t OLED_UpdateTimeout = 0;
/* OLED Constructor */
U8G2_SH1106_128X64_NONAME_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);

extern int KnobCounter;

/*Private Function Prototypes****************************/
void SetFirstIndex(uint8_t* findex, int num_items, int count);
void UpdateSelection(int* selected, uint8_t num_items, uint8_t findex, int count);


/*Exported Functions*************************************/

/**
 * @brief Initialisation function for OLED Screen
*/
void OLED_Init() 
{
  u8g2.begin();
  u8g2.setFlipMode(1);
}

void OLED_Update(MenuHandle_t* hmenu) 
{
  if (TimeElapsed_ms(&OLED_UpdateTimeout) > 100)
  {
    UpdateTime(&OLED_UpdateTimeout);
    

    void* pagehandle_ptr = hmenu->page[hmenu->state].pagehandle;
    PageType_t pagetype = hmenu->page[hmenu->state].pagetype;

    switch(pagetype) 
    {
    case PT_MENU_SCROLL:
      ScrollMenuHandle_t* hmenu_scroll;
      hmenu_scroll = (ScrollMenuHandle_t*)pagehandle_ptr;
      static uint8_t firstIndex = 0; 
      int selected[MAX_BUTTONS_ON_SCREEN];

      SetFirstIndex(&firstIndex, hmenu_scroll->num_items, KnobCounter);
      UpdateSelection(selected, hmenu_scroll->num_items, firstIndex, KnobCounter);


      u8g2.firstPage();
      do {
        //Heading
        u8g2.setFont(u8g2_font_courR10_tf);
        u8g2.drawStr(0,15,hmenu_scroll->header);
        u8g2.drawBox(0,16,u8g2.getDisplayWidth(), 2);

        //Menu Options
        u8g2.setFont(u8g2_font_courR10_tf);
        u8g2.drawButtonUTF8(5, 28, selected[0], u8g2.getDisplayWidth()-5*2,  5,  1, hmenu_scroll->itemlist[firstIndex].name );
        u8g2.drawButtonUTF8(5, 43, selected[1], u8g2.getDisplayWidth()-5*2,  5,  1, hmenu_scroll->itemlist[firstIndex+1].name );
        u8g2.drawButtonUTF8(5, 58, selected[2], u8g2.getDisplayWidth()-5*2,  5,  1, hmenu_scroll->itemlist[firstIndex+2].name );
      } while ( u8g2.nextPage() );

      break;


    case PT_SETTING_DECIMAL_RANGE_ADJUST:
      SettingDecimalHandle_t* hopt_dec;
      hopt_dec = (SettingDecimalHandle_t*)pagehandle_ptr;
      sprintf(KnobCounter_str, "%d%s", KnobCounter, hopt_dec->unittxt);


      u8g2.firstPage();
      do {
      //Heading
      u8g2.setFont(u8g2_font_courR10_tf);
      u8g2.drawStr(((u8g2.getDisplayWidth()-u8g2.getStrWidth(hopt_dec->header))/2),15,hopt_dec->header);
      u8g2.drawBox(0,16,u8g2.getDisplayWidth(), 2);
      
      //Time KnobCounter
      u8g2.drawStr(((u8g2.getDisplayWidth()-u8g2.getStrWidth(KnobCounter_str))/2),40,KnobCounter_str);

      u8g2.drawButtonUTF8((u8g2.getDisplayWidth()-u8g2.getStrWidth(hopt_dec->buttontxt))/2, 60, U8G2_BTN_INV, 25,  25,  1, hopt_dec->buttontxt );
      } while ( u8g2.nextPage() );
      
      break;


    case PT_RUNTIME_STATS:
    case PT_SETTING_OPTION_SELECT:
    default:
      //intentional Fallthrough. Not Implemented
      break;
    }
  }  
}

/*Private Functions*****************************************************/

void UpdateSelection(int* selected, uint8_t num_items, uint8_t findex, int count) 
{
  int select = 0;
  if (count <= findex){ 
    select = 0;
  } else if (count >= (findex + MAX_BUTTONS_ON_SCREEN-1)){
    select = MAX_BUTTONS_ON_SCREEN-1;
  } else {
    select = count-findex;
  }
  for(int i = 0; i < MAX_BUTTONS_ON_SCREEN; i++){
    if (i == select) {
      selected[i] = U8G2_BTN_INV;
    } else {
      selected[i] = U8G2_BTN_BW1;
    }
  }
}

void SetFirstIndex(uint8_t* findex, int num_items, int count)
{
  do
  {
    if(count < *findex && *findex>0)
    {
      *findex--;
    } 
    else if((count > (MAX_BUTTONS_ON_SCREEN-1 + *findex)) && ((*findex + MAX_BUTTONS_ON_SCREEN)<(num_items)))
    {
      *findex++;
    }
  } while ((count > (MAX_BUTTONS_ON_SCREEN-1 + *findex)) || (count < *findex));
  
} 
