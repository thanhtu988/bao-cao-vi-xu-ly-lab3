/*
 * timer.c
 *
 *  Created on: Oct 28, 2026
 */

#include "main.h"
#include "input_reading.h"
#include "timer.h"

/* ================= SOFTWARE TIMER (giong Program 1.6 cua Lab 2) =================
   setTimerX(ms) doi ms ra so nhip ngat; timer_run() dem lui 1 nhip moi lan ngat,
   dem ve 0 thi dung co timerX_flag de vong lap chinh biet "da den han". */
int timer0_counter = 0;
int timer0_flag    = 0;
int timer1_counter = 0;
int timer1_flag    = 0;
int timer2_counter = 0;
int timer2_flag    = 0;

void setTimer0(int duration)
{
  timer0_counter = duration / TIMER_CYCLE;
  timer0_flag    = 0;
}

void setTimer1(int duration)
{
  timer1_counter = duration / TIMER_CYCLE;
  timer1_flag    = 0;
}

void setTimer2(int duration)
{
  timer2_counter = duration / TIMER_CYCLE;
  timer2_flag    = 0;
}

void timer_run(void)
{
  if (timer0_counter > 0) {
    timer0_counter--;
    if (timer0_counter == 0) timer0_flag = 1;
  }
  if (timer1_counter > 0) {
    timer1_counter--;
    if (timer1_counter == 0) timer1_flag = 1;
  }
  if (timer2_counter > 0) {
    timer2_counter--;
    if (timer2_counter == 0) timer2_flag = 1;
  }
}

/* ================= NGAT TIM2 (Program 1.6) =================
   Moi TIMER_CYCLE ms: doc + chong doi 3 nut, roi dem cac software timer. */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2) {
    button_reading();
    timer_run();
  }
}
