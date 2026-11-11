/*
 * timer.h
 *
 *  Created on: Oct 28, 2026
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

/* Chu ky ngat cua TIM2, don vi ms.
   Exercise 4: khi doi chu ky ngat (PSC/ARR cua TIM2 trong file .ioc)
   chi can sua DUY NHAT hang so nay. Moi khoang thoi gian khac trong
   chuong trinh deu viet bang ms va duoc doi ra so nhip ngat qua TIMER_CYCLE. */
#define TIMER_CYCLE   10

extern int timer0_flag;     /* nhip 1 giay cua den giao thong (mode 1)  */
extern int timer1_flag;     /* nhip nhap nhay 2 Hz cua den (mode 2-4)   */
extern int timer2_flag;     /* nhip quet LED 7 doan                     */

void setTimer0(int duration);
void setTimer1(int duration);
void setTimer2(int duration);
void timer_run(void);

#endif /* INC_TIMER_H_ */
