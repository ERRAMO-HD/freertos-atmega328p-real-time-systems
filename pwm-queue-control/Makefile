# Portable AVR-GCC Makefile for ATmega328P + FreeRTOS v7.1.0
MCU ?= atmega328p
F_CPU ?= 16000000UL
PORT ?= COM5
BAUD ?= 115200
PROGRAMMER ?= arduino
CC ?= avr-gcc
# 'make' defines CC=cc by default; force AVR compiler if not overridden on command line.
ifeq ($(origin CC),default)
CC = avr-gcc
endif
OBJCOPY ?= avr-objcopy
AVRDUDE ?= avrdude
FREERTOS_DIR := ../freertos-kernel
INCLUDES := -I. -I$(FREERTOS_DIR)/include -I$(FREERTOS_DIR)/source/portable
CFLAGS := -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -std=gnu99 -Wall $(INCLUDES)
FREERTOS_SRC := $(FREERTOS_DIR)/source/tasks.c \
               $(FREERTOS_DIR)/source/queue.c \
               $(FREERTOS_DIR)/source/list.c \
               $(FREERTOS_DIR)/source/timers.c \
               $(FREERTOS_DIR)/source/event_groups.c \
               $(FREERTOS_DIR)/source/croutine.c \
               $(FREERTOS_DIR)/source/portable/heap_1.c \
               $(FREERTOS_DIR)/source/portable/port.c
LOCAL_SRC := main.c pwm.c

.PHONY: all clean upload
all: main.hex

main.elf: $(LOCAL_SRC) $(FREERTOS_SRC) FreeRTOSConfig.h
	$(CC) $(CFLAGS) $(LOCAL_SRC) $(FREERTOS_SRC) -o $@

main.hex: main.elf
	$(OBJCOPY) -R .eeprom -O ihex $< $@

upload: main.hex
	$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -D -U flash:w:main.hex:i

clean:
	-rm -f main.elf
	@echo 'Note: main.hex is retained because Proteus uses it directly.'
