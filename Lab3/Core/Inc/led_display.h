/*
 * led_display.h
 *
 *  Created on: Oct 28, 2026
 */

#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

#define MAX_LED        4      /* 4 LED 7 doan */

/* 3 mau den */
#define COLOR_RED      0
#define COLOR_AMBER    1
#define COLOR_GREEN    2

/* 2 tuyen duong */
#define ROAD1          0      /* Bac - Nam  : LED 7 doan EN0, EN1 */
#define ROAD2          1      /* Dong - Tay : LED 7 doan EN2, EN3 */

extern int led_buffer[MAX_LED];   /* bo dem dau ra cua 4 LED 7 doan */

/* LED 7 doan */
void display7SEG(int num);
void update7SEG(int index);
void update7SEGBuffer(int road1_value, int road2_value);

/* 12 den giao thong */
void setTrafficLight(int road, int color);
void clearAllTrafficLights(void);
void toggleTrafficColor(int color);

#endif /* INC_LED_DISPLAY_H_ */
