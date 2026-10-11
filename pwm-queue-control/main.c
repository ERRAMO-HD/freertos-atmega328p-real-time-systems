/*
 * Exercice : Contrôle LED PWM via FreeRTOS + Queue
 * Version simplifiée et conforme à l’énoncé
 */

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "pwm.h"

/* --- Configuration matériel --- */
#define BTN_PIN   PD2   // bouton sur PD2

/* Niveaux PWM */
#define NUM_LEVELS 5
const uint8_t duty_levels[NUM_LEVELS] = {0, 25, 50, 75, 100};

/* Queue FreeRTOS */
QueueHandle_t qPWM;

/* Message envoyé à la tâche PWM */
typedef struct {
    uint8_t duty;
} PWMMessage_t;

/* ---------------- BOUTON ---------------- */
void Button_Init(void)
{
    DDRD &= ~(1 << BTN_PIN);  // entrée
    PORTD |= (1 << BTN_PIN);  // pull-up
}

uint8_t Button_Read(void)
{
    return !(PIND & (1 << BTN_PIN));  // actif bas -> 1 si appuyé
}

/* Anti-rebond simple */
uint8_t Debounce(void)
{
    uint8_t stable = Button_Read();
    vTaskDelay(pdMS_TO_TICKS(20));
    if (Button_Read() == stable) {
    return stable;
	} 
	else {
      return 0;
	}

}		

/* --------------- TÂCHE BOUTON --------------- */
void Task_Button(void *p)
{
    uint8_t index = 0;
    uint8_t last = 0;
    PWMMessage_t msg;

    for (;;)
    {
        uint8_t press = Debounce();

        if (press && !last)  // front montant
        {
            index = (index + 1) % NUM_LEVELS;
            msg.duty = duty_levels[index];
            xQueueSend(qPWM, &msg, 0);
        }

        last = press;
    }
}

/* --------------- TÂCHE PWM --------------- */
void Task_PWM(void *p)
{
    PWMMessage_t rx;

    for (;;)
    {
        if (xQueueReceive(qPWM, &rx, portMAX_DELAY) == pdPASS)
            PWM_SetDutyCycle(rx.duty);//=0 led ettient si non 100 led a fond 
    }
}

/* ------------------ MAIN ------------------ */
int main(void)
{
    Button_Init();

    if (PWM_Init() != 0) 
	while (1);

    qPWM = xQueueCreate(5, sizeof(PWMMessage_t));
    if (!qPWM) 
	while (1);

    PWMMessage_t init = {0};
    xQueueSend(qPWM, &init, 0);

    xTaskCreate(Task_Button, "BTN", 128, NULL, 2, NULL);
    xTaskCreate(Task_PWM,    "PWM", 128, NULL, 1, NULL);

    sei();
    vTaskStartScheduler();
    while (1);
}

/* Hooks */
void vApplicationIdleHook(void) {}
void vApplicationMallocFailedHook(void){ while(1); }
void vApplicationStackOverflowHook(TaskHandle_t t, char *n){ while(1); }

