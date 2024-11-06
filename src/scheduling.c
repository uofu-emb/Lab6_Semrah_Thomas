#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/cyw43_arch.h>
#include <unity.h>

#include "scheduling.h"

SemaphoreHandle_t semaphore;

int* lower_count;
int* higher_count;

void side_thread_lower(void *params)
{
    int status = xSemaphoreTake(semaphore, portMAX_DELAY);

	while (1) {
        vTaskDelay(1000);

        if(*lower_count == 100) {
            xSemaphoreGive(semaphore);
            return;
        }

        *lower_count++;
	}
}

void side_thread_higher(void *params)
{
    int status = xSemaphoreTake(semaphore, portMAX_DELAY);

	while (1) {
        vTaskDelay(1000);

        if(*higher_count == 100) {
            xSemaphoreGive(semaphore);
            return;
        }

        *higher_count++;
	}
}

void supervisor_thread(void *params)
{
    xSemaphoreGive(semaphore);

    TaskHandle_t lower_worker, higher_worker;

    xTaskCreate(side_thread_lower, "LowerWorker",
        WORKER_TASK_LOWER_STACK_SIZE, NULL, WORKER_TASK_LOWER_PRIORITY, &lower_worker);

    vTaskDelay(10);

    xTaskCreate(side_thread_higher, "HigherWorker",
        WORKER_TASK_HIGHER_STACK_SIZE, NULL, WORKER_TASK_HIGHER_PRIORITY, &higher_worker);

    vTaskDelay(10000);

}

void startInversion(SemaphoreHandle_t *inputSemaphore, int* lower, int* higher) {
    semaphore = *inputSemaphore;
    lower_count = lower;
    higher_count = higher;

    TaskHandle_t supervisor_handler;

    xTaskCreate(supervisor_thread, "SupervisorThread",
            SUPERVISOR_TASK_STACK_SIZE, NULL, SUPERVISOR_TASK_PRIORITY , &supervisor_handler);
}