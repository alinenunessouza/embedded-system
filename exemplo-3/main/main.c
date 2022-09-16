#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"

#define PINO_LED 2

//pisca led com tarefas

SemaphoreHandle_t semaphore = NULL;

uint32_t tempo_delay = 1000; //tempo de delay em ms
bool estado_led = 0;

void TarefaLed(void *parameters)
{
    //setup do LED
    gpio_reset_pin(PINO_LED);
    gpio_set_direction(PINO_LED, GPIO_MODE_OUTPUT);

    for(;;)
    {
       estado_led = !estado_led;
       gpio_set_level(PINO_LED, estado_led);
       vTaskDelay(tempo_delay / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    xTaskCreate(TarefaLed, "led", 2048, NULL, 0, NULL);
}