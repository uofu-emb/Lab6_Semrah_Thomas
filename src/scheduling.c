#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/cyw43_arch.h>

#include "threads_functions.h"

#define SUPERVISOR_TASK_PRIORITY      ( tskIDLE_PRIORITY + 3UL )
#define SUPERVISOR_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

#define WORKER_TASK_LOWER_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define WORKER_TASK_LOWER_STACK_SIZE configMINIMAL_STACK_SIZE

#define WORKER_TASK_HIGHER_PRIORITY      ( tskIDLE_PRIORITY + 2UL )
#define WORKER_TASK_HIGHER_STACK_SIZE configMINIMAL_STACK_SIZE

SemaphoreHandle_t semaphore;

void side_thread_lower(void *params)
{
    printf("Starting lower priority thread.\n");

    xSemaphoreTake(semaphore, portMAX_DELAY);

    printf("Lowe priority thread has acquired the semaphore.\n");

    int count = 0;

	while (1) {
        printf("Lower Priority Executing. Current Count: %d\n", count);
        vTaskDelay(1000);

        if(count = 100) {
            xSemaphoreGive(semaphore);
            return;
        }
	}
}

void side_thread_higher(void *params)
{
    printf("Starting higher priority thread.\n");

    xSemaphoreTake(semaphore, portMAX_DELAY);

    printf("Higher priority thread has acquired the semaphore.\n");

    int count = 0;

	while (1) {
        printf("Higher Priority Executing. Current Count: %d\n", count);
        vTaskDelay(1000);

        if(count = 100) {
            xSemaphoreGive(semaphore);
            return;
        }
	}
}

void supervisor_thread(void *params)
{
    semaphore = xSemaphoreCreateBinary();

    TaskHandle_t lower_worker, higher_worker;

    xTaskCreate(side_thread_lower, "LowerWorker",
        WORKER_TASK_LOWER_STACK_SIZE, NULL, WORKER_TASK_LOWER_PRIORITY, &lower_worker);

    vTaskDelay(1000);

    xTaskCreate(side_thread_higher, "HigherWorker",
        WORKER_TASK_HIGHER_STACK_SIZE, NULL, WORKER_TASK_HIGHER_PRIORITY, &higher_worker);

}

int main(void)
{
    stdio_init_all();
    sleep_ms(5000);
    TaskHandle_t supervisor_handler;
    xTaskCreate(supervisor_thread, "SupervisorThread",
                SUPERVISOR_TASK_STACK_SIZE, NULL, SUPERVISOR_TASK_PRIORITY , &supervisor_handler);
    vTaskStartScheduler();
	return 0;
}