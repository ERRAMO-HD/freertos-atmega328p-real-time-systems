/*
 * pwm.h
 * Driver PWM pour ATmega328P - Timer0 (OC0A / PD6)
 */

#ifndef PWM_DRIVER_H
#define PWM_DRIVER_H

#include <stdint.h>
#include <avr/io.h>

int8_t PWM_Init(void);
int8_t PWM_SetDutyCycle(uint8_t duty_percent);
uint8_t PWM_GetDutyCycle(void);
int8_t PWM_Enable(void);
int8_t PWM_Disable(void);

#endif /* PWM_DRIVER_H */
