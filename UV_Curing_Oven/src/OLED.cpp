#include "OLED.h"
#include "Definitions.h"
#include <SPI.h>
#include <U8g2lib.h>

static unsigned long last_update = 0;
extern int counter;
extern char message[];
/* OLED Constructor */
U8G2_SH1106_128X64_NONAME_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
int menu_selected[MAX_MENU_SIZE];

char status_message[14] = "UV LED OFF";
char LED_ON[14] = "UV LED ON!";
char LED_OFF[14] = "UV LED OFF";
int first_index = 0;


char a[14] = "UV On";
char b[14] = "UV Off";
char c[14] = "Set Time";
char d[14] = "UV Brightness";
char e[14] = "Motor Speed";
char f[14] = "Useless";
char *Button_List[6] = {a,b,c,d,e,f};

struct Page_details {
  int options_os;
  int options_tot;
  int selected[MAX_MENU_SIZE];
  int first_index;
};

void init_OLED() {
  u8g2.begin();
  u8g2.setFlipMode(1);
}

Page_details get_selection(Page_details menu) {
  int select = 0;
  if (counter <= first_index){ 
    select = 0;
  } else if (counter >= menu.options_tot-1){
    select = menu.options_os-1;
  } else {
    select = counter-first_index;
  }
  for(int i = 0; i < menu.options_os; i++){
    if (i == select) {
      menu.selected[i] = U8G2_BTN_INV;
    } else {
      menu.selected[i] = U8G2_BTN_BW1;
    }
  }
  return menu;
}

void set_counter_range(int low, int high){
  if (counter <= low){ 
    counter = low;
  } else if (counter >= high){
    counter = high;
  }
}

void set_status_message(){
  if(PIND & (1<<PD5)){
    for(int i = 0; i<=13; i++){
      status_message[i] = LED_ON[i];
    }
  } else {
    for(int i = 0; i<=10; i++){
      status_message[i] = LED_OFF[i];
    }
  }
}

void set_first_index(int screen, int options){
  if(counter < first_index && first_index>0){
    first_index--;
  } else if(counter >= screen+first_index && (first_index+screen)<(options)){
    first_index++;
  }
} 

void update_OLED(int menu_state) {
  if (millis()-last_update > 100){

    last_update = millis();
    set_status_message();
    Page_details menu;
    menu.options_os = 0;
    menu.options_tot = 0;
    char char_counter[5];

    switch(menu_state) {
      case 0:
        menu.options_os = 3;
        menu.options_tot = 6;
        set_first_index(menu.options_os, menu.options_tot);
        set_counter_range(0,menu.options_tot-1);
        menu = get_selection(menu);
      case 1:
        menu.options_os = 0;
        menu.options_tot = 0;
        set_counter_range(0,MAX_ON_TIME_S);
    }
    
    sprintf(char_counter, "%d", counter);
    
    
    switch(menu_state) {
    case 0:
      u8g2.firstPage();
      do {
        //Heading
        u8g2.setFont(u8g2_font_courR10_tf);
        u8g2.drawStr(0,15,status_message);
        u8g2.drawBox(0,16,u8g2.getDisplayWidth(), 2);

        //Menu Options
        u8g2.setFont(u8g2_font_courR10_tf);
        u8g2.drawButtonUTF8(5, 28, menu.selected[0], u8g2.getDisplayWidth()-5*2,  5,  1, Button_List[first_index] );
        u8g2.drawButtonUTF8(5, 43, menu.selected[1], u8g2.getDisplayWidth()-5*2,  5,  1, Button_List[first_index+1] );
        u8g2.drawButtonUTF8(5, 58, menu.selected[2], u8g2.getDisplayWidth()-5*2,  5,  1, Button_List[first_index+2] );
      } while ( u8g2.nextPage() );
      break;
    case 1:
      u8g2.firstPage();
      do {
      //Heading
      u8g2.setFont(u8g2_font_courR10_tf);
      u8g2.drawStr(((u8g2.getDisplayWidth()-u8g2.getStrWidth("Exposure Time"))/2),15,"Exposure Time");
      u8g2.drawBox(0,16,u8g2.getDisplayWidth(), 2);
      
      //Time counter
      u8g2.drawStr(((u8g2.getDisplayWidth()-u8g2.getStrWidth(char_counter))/2),40,char_counter);

      u8g2.drawButtonUTF8((u8g2.getDisplayWidth()-u8g2.getStrWidth("OK"))/2, 60, U8G2_BTN_INV, 25,  25,  1, "OK" );
      } while ( u8g2.nextPage() );
      break;
    case 2:
      u8g2.firstPage();
      do {
      //Heading
      u8g2.setFont(u8g2_font_courR10_tf);
      u8g2.drawStr(((u8g2.getDisplayWidth()-u8g2.getStrWidth("UV Brightness"))/2),15,"UV Brightness");
      u8g2.drawBox(0,16,u8g2.getDisplayWidth(), 2);
      
      //Time counter
      u8g2.drawStr(((u8g2.getDisplayWidth()-u8g2.getStrWidth(char_counter))/2),40,char_counter);

      u8g2.drawButtonUTF8((u8g2.getDisplayWidth()-u8g2.getStrWidth("OK"))/2, 60, U8G2_BTN_INV, 25,  25,  1, "OK" );
      } while ( u8g2.nextPage() );
      break;
    }
  }  
}