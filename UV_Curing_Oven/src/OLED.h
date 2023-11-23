#ifndef OLED_H // include guard
#define OLED_H

#define MAX_MENU_SIZE 4
#define MAX_BUTTON_CHAR 14
#define MAX_ON_TIME_S 300

struct Page_details;

void init_OLED();
void update_OLED(int);
void set_KnobCounter_range(int, int);
Page_details get_selection(Page_details);
void set_status_message();

#endif /* OLED_H */