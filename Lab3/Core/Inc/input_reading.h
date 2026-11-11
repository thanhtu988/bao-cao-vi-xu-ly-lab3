/*
 * input_reading.h
 *
 *  Created on: Oct 28, 2026
 */

#ifndef INC_INPUT_READING_H_
#define INC_INPUT_READING_H_

/* we aim to work with more than one buttons: 3 nut cua de bai.
   (Code mau dat hang so nay trong input_reading.c; dua ra header de
    input_processing.c dung chung, khong phai khai bao lai.) */
#define N0_OF_BUTTONS   3

void button_reading(void);
unsigned char is_button_pressed(unsigned char index);
unsigned char is_button_pressed_1s(unsigned char index);

#endif /* INC_INPUT_READING_H_ */
