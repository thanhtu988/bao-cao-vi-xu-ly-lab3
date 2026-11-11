/*
 * input_processing.c
 *
 *  Created on: Oct 28, 2026
 */

#include "main.h"
#include "input_reading.h"
#include "led_display.h"
#include "timer.h"

/* ---- 3 nut nhan ---- */
#define BUTTON_MODE       0     /* nut 1: chon mode           */
#define BUTTON_INC        1     /* nut 2: tang gia tri        */
#define BUTTON_SET        2     /* nut 3: luu gia tri da chon */

/* ---- thoi gian, don vi ms (Exercise 4: khong phu thuoc chu ky ngat) ---- */
#define ONE_SECOND        1000          /* nhip dem lui cua den giao thong     */
#define BLINK_DURATION    250           /* 2 Hz: sang 250 ms + tat 250 ms = 0,5 s */
#define SCAN_DURATION     (1000 / MAX_LED)  /* 250 ms moi LED -> quet 4 LED = 1 Hz (nhu Lab 2) */

/* ---- gioi han gia tri thoi gian (de bai: 1 - 99) ---- */
#define MIN_DURATION      1
#define MAX_DURATION      99

/* =====================================================================
 *  1. FSM XU LY NUT NHAN - Program 1.7, ap dung cho ca 3 nut
 * ===================================================================== */
enum ButtonState { BUTTON_RELEASED, BUTTON_PRESSED, BUTTON_PRESSED_MORE_THAN_1_SECOND };
enum ButtonState buttonState[N0_OF_BUTTONS] = {
  BUTTON_RELEASED, BUTTON_RELEASED, BUTTON_RELEASED
};

/* Tra ve 1 dung MOT lan khi nut vua chuyen tu "nha" sang "nhan".
   Giu nut bao lau cung chi tinh la 1 lan nhan. */
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
    /* de bai khong yeu cau tu tang khi giu nut -> khong lam gi them */
    break;
  }
  return pressed;
}

/* =====================================================================
 *  2. THOI GIAN CUA TUNG MAU (giay) - mac dinh 5 / 2 / 3 nhu Lab 1
 * ===================================================================== */
int duration_red   = 5;
int duration_amber = 2;
int duration_green = 3;

/* =====================================================================
 *  3. FSM DEN GIAO THONG (mode 1) - moi tuyen chay vong DO -> XANH -> VANG
 * ===================================================================== */
static int light_color[2];      /* mau dang sang cua tung tuyen          */
static int light_counter[2];    /* so giay con lai cua mau do            */

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

/* =====================================================================
 *  4. FSM CHE DO (mode 1 -> 2 -> 3 -> 4 -> 1)
 * ===================================================================== */
enum Mode { INIT, MODE_1_NORMAL, MODE_2_RED, MODE_3_AMBER, MODE_4_GREEN };
enum Mode mode = INIT;

static int temp_value;          /* gia tri dang chinh, chua luu */
static int index_led = 0;       /* LED 7 doan dang duoc quet    */

/* Vao mode chinh sua: tat het den, 2 LED tuyen 1 hien gia tri,
   2 LED tuyen 2 hien so mode, bat dau nhip nhap nhay 2 Hz */
static void enter_modify_mode(int current_value, int mode_number)
{
  clearAllTrafficLights();
  temp_value = current_value;
  update7SEGBuffer(temp_value, mode_number);
  setTimer1(BLINK_DURATION);
}

/* Nut 2: tang 1 don vi, qua 99 thi quay ve 1 (khong co nut giam) */
static void increase_temp_value(int mode_number)
{
  temp_value++;
  if (temp_value > MAX_DURATION) temp_value = MIN_DURATION;
  update7SEGBuffer(temp_value, mode_number);
}

/* Goi lien tuc trong while(1) cua main() - Program 1.9 */
void fsm_for_input_processing(void)
{
  /* doc su kien 3 nut: moi lan nhan chi tinh 1 lan */
  unsigned char mode_pressed = fsm_for_button(BUTTON_MODE);
  unsigned char inc_pressed  = fsm_for_button(BUTTON_INC);
  unsigned char set_pressed  = fsm_for_button(BUTTON_SET);

  switch (mode) {
  case INIT:
    setTimer2(SCAN_DURATION);
    traffic_start();
    mode = MODE_1_NORMAL;
    break;

  /* ---------------- Mode 1: den giao thong chay binh thuong ---------------- */
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

  /* ---------------- Mode 2: chinh thoi gian den DO ---------------- */
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

  /* ---------------- Mode 3: chinh thoi gian den VANG ---------------- */
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

  /* ---------------- Mode 4: chinh thoi gian den XANH ---------------- */
  case MODE_4_GREEN:
    if (timer1_flag == 1) {
      setTimer1(BLINK_DURATION);
      toggleTrafficColor(COLOR_GREEN);
    }
    if (inc_pressed) increase_temp_value(4);
    if (set_pressed) duration_green = temp_value;
    if (mode_pressed) {
      mode = MODE_1_NORMAL;
      traffic_start();            /* chay lai tu dau voi thoi gian moi */
    }
    break;

  default:
    break;
  }

  /* ---- quet 4 LED 7 doan (driver dau ra) ---- */
  if (timer2_flag == 1) {
    setTimer2(SCAN_DURATION);
    update7SEG(index_led++);
    if (index_led >= MAX_LED) index_led = 0;
  }
}
