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


void Timer_Init(unsigned int period) {
    TIMER_PERIOD_REG = period;
    Timer_EnableInterrupt();
}

void Timer_Start(void) {
    TIMER_CONTROL_REG |= 0x01;
}

void Timer_Stop(void) {
    TIMER_CONTROL_REG &= ~0x01;
}

void Timer_Reset(void) {
    TIMER_VALUE_REG = 0;
}

unsigned int Timer_Read(void) {
    return TIMER_VALUE_REG;
}

void Timer_EnableInterrupt(void) {
    TIMER_INTERRUPT_ENABLE |= 0x01;
}

void Timer_DisableInterrupt(void) {
    TIMER_INTERRUPT_ENABLE &= ~0x01;
}

void Timer_ISR(void) {
    // Handle the timer interrupt (e.g., clear interrupt flag, execute callback)
}