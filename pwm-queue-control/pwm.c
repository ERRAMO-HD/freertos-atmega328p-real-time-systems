/*
 * pwm.c
 * Driver PWM pour ATmega328P - Timer0 (OC0A -> PD6)
 *
 * Configuration cible:
 *  - Pin: OC0A (PD6 / Arduino D6)
 *  - Timer0 en Fast PWM (TOP = 0xFF)
 *  - Prescaler = 64 -> ~976 Hz (16 MHz / (64 * 256))
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "pwm.h"

#define PWM_DDR_REG   DDRD
#define PWM_PORT_REG  PORTD
#define PWM_PIN_BIT   PD6   /* numéro du bit (6) */

/* driver interne */
static uint8_t current_duty_cycle = 0;
static uint8_t pwm_enabled = 0;

int8_t PWM_Init(void)
{
    /* configurer PD6 comme sortie */
    PWM_DDR_REG |= (1 << PWM_PIN_BIT);

    /* Timer0 en Fast PWM non-inverting sur OC0A */
    /* WGM01:0 = 11 (Fast PWM, TOP=0xFF) ; COM0A1:0 = 10 (non-inverting) */
    TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);

    /* Prescaler = 64 -> CS02:0 = 011 */
    TCCR0B = (1 << CS01) | (1 << CS00);

    /* init duty = 0% */
    OCR0A = 0;
    current_duty_cycle = 0;
    pwm_enabled = 1;

    return 0;
}

int8_t PWM_SetDutyCycle(uint8_t duty_percent)
{
    if (duty_percent > 100) return -1;

    /* calcul 0..255 */
    uint16_t ocr = ((uint16_t)duty_percent * 255UL) / 100UL;
    OCR0A = (uint8_t)ocr;
    current_duty_cycle = duty_percent;
    return 0;
}

uint8_t PWM_GetDutyCycle(void)
{
    return current_duty_cycle;
}

int8_t PWM_Enable(void)
{
    /* remettre le mode compare non-inverting (COM0A1:0 = 10) */
    TCCR0A &= ~((1 << COM0A0));      /* COM0A0 = 0 */
    TCCR0A |=  (1 << COM0A1);       /* COM0A1 = 1 */
    pwm_enabled = 1;
    return 0;
}

int8_t PWM_Disable(void)
{
    /* désactiver la sortie hardware (COM0A1:0 = 00) */
    TCCR0A &= ~((1 << COM0A1) | (1 << COM0A0));

    /* forcer la sortie à 0 (la broche est en sortie) */
    PWM_PORT_REG &= ~(1 << PWM_PIN_BIT);

    pwm_enabled = 0;
    return 0;
}

