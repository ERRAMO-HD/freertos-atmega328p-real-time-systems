# Exercise 02 — FreeRTOS Queue + Hardware PWM

**Source:** [`main.c`](main.c), [`pwm.c`](pwm.c), [`pwm.h`](pwm.h)  
**Proteus:** [`simulation/simulation.pdsprj`](simulation/simulation.pdsprj)  
**Precompiled firmware:** [`main.hex`](main.hex)

## Operation

- Push button: **PD2**, with input pull-up and software debounce.
- LED PWM output: **PD6 / OC0A** (Timer0 Fast PWM, prescaler 64, about 976 Hz at 16 MHz).
- A FreeRTOS `Task_Button` sends duty-cycle commands using `xQueueSend`.
- `Task_PWM` blocks in `xQueueReceive` until a new command arrives, then updates OCR0A using `PWM_SetDutyCycle`.
- Duty levels cycle through **0%, 25%, 50%, 75%, 100%**.

## Original video demonstrations

- [Code walkthrough](videos/pwm_queue_code_explanation.mp4)
- [Proteus demonstration](videos/pwm_queue_simulation.mp4)

## Build / simulation

`make` builds `main.hex` using shared FreeRTOS source in `../freertos-kernel/`. Open `simulation/simulation.pdsprj` in Proteus; it references `../main.hex`.

## Archived optional code

The `optional-servo/` directory preserves `SERVO.c` and `SERVO.h` found in the supplied exercise archive. They are not referenced by this exercise's Makefile and are not used by this PWM implementation.
