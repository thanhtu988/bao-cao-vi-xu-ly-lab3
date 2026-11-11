/*
 * led_display.c
 *
 *  Created on: Oct 28, 2026
 */

#include "main.h"
#include "led_display.h"

/* LED don noi anode len 3.3V, cathode vao MCU -> muc 0 thi sang (Lab 1) */
#define LED_ON         GPIO_PIN_RESET
#define LED_OFF        GPIO_PIN_SET
/* LED 7 doan chung anode -> doan sang khi xuat muc 0 (Lab 1) */
#define SEG_ON         GPIO_PIN_RESET
#define SEG_OFF        GPIO_PIN_SET
/* EN qua transistor PNP -> muc 0 thi LED 7 doan duoc cap nguon (Lab 2) */
#define EN_ON          GPIO_PIN_RESET
#define EN_OFF         GPIO_PIN_SET

/* ======================= LED 7 DOAN ======================= */
int led_buffer[MAX_LED] = {0, 0, 0, 0};

/* Bang ma 7 doan, bit0..bit6 = doan a..g */
static const uint8_t SEG_CODE[10] = {
  0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

void display7SEG(int num)
{
  if (num < 0 || num > 9) return;
  for (int i = 0; i < 7; i++) {                   /* SEG0..SEG6 = PB0..PB6 */
    HAL_GPIO_WritePin(SEG0_GPIO_Port, (uint16_t)(SEG0_Pin << i),
                      ((SEG_CODE[num] >> i) & 1) ? SEG_ON : SEG_OFF);
  }
}

/* Quet 1 LED theo 3 buoc cua Lab 2: tat het -> dat du lieu -> bat dung 1 LED */
void update7SEG(int index)
{
  HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin | EN1_Pin | EN2_Pin | EN3_Pin, EN_OFF);

  switch (index) {
  case 0:
    display7SEG(led_buffer[0]);
    HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, EN_ON);
    break;
  case 1:
    display7SEG(led_buffer[1]);
    HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, EN_ON);
    break;
  case 2:
    display7SEG(led_buffer[2]);
    HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, EN_ON);
    break;
  case 3:
    display7SEG(led_buffer[3]);
    HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, EN_ON);
    break;
  default:
    break;
  }
}

/* Moi tuyen 2 LED 7 doan: tach so 2 chu so thanh hang chuc / hang don vi */
void update7SEGBuffer(int road1_value, int road2_value)
{
  led_buffer[0] = road1_value / 10;
  led_buffer[1] = road1_value % 10;
  led_buffer[2] = road2_value / 10;
  led_buffer[3] = road2_value % 10;
}

/* ===================== 12 DEN GIAO THONG ===================== */
/* Moi tuyen co 2 huong chay cung nhau:
   ROAD1 = Bac (PA1-PA3) + Nam (PA4-PA6), ROAD2 = Dong (PA7-PA9) + Tay (PA10-PA12) */
static const uint16_t ROAD_PIN[2][3] = {
  /*        RED                    AMBER                      GREEN                  */
  { N_RED_Pin | S_RED_Pin, N_AMBER_Pin | S_AMBER_Pin, N_GREEN_Pin | S_GREEN_Pin },
  { E_RED_Pin | W_RED_Pin, E_AMBER_Pin | W_AMBER_Pin, E_GREEN_Pin | W_GREEN_Pin }
};

/* Bat dung 1 mau cua 1 tuyen, 2 mau con lai tat */
void setTrafficLight(int road, int color)
{
  for (int c = 0; c < 3; c++) {
    HAL_GPIO_WritePin(N_RED_GPIO_Port, ROAD_PIN[road][c], (c == color) ? LED_ON : LED_OFF);
  }
}

void clearAllTrafficLights(void)
{
  for (int c = 0; c < 3; c++) {
    HAL_GPIO_WritePin(N_RED_GPIO_Port, ROAD_PIN[ROAD1][c] | ROAD_PIN[ROAD2][c], LED_OFF);
  }
}

/* Dao trang thai ca 4 den cung mau (nhap nhay o mode 2-4) */
void toggleTrafficColor(int color)
{
  HAL_GPIO_TogglePin(N_RED_GPIO_Port, ROAD_PIN[ROAD1][color] | ROAD_PIN[ROAD2][color]);
}
