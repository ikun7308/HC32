#ifndef __SYSCLOCK_H__
#define __SYSCLOCK_H__

extern uint8_t systime1ms_flag;
extern uint8_t systime100ms_flag;
extern uint8_t systime10min_flag;

extern void sysclock_init(void);					
extern void TIMER0_Init(void);
extern void WDOG_Init(void);
extern void WDOG_Feed(void);



#endif

