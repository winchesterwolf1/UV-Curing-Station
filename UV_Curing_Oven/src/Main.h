/***************************************************
 *  Main header include file
 *
 ***************************************************
 */

#ifndef MAIN_H
#define MAIN_H

/*Exported Defines**********************************/
/*Exported Macros***********************************/
/*Exported Typedefs*********************************/

//struct for the current settings on device
typedef struct {
  unsigned long on_time;
  int motor_speed;
}ApplicationSettings_t;


/*Exported Function Prototypes**********************/

ApplicationSettings_t APP_GetSettings(void);
void APP_SetSetings(ApplicationSettings_t settings_n);



#endif /* MAIN_H */