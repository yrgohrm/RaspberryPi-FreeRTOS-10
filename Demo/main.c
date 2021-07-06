#include <FreeRTOS.h>
#include <task.h>

#include "Drivers/irq.h"
#include "Drivers/gpio.h"

static volatile int blink = 1;

void task1(void *pParam) {
    while(1) {
        if (blink)
            SetGpio(16, 1);
        vTaskDelay(200);
    }
}

void task2(void *pParam) {
    while(1) {
        vTaskDelay(100);
        SetGpio(16, 0);
        vTaskDelay(100);
    }
}

void button_handler(unsigned int irq, void *pParam) {
    if (EventDetected(12)) {
        blink = !blink;
        ClearGpioInterrupt(12);
    }
}

/**
 *    This is the systems main entry, some call it a boot thread.
 *
 *    -- Absolutely nothing wrong with this being called main(), just it doesn't have
 *    -- the same prototype as you'd see in a linux program.
 **/
void main (void)
{
    SetGpioFunction(16, GPIO_OUT);        // LED on GPIO 16
    SetGpioFunction(12, GPIO_IN);        // Button on GPIO 12

    EnableGpioDetect(12, DETECT_RISING);
    ClearGpioInterrupt(12);

    irqRegister(49, button_handler, NULL);
    irqEnable(49);

    xTaskCreate(task1, (const signed char*)"LED_0", 128, NULL, 0, NULL);
    xTaskCreate(task2, (const signed char*)"LED_1", 128, NULL, 0, NULL);

    vTaskStartScheduler();

    /*
     *    We should never get here, but just in case something goes wrong,
     *    we'll place the CPU into a safe loop.
     */
    while(1) {
        ;
    }
}
