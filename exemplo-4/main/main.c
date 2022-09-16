#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_adc_cal.h"

#define PINO_LED 2

//adc
#define DEFAULT_VREF 1100 //use adc3_vref_gpio() to obtain a better estimate
#define NO_OF_SAMPLES 50 //multisampling, dá uma estabilidade um pouco melhor

// control LED Brightness With a Potentiometer

SemaphoreHandle_t semaphore = NULL;

//configuração do ADC
static const adc_channel_t channel = ADC_CHANNEL_6; //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_bits_width_t width = ADC_WIDTH_BIT_12;
static const adc_atten_t atten = ADC_ATTEN_DB_11; //dá 3,9v, é o mais perto de 3,3v

//configuração do LED
uint32_t tempo_delay = 1000; //tempo de delay em ms
bool estado_led = 0;

void TarefaAdc(void *parameters)
{
    //configure ADC
    adc1_config_width(width);
    adc1_config_channel_atten(channel, atten);

    for(;;)
    {
        uint32_t adc_reading = 0;

        //Multisampling
        for(int i = 0; i < NO_OF_SAMPLES; i++)
        {
            adc_reading += adc1_get_raw((adc1_channel_t)channel);
        }
        adc_reading /= NO_OF_SAMPLES;
        printf("ADC:%d\n", adc_reading);
        tempo_delay = adc_reading;
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void TarefaLed(void *parameters)
{
    uint32_t tempo_led = 0;

    //setup do LED
    gpio_reset_pin(PINO_LED);
    gpio_set_direction(PINO_LED, GPIO_MODE_OUTPUT);

    for(;;)
    {
       estado_led = !estado_led;
       gpio_set_level(PINO_LED, estado_led);

        if(semaphore != NULL)
        {
            if(xSemaphoreTake(semaphore, 10))
            {
                tempo_led = tempo_delay;
                xSemaphoreGive(semaphore);
            }
        }

        vTaskDelay(tempo_delay / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    semaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(semaphore);

    xTaskCreate(TarefaLed, "led", 2048, NULL, 0, NULL);
    xTaskCreate(TarefaAdc, "adc", 2048, NULL, 0, NULL);
}