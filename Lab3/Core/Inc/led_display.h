#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

#define MAX_LED        4

#define COLOR_RED      0
#define COLOR_AMBER    1
#define COLOR_GREEN    2

#define ROAD1          0
#define ROAD2          1

extern int led_buffer[MAX_LED];

void display7SEG(int num);
void update7SEG(int index);
void update7SEGBuffer(int road1_value, int road2_value);

void setTrafficLight(int road, int color);
void clearAllTrafficLights(void);
void toggleTrafficColor(int color);

#endif
