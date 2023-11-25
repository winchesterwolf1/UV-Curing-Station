/***************************************************
 *  Main header include file
 *
 ***************************************************
 */

#ifndef MAIN_H
#define MAIN_H

typedef struct {
  unsigned long on_time;
  int motor_speed;
}SelectedSettings_t;

SelectedSettings_t APP_GetSettings(void);
void APP_SetSetings(SelectedSettings_t settings_n);



#endif /* MAIN_H */