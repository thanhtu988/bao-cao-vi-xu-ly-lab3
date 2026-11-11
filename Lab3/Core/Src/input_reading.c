/*
 * input_reading.c
 *
 *  Created on: Oct 28, 2026
 */

#include "main.h"
#include "timer.h"
#include "input_reading.h"

// timer interrupt duration is TIMER_CYCLE ms, so to pass 1 second,
// we need to jump to the interrupt service routine 1000 / TIMER_CYCLE times
// (Exercise 4: code mau ghi cung 100, chi dung khi ngat dung 10 ms)
#define DURATION_FOR_AUTO_INCREASING    (1000 / TIMER_CYCLE)
#define BUTTON_IS_PRESSED               GPIO_PIN_RESET
#define BUTTON_IS_RELEASED              GPIO_PIN_SET

// chan cua tung nut (code mau chi doc BUTTON_1 cho moi i)
static GPIO_TypeDef * const buttonPort[N0_OF_BUTTONS] = {
  BUTTON_1_GPIO_Port, BUTTON_2_GPIO_Port, BUTTON_3_GPIO_Port
};
static const uint16_t buttonPin[N0_OF_BUTTONS] = {
  BUTTON_1_Pin, BUTTON_2_Pin, BUTTON_3_Pin
};

// the buffer that the final result is stored after debouncing
// (khoi tao = RELEASED: neu de mac dinh 0 = GPIO_PIN_RESET thi ngay luc
//  khoi dong ca 3 nut deu bi xem la "dang nhan")
static GPIO_PinState buttonBuffer[N0_OF_BUTTONS] = {
  BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED
};
// we define two buffers for debouncing
static GPIO_PinState debounceButtonBuffer1[N0_OF_BUTTONS] = {
  BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED
};
static GPIO_PinState debounceButtonBuffer2[N0_OF_BUTTONS] = {
  BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED
};
// we define a flag for a button pressed more than 1 second.
static uint8_t flagForButtonPress1s[N0_OF_BUTTONS];
// we define counter for automatically increasing the value
// after the button is pressed more than 1 second.
static uint16_t counterForButtonPress1s[N0_OF_BUTTONS];

void button_reading(void)
{
  for (uint8_t i = 0; i < N0_OF_BUTTONS; i++) {
    debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
    debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(buttonPort[i], buttonPin[i]);
    if (debounceButtonBuffer1[i] == debounceButtonBuffer2[i])
      buttonBuffer[i] = debounceButtonBuffer1[i];
    if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
      // if a button is pressed, we start counting
      if (counterForButtonPress1s[i] < DURATION_FOR_AUTO_INCREASING) {
        counterForButtonPress1s[i]++;
      } else {
        // the flag is turned on when 1 second has passed
        // since the button is pressed.
        flagForButtonPress1s[i] = 1;
      }
    } else {
      counterForButtonPress1s[i] = 0;
      flagForButtonPress1s[i] = 0;
    }
  }
}

unsigned char is_button_pressed(unsigned char index)
{
  if (index >= N0_OF_BUTTONS) return 0;
  return (buttonBuffer[index] == BUTTON_IS_PRESSED);
}

unsigned char is_button_pressed_1s(unsigned char index)
{
  if (index >= N0_OF_BUTTONS) return 0xff;
  return (flagForButtonPress1s[index] == 1);
}
