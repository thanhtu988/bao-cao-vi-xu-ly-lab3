#include "main.h"
#include "input_reading.h"
#include "led_display.h"
#include "timer.h"

#define BUTTON_MODE       0
#define BUTTON_INC        1
#define BUTTON_SET        2

#define ONE_SECOND        1000
#define BLINK_DURATION    250
#define SCAN_DURATION     (1000 / MAX_LED)

#define MIN_DURATION      1
#define MAX_DURATION      99

enum ButtonState { BUTTON_RELEASED, BUTTON_PRESSED, BUTTON_PRESSED_MORE_THAN_1_SECOND };
enum ButtonState buttonState[N0_OF_BUTTONS] = {
  BUTTON_RELEASED, BUTTON_RELEASED, BUTTON_RELEASED
};

static unsigned char fsm_for_button(unsigned char index)
{
  unsigned char pressed = 0;

  switch (buttonState[index]) {
  case BUTTON_RELEASED:
    if (is_button_pressed(index)) {
      buttonState[index] = BUTTON_PRESSED;
      pressed = 1;
    }
    break;
  case BUTTON_PRESSED:
    if (!is_button_pressed(index)) {
      buttonState[index] = BUTTON_RELEASED;
    } else {
      if (is_button_pressed_1s(index)) {
        buttonState[index] = BUTTON_PRESSED_MORE_THAN_1_SECOND;
      }
    }
    break;
  case BUTTON_PRESSED_MORE_THAN_1_SECOND:
    if (!is_button_pressed(index)) {
      buttonState[index] = BUTTON_RELEASED;
    }
    break;
  }
  return pressed;
}

int duration_red   = 5;
int duration_amber = 2;
int duration_green = 3;

static int light_color[2];
static int light_counter[2];

static void traffic_start(void)
{
  light_color[ROAD1]   = COLOR_RED;
  light_counter[ROAD1] = duration_red;
  light_color[ROAD2]   = COLOR_GREEN;
  light_counter[ROAD2] = duration_green;

  setTrafficLight(ROAD1, light_color[ROAD1]);
  setTrafficLight(ROAD2, light_color[ROAD2]);
  update7SEGBuffer(light_counter[ROAD1], light_counter[ROAD2]);
  setTimer0(ONE_SECOND);
}

static void traffic_tick(int road)
{
  light_counter[road]--;
  if (light_counter[road] <= 0) {
    switch (light_color[road]) {
    case COLOR_RED:
      light_color[road]   = COLOR_GREEN;
      light_counter[road] = duration_green;
      break;
    case COLOR_GREEN:
      light_color[road]   = COLOR_AMBER;
      light_counter[road] = duration_amber;
      break;
    case COLOR_AMBER:
      light_color[road]   = COLOR_RED;
      light_counter[road] = duration_red;
      break;
    default:
      break;
    }
    setTrafficLight(road, light_color[road]);
  }
}

enum Mode { INIT, MODE_1_NORMAL, MODE_2_RED, MODE_3_AMBER, MODE_4_GREEN };
enum Mode mode = INIT;

static int temp_value;
static int index_led = 0;

static void enter_modify_mode(int current_value, int mode_number)
{
  clearAllTrafficLights();
  temp_value = current_value;
  update7SEGBuffer(temp_value, mode_number);
  setTimer1(BLINK_DURATION);
}

static void increase_temp_value(int mode_number)
{
  temp_value++;
  if (temp_value > MAX_DURATION) temp_value = MIN_DURATION;
  update7SEGBuffer(temp_value, mode_number);
}

void fsm_for_input_processing(void)
{
  unsigned char mode_pressed = fsm_for_button(BUTTON_MODE);
  unsigned char inc_pressed  = fsm_for_button(BUTTON_INC);
  unsigned char set_pressed  = fsm_for_button(BUTTON_SET);

  switch (mode) {
  case INIT:
    setTimer2(SCAN_DURATION);
    traffic_start();
    mode = MODE_1_NORMAL;
    break;

  case MODE_1_NORMAL:
    if (timer0_flag == 1) {
      setTimer0(ONE_SECOND);
      traffic_tick(ROAD1);
      traffic_tick(ROAD2);
      update7SEGBuffer(light_counter[ROAD1], light_counter[ROAD2]);
    }
    if (mode_pressed) {
      mode = MODE_2_RED;
      enter_modify_mode(duration_red, 2);
    }
    break;

  case MODE_2_RED:
    if (timer1_flag == 1) {
      setTimer1(BLINK_DURATION);
      toggleTrafficColor(COLOR_RED);
    }
    if (inc_pressed) increase_temp_value(2);
    if (set_pressed) duration_red = temp_value;
    if (mode_pressed) {
      mode = MODE_3_AMBER;
      enter_modify_mode(duration_amber, 3);
    }
    break;

  case MODE_3_AMBER:
    if (timer1_flag == 1) {
      setTimer1(BLINK_DURATION);
      toggleTrafficColor(COLOR_AMBER);
    }
    if (inc_pressed) increase_temp_value(3);
    if (set_pressed) duration_amber = temp_value;
    if (mode_pressed) {
      mode = MODE_4_GREEN;
      enter_modify_mode(duration_green, 4);
    }
    break;

  case MODE_4_GREEN:
    if (timer1_flag == 1) {
      setTimer1(BLINK_DURATION);
      toggleTrafficColor(COLOR_GREEN);
    }
    if (inc_pressed) increase_temp_value(4);
    if (set_pressed) duration_green = temp_value;
    if (mode_pressed) {
      mode = MODE_1_NORMAL;
      traffic_start();
    }
    break;

  default:
    break;
  }

  if (timer2_flag == 1) {
    setTimer2(SCAN_DURATION);
    update7SEG(index_led++);
    if (index_led >= MAX_LED) index_led = 0;
  }
}
