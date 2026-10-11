# Build both AVR/FreeRTOS demos (requires make and avr-gcc)
.PHONY: all event-groups pwm-queue-control clean
all: event-groups pwm-queue-control

event-groups:
	$(MAKE) -C event-groups

pwm-queue-control:
	$(MAKE) -C pwm-queue-control

clean:
	$(MAKE) -C event-groups clean
	$(MAKE) -C pwm-queue-control clean
