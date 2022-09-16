#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
# include "freertos/semphr.h"

TaskHandle_t th_task[2];
SemaphoreHandle_t semaphore = NULL;

uint32_t variavel_critica = 0;

void incrementa_variavel_critica()
{
    if(variavel_critica < 1000)
    {
        variavel_critica++;
    }
    else
    {
        variavel_critica = 0;
    }
    printf("%d\n", variavel_critica);
}

void CodigoTarefa(void *parameters)
{
    uint8_t taskId = (uint8_t) parameters;

    printf("Início da tarefa %d\n", taskId);

    for(;;)
    {
        /*
        printf("Tarefa... %d\n", taskId);
        vTaskDelay(1000 / portTICK_PERIOD_MS); //portTICK_PERIOD_MS é o tempo que ele vai levar para ter um tick na base de milissegundos
        */
       if(semaphore != NULL)
       {
            if(xSemaphoreTake(semaphore, 10))
            {
                incrementa_variavel_critica();
                vTaskDelay(1000/portTICK_PERIOD_MS);
                xSemaphoreGive(semaphore);
            }
            else
            {  
                printf("%d não pegou a sinaleira\n", taskId);
            }
        }
    }

    vTaskDelete(th_task);
}

void app_main(void)
{
    semaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(semaphore);

    //xTaskCreate(CodigoTarefa, "tarefa 1", 1024, NULL, 0, &th_task);
    xTaskCreate(CodigoTarefa, "tarefa 1", 2048, (void *) 1, 0, &th_task[0]);
    //vTaskDelay(2000 / portTICK_PERIOD_MS);
    xTaskCreate(CodigoTarefa, "tarefa 2", 2048, (void *) 2, 0, &th_task[1]);
    //vTaskDelay(2000 / portTICK_PERIOD_MS);
    //vTaskDelete(th_task[0]);
}