#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_PLACA 2

void app_main(void)
{
    gpio_reset_pin(LED_PLACA); //pin reset
    gpio_set_direction(LED_PLACA, GPIO_MODE_OUTPUT);

    while(1)
    {
        printf("Ligando o LED ✔\n");
        gpio_set_level(LED_PLACA, 1); //equivalente ao digital write 
        vTaskDelay(1000 / portTICK_PERIOD_MS); //portTICK_PERIOD_MS é o tempo que ele vai levar para ter um tick na base de milissegundos

        printf("Desligando o LED ❌\n");
        gpio_set_level(LED_PLACA, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}