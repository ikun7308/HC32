#ifndef _WAKEUP_H_
#define _WAKEUP_H_

extern void KeyWakeup_Config(void);
extern void Gpio_Set_IN(en_gpio_port_t enPort, en_gpio_pin_t enPin);
extern void Gpio_Set_OUT(en_gpio_port_t enPort, en_gpio_pin_t enPin);
extern void Gpio_Set_IN_D(en_gpio_port_t enPort, en_gpio_pin_t enPin);
extern void Closedoor_Parament_Init(void);
#endif
