#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/cyw43_arch.h>

#include "threads_functions.h"

#define SUPERVISOR_TASK_PRIORITY (tskIDLE_PRIORITY + 10)
#define SUPERVISOR_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

#define WORKER_TASK_LOWER_PRIORITY (tskIDLE_PRIORITY + 3)
#define WORKER_TASK_LOWER_STACK_SIZE configMINIMAL_STACK_SIZE

#define WORKER_TASK_HIGHER_PRIORITY (tskIDLE_PRIORITY + 4)
#define WORKER_TASK_HIGHER_STACK_SIZE configMINIMAL_STACK_SIZE

SemaphoreHandle_t semaphore;

void side_thread_lower(void *params)
{
    printf("Starting lower priority thread.\n");

    int status = xSemaphoreTake(semaphore, portMAX_DELAY);

    if(status == pdTRUE) {
        printf("Lower priority thread has acquired the semaphore.\n");
    }

    int count = 0;

	while (1) {
        printf("Lower Priority Executing. Current Count: %d\n", count);
        vTaskDelay(1000);

        if(count == 100) {
            xSemaphoreGive(semaphore);
            return;
        }

        count++;
	}
}

void side_thread_higher(void *params)
{
    printf("Starting higher priority thread.\n");

    int status = xSemaphoreTake(semaphore, portMAX_DELAY);

    if(status == pdTRUE) {
        printf("Higher priority thread has acquired the semaphore.\n");
    }

    int count = 0;

	while (1) {
        printf("Higher Priority Executing. Current Count: %d\n", count);
        vTaskDelay(1000);

        if(count == 100) {
            xSemaphoreGive(semaphore);
            return;
        }

        count++;
	}
}

void supervisor_thread(void *params)
{
    semaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(semaphore);

    TaskHandle_t lower_worker, higher_worker;

    xTaskCreate(side_thread_lower, "LowerWorker",
        WORKER_TASK_LOWER_STACK_SIZE, NULL, WORKER_TASK_LOWER_PRIORITY, &lower_worker);

    vTaskDelay(10000);

    printf("Starting High Priority Thread.\n");

    xTaskCreate(side_thread_higher, "HigherWorker",
        WORKER_TASK_HIGHER_STACK_SIZE, NULL, WORKER_TASK_HIGHER_PRIORITY, &higher_worker);

    vTaskDelay(10000);

}

int main(void)
{
    stdio_init_all();
    sleep_ms(10000);
    TaskHandle_t supervisor_handler;
    xTaskCreate(supervisor_thread, "SupervisorThread",
                SUPERVISOR_TASK_STACK_SIZE, NULL, SUPERVISOR_TASK_PRIORITY , &supervisor_handler);
    vTaskStartScheduler();
	return 0;
}