#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

// ==============================
//        Zones matérielles
// ==============================

// LEDs
#define led1   PB0
#define led2   PB1
#define led3   PB2

// Boutons
#define buton1   PD2
#define buton2   PD3
#define buton3   PD4

// Bits d’évènements
#define event1  (1 << 0)
#define event2  (1 << 1)
#define event3  (1 << 2)

// Handle Event Group
EventGroupHandle_t sysEvents;

// ==============================
//      Initialisation GPIO
// ==============================
void initialiser_IO(void) {

    // LEDs en sortie
    DDRB |= (1 << led1) | (1 << led2) | (1 << led3);

    // Boutons en entrée
    DDRD &= ~((1 << buton1) | (1 << buton2) | (1 << buton3));

    // Pull-up activés ? boutons actifs à LOW
    PORTD |= (1 << buton1) | (1 << buton2) | (1 << buton3);
}

// ==============================
//          Tâche 1
// ==============================
void Task_Read1(void *pvParameters) {
    while(1) {

        // Bouton en PULL-UP ? action quand LOW (0)
        if (!(PIND & (1 << buton1))) {
            xEventGroupSetBits(sysEvents, event1);
            vTaskDelay(pdMS_TO_TICKS(500)); // anti-rebond simple
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

// ==============================
//          Tâche 2
// ==============================
void Task_Read2(void *pvParameters) {
    while(1) {

        if (!(PIND & (1 << buton2))) {
            xEventGroupSetBits(sysEvents, event2);
            vTaskDelay(pdMS_TO_TICKS(500));
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

// ==============================
//          Tâche 3
// ==============================
void Task_Read3(void *pvParameters) {
    while(1) {

        if (!(PIND & (1 << buton3))) {
            xEventGroupSetBits(sysEvents, event3);
            vTaskDelay(pdMS_TO_TICKS(500));
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

// ==============================
//       Tâche Manager
// ==============================
void Task_AllTriggered(void *pvParameters) {
    EventBits_t uxBits;

    while(1) {
        uxBits = xEventGroupWaitBits(sysEvents,event1 | event2 | event3,pdTRUE,pdFALSE,portMAX_DELAY);

        if (uxBits & event1) PORTB |= (1 << led1);
        if (uxBits & event2) PORTB |= (1 << led2);
        if (uxBits & event3) PORTB |= (1 << led3);
    }
}

// ==============================
//               MAIN
// ==============================
int main(void) {

    initialiser_IO();

    sysEvents = xEventGroupCreate();

    xTaskCreate(Task_Read1,     "Evt1",    128, NULL, 1, NULL);
    xTaskCreate(Task_Read2,     "Evt2",    128, NULL, 1, NULL);
    xTaskCreate(Task_Read3,     "Evt3",    128, NULL, 1, NULL);
    xTaskCreate(Task_AllTriggered, "Manager", 128, NULL, 2, NULL);

    vTaskStartScheduler();

    while (1);
}

// ==============================
//          Idle Hook
// ==============================
void vApplicationIdleHook(void)
{
}

