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
U8G2_SH1106_128X64_NONAME_1_4W_SW_SPI u8g2(U8G2_R0, OLED_CLK, OLED_MOSI, OLED_CS, OLED_DC, OLED_RST);

extern int KnobCounter;

/*Private Function Prototypes****************************/

void DrawScrollMenuPage(ScrollMenuPageHandle_t* hpage, int num_items);
void DrawDecimalVariablePage(DecimalSettingPageHandle_t* hpage);

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

/**
 * @brief Update the OLED with the current menu state. Reads menu
 * handle and chooses which type of page to draw, and uses the 
 * strings configured in hmenu to draw buttons and headers
 * 
 * @param hmenu Menu Handle
*/
void OLED_Update(MenuHandle_t* hmenu) 
{
  //get information of the current page 
  void* pagehandle_ptr = hmenu->page[hmenu->p_index].pagehandle;
  PageType_t pagetype = hmenu->page[hmenu->p_index].pagetype;
  CounterRange_t cRange = hmenu->page[hmenu->p_index].range;

  //choose which page to draw based on page type
  switch(pagetype) 
  {
  case PT_MENU_SCROLL:
    DrawScrollMenuPage((ScrollMenuPageHandle_t*)pagehandle_ptr, cRange.high+1);
    break;
  case PT_SETTING_DECIMAL_RANGE_ADJUST:
    DrawDecimalVariablePage((DecimalSettingPageHandle_t*)pagehandle_ptr);
    break;
  case PT_RUNTIME_STATS:
  case PT_SETTING_OPTION_SELECT:
  default:
    //intentional Fallthrough. Not yet Implemented TODO:IMPLEMENT
    break;
  }
}

/*Private Functions*****************************************************/

/**
 * @brief Draw the current scroll menu page. 
 * 
 * @param hpage Pointer to page handle
 * @param num_items Number of items in the scroll menu
*/
void DrawScrollMenuPage(ScrollMenuPageHandle_t* hpage, int num_items)
{
  static uint8_t firstIndex = 0; 
  int selected[MAX_BUTTONS_ON_SCREEN];

  SetFirstIndex(&firstIndex, num_items, KnobCounter);
  UpdateSelection(selected, num_items, firstIndex, KnobCounter);


  u8g2.firstPage();
  do {
    //Heading
    u8g2.setFont(u8g2_font_courR10_tf);
    u8g2.drawStr(0,15,hpage->header);
    u8g2.drawBox(0,16,u8g2.getDisplayWidth(), 2);

    //Menu Options
    u8g2.setFont(u8g2_font_courR10_tf);
    u8g2.drawButtonUTF8(5, 28, selected[0], u8g2.getDisplayWidth()-5*2,  5,  1, hpage->itemlist[firstIndex].name );
    u8g2.drawButtonUTF8(5, 43, selected[1], u8g2.getDisplayWidth()-5*2,  5,  1, hpage->itemlist[firstIndex+1].name );
    u8g2.drawButtonUTF8(5, 58, selected[2], u8g2.getDisplayWidth()-5*2,  5,  1, hpage->itemlist[firstIndex+2].name );
  } while ( u8g2.nextPage() );
}

/**
 * @brief Draw the current decimal variable menu page. 
 * 
 * @param hpage Pointer to page handle
*/
void DrawDecimalVariablePage(DecimalSettingPageHandle_t* hpage)
{
  sprintf(KnobCounter_str, "%d%s", KnobCounter, hpage->unittxt);

  u8g2.firstPage();
  do {
  //Heading
  u8g2.setFont(u8g2_font_courR10_tf);
  u8g2.drawStr(((u8g2.getDisplayWidth()-u8g2.getStrWidth(hpage->header))/2),15,hpage->header);
  u8g2.drawBox(0,16,u8g2.getDisplayWidth(), 2);

  //Time KnobCounter
  u8g2.drawStr(((u8g2.getDisplayWidth()-u8g2.getStrWidth(KnobCounter_str))/2),40,KnobCounter_str);

  u8g2.drawButtonUTF8((u8g2.getDisplayWidth()-u8g2.getStrWidth(hpage->buttontxt))/2, 60, U8G2_BTN_INV, 25,  25,  1, hpage->buttontxt );
  } while ( u8g2.nextPage() );
}

/**
 * @brief Update the button drawing array so that the correct button is 
 * drawn to be highlighted based on the count.
 * 
 * @param btntype_arr Array of button drawing types
 * @param num_items Number of items in the scrolling array
 * @param findex The index of the top item to be drawn on the screen
 * @param count Current digital encoder knob count
*/
void UpdateSelection(int* btntype_arr, uint8_t num_items, uint8_t findex, int count) 
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
      btntype_arr[i] = U8G2_BTN_INV;
    } else {
      btntype_arr[i] = U8G2_BTN_BW1;
    }
  }
}

/**
 * @brief Sets the index of the top item to draw on the screen on a scrolling page
 * 
 * @param findex Pointer to the variable used to store the index of the top item
 * @param num_items Number of items in the list of items in menu
 * @param count current digital encoder knob count
*/
void SetFirstIndex(uint8_t* findex, int num_items, int count)
{
  do
  {
    if(count < *findex && *findex>0)
    {
      *findex = *findex - 1;
    } 
    else if((count > (MAX_BUTTONS_ON_SCREEN-1 + *findex)) && ((*findex + MAX_BUTTONS_ON_SCREEN)<(num_items)))
    {
      *findex = *findex + 1;
    }
  } while ((count > (MAX_BUTTONS_ON_SCREEN-1 + *findex)) || (count < *findex));
  
} 
