#include "main.h"
#include "led_display.h"

#define LED_ON         GPIO_PIN_RESET
#define LED_OFF        GPIO_PIN_SET

#define SEG_ON         GPIO_PIN_RESET
#define SEG_OFF        GPIO_PIN_SET

#define EN_ON          GPIO_PIN_RESET
#define EN_OFF         GPIO_PIN_SET

int led_buffer[MAX_LED] = {0, 0, 0, 0};

static const uint8_t SEG_CODE[10] = {
  0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

void display7SEG(int num)
{
  if (num < 0 || num > 9) return;
  for (int i = 0; i < 7; i++) {
    HAL_GPIO_WritePin(SEG0_GPIO_Port, (uint16_t)(SEG0_Pin << i),
                      ((SEG_CODE[num] >> i) & 1) ? SEG_ON : SEG_OFF);
  }
}

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

void update7SEGBuffer(int road1_value, int road2_value)
{
  led_buffer[0] = road1_value / 10;
  led_buffer[1] = road1_value % 10;
  led_buffer[2] = road2_value / 10;
  led_buffer[3] = road2_value % 10;
}

static const uint16_t ROAD_PIN[2][3] = {
  { N_RED_Pin | S_RED_Pin, N_AMBER_Pin | S_AMBER_Pin, N_GREEN_Pin | S_GREEN_Pin },
  { E_RED_Pin | W_RED_Pin, E_AMBER_Pin | W_AMBER_Pin, E_GREEN_Pin | W_GREEN_Pin }
};

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

void toggleTrafficColor(int color)
{
  HAL_GPIO_TogglePin(N_RED_GPIO_Port, ROAD_PIN[ROAD1][color] | ROAD_PIN[ROAD2][color]);
}
