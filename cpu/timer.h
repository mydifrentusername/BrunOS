#ifndef CPU_TIMER_H
#define CPU_TIMER_H

#define TIMER_BASE_ADDRESS 0x40000000
#define TIMER_CONTROL_REG (*(volatile unsigned int *)(TIMER_BASE_ADDRESS + 0x00))
#define TIMER_PERIOD_REG (*(volatile unsigned int *)(TIMER_BASE_ADDRESS + 0x04))
#define TIMER_VALUE_REG (*(volatile unsigned int *)(TIMER_BASE_ADDRESS + 0x08))
#define TIMER_INTERRUPT_ENABLE (*(volatile unsigned int *)(TIMER_BASE_ADDRESS + 0x0C))

void Timer_Init(unsigned int period);
void Timer_Start(void);
void Timer_Stop(void);
void Timer_Reset(void);
unsigned int Timer_Read(void);
void Timer_EnableInterrupt(void);
void Timer_DisableInterrupt(void);

void Timer_ISR(void);

#endif