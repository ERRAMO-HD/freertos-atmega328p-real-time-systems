# Real-Time Embedded Systems on ATmega328P · FreeRTOS

**Two microcontroller programming exercises implemented in embedded C without Arduino IDE**: FreeRTOS event-group synchronization and queue-driven hardware PWM on the ATmega328P.

**Author:** Mohamed Erradi  
**Hardware:** ATmega328P / Arduino Uno-compatible target, 16 MHz  
**Toolchain:** AVR-GCC · GNU Make · AVRDUDE · Proteus · FreeRTOS (legacy v7.1.0)

## Exercises

| Exercise | What it demonstrates | Source | Demonstration videos |
|---|---|---|---|
| **01 — Event Groups** | 3 button-input tasks, 1 task waiting on event bits; LED control using `xEventGroupSetBits()` and `xEventGroupWaitBits()` | [event-groups/](event-groups/) | [Code explanation](event-groups/videos/event_groups_code_explanation.mp4) · [OR simulation](event-groups/videos/event_groups_or_simulation.mp4) · [AND simulation](event-groups/videos/event_groups_and_simulation.mp4) |
| **02 — PWM with Queue** | Button debounce, `xQueueSend()` / `xQueueReceive()`, Timer0 PWM duty-cycle control on OC0A (PD6) | [pwm-queue-control/](pwm-queue-control/) | [Code explanation](pwm-queue-control/videos/pwm_queue_code_explanation.mp4) · [Proteus simulation](pwm-queue-control/videos/pwm_queue_simulation.mp4) |

The original **exercise statements only** (without the university logo, masthead or date) are available in [docs/exercise-statements.pdf](docs/exercise-statements.pdf).

## Project layout

```text
freertos-atmega328p-real-time-systems/
├── event-groups/
│   ├── main.c
│   ├── FreeRTOSConfig.h
│   ├── Makefile
│   ├── main.hex                # Precompiled firmware for Proteus
│   ├── simulation/simulation.pdsprj
│   └── videos/                # 3 original MP4 demonstrations
├── pwm-queue-control/
│   ├── main.c
│   ├── pwm.c
│   ├── pwm.h
│   ├── FreeRTOSConfig.h
│   ├── Makefile
│   ├── main.hex                # Precompiled firmware for Proteus
│   ├── simulation/simulation.pdsprj
│   ├── optional-servo/        # Archived unused sources from submitted ZIP
│   └── videos/                # 2 original MP4 demonstrations
├── freertos-kernel/           # Supplied FreeRTOS v7.1.0 sources, shared
├── docs/exercise-statements.pdf
├── Makefile
├── THIRD_PARTY_NOTICES.md
└── .gitignore
```

## Build from the command line

Install **AVR-GCC**, **GNU Make**, **avr-objcopy**, and **AVRDUDE** for your OS and ensure the executables are on `PATH`. No Arduino IDE or Microchip Studio is needed for building the code.

```bash
# Build both exercises
make

# Or build an individual example
cd event-groups
make

# Flash a connected Arduino Uno-compatible board with AVRDUDE:
make upload PORT=COM5 BAUD=115200 PROGRAMMER=arduino
```

On Windows, run the commands from a shell with `make` and the AVR tools installed. Replace `COM5` with your board's actual serial port. The bundled `.hex` files are the author's previously generated binaries; the Makefiles are updated to use **relative paths** but have **not been recompiled in this package environment**.

## Proteus simulation

1. Open `event-groups/simulation/simulation.pdsprj` or `pwm-queue-control/simulation/simulation.pdsprj` in Proteus.
2. The ATmega328P program file in both supplied simulation projects is set to `..\main.hex` (relative to `simulation/`).
3. Run the simulation. To test your own source changes, run `make` from the corresponding exercise directory and reload the simulation.

**Note:** The supplied projects use an Arduino Uno Proteus component from TheEngineeringProjects.com. If that custom library is not installed locally, Proteus may ask you for the component library.

## Implementation notes

- **Exercise 01:** `Task_Read1`, `Task_Read2`, and `Task_Read3` poll active-low buttons on PD2/PD3/PD4 and set FreeRTOS event-group bits. `Task_AllTriggered` waits for **any** event (`pdFALSE`) and lights LEDs on PB0/PB1/PB2. As supplied, the input tasks are **polled**, *not actual external-interrupt ISRs*; this differs from the interrupt requirement in the exercise statement. The AND-mode video is included as recorded; the provided `main.c` is the OR-mode code.
- **Exercise 02:** A button on PD2 cycles through **0/25/50/75/100%** brightness commands, passed via a FreeRTOS queue to a PWM task. Timer0 Fast PWM drives PD6 (OC0A) at approximately **976 Hz** using a prescaler of 64 and an 8-bit counter.
- The source code is preserved in substance from the submitted archive. This repository packages coursework/demonstrations, not a certified industrial real-time application.

## Videos

All **five original MP4 files** from the project submission are included in their respective `videos/` directories; none of their video content was altered. GitHub Desktop is recommended for uploading the repository while preserving folder structure.

## Third-party code

The `freertos-kernel/` directory contains **FreeRTOS v7.1.0** sources originally supplied with the coursework. See [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md) for provenance and licensing notes. The optional SERVO files from the original exercise 02 archive are preserved but not part of the build.
