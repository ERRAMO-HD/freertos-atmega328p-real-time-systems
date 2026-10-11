# Exercise 01 — FreeRTOS Event Groups

**Source:** [`main.c`](main.c)  
**Proteus:** [`simulation/simulation.pdsprj`](simulation/simulation.pdsprj)  
**Precompiled firmware:** [`main.hex`](main.hex)

## Operation

- Buttons: **PD2**, **PD3**, **PD4** (input pull-ups, active LOW).
- LEDs: **PB0**, **PB1**, **PB2** (output).
- Three FreeRTOS tasks poll their buttons, with simple 500 ms delay following detection and 50 ms between polling passes.
- A fourth task blocks on `xEventGroupWaitBits` until any event arrives; the associated LED is set ON. LEDs stay ON in the submitted code until reset.

> The assignment explicitly requests interrupt-generated events. The supplied implementation instead **polls** the inputs. The current source uses OR mode (`pdFALSE` for the wait-all parameter); an additional AND-mode simulation video is preserved from the submission.

## Original video demonstrations

- [Walkthrough of the event-group code](videos/event_groups_code_explanation.mp4)
- [Simulation — OR mode](videos/event_groups_or_simulation.mp4)
- [Simulation — AND mode](videos/event_groups_and_simulation.mp4)

## Build / simulation

`make` builds `main.hex` using the shared legacy FreeRTOS sources in `../freertos-kernel/` and the local `FreeRTOSConfig.h`. Open `simulation/simulation.pdsprj` in Proteus; it references `../main.hex`.
