#ifndef KNOB_H // include guard
#define KNOB_H

#define Key 16 //Push switch Active Low
#define S1 15 //ACW rotation first low
#define S2 14 //ACW rotation second low
#define CW_rot 1

void init_knob();
void updateEncoder();
bool poll_button();




#endif /* KNOB_H */