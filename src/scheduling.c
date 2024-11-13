#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <unity.h>
#include <task.h>

#include "scheduling.h"

SemaphoreHandle_t semaphore;

int* lower_count;
int* higher_count;
int kys = 1;

void side_thread_lower(void *params)
{
    printf("Lower waiting for semaphore.\n");
    int status = xSemaphoreTake(semaphore, portMAX_DELAY);
    printf("Lower has semaphore.\n");

	while (kys) {
        vTaskDelay(1000);

        if(*lower_count == 15) {
            xSemaphoreGive(semaphore);
            vTaskDelete(NULL);
        }
        printf("Lower: %d\n", *lower_count);

        *lower_count = *lower_count + 1;
	}
    xSemaphoreGive(semaphore);
    vTaskDelete(NULL);
}

void side_thread_higher(void *params)
{
    printf("Higher waiting for semaphore.\n");
    int status = xSemaphoreTake(semaphore, portMAX_DELAY);
    printf("Higher has semaphore.\n");

	while (kys) {
        vTaskDelay(1000);

        if(*higher_count == 20) {
            xSemaphoreGive(semaphore);
            vTaskDelete(NULL);
        }
        printf("Higher: %d\n", *higher_count);

        *higher_count = *higher_count + 1;
	}
    xSemaphoreGive(semaphore);
    vTaskDelete(NULL);
}

void supervisor_thread(void *params)
{
    printf("Supervisor waiting for semaphore.\n");
    xSemaphoreGive(semaphore);
    printf("Supervisor has given semaphore.\n");
    kys = 1;

    TaskHandle_t lower_worker, higher_worker;

    printf("Starting Lower Worker Thread.\n");
    xTaskCreate(side_thread_lower, "LowerWorker",
        WORKER_TASK_LOWER_STACK_SIZE, NULL, WORKER_TASK_LOWER_PRIORITY, &lower_worker);

    vTaskDelay(10);

    printf("Starting Higher Worker Thread.\n");
    xTaskCreate(side_thread_higher, "HigherWorker",
        WORKER_TASK_HIGHER_STACK_SIZE, NULL, WORKER_TASK_HIGHER_PRIORITY, &higher_worker);

    vTaskDelay(20000);
    kys = 0;
    vTaskDelay(2000);
    printf("Killing Supervisor\n");
    vTaskDelete(NULL);
}

void startInversion(SemaphoreHandle_t *inputSemaphore, int* lower, int* higher) {
    semaphore = *inputSemaphore;
    lower_count = lower;
    higher_count = higher;

    TaskHandle_t supervisor_handler;

    printf("Starting Supervisor Thread.\n");
    xTaskCreate(supervisor_thread, "SupervisorThread",
            SUPERVISOR_TASK_STACK_SIZE, NULL, SUPERVISOR_TASK_PRIORITY , &supervisor_handler);

    vTaskDelay(25000);
}

void busy_busy(void)
{
    for (int i = 0; ; i++);
}

void busy_yield(void)
{
    for (int i = 0; ; i++) {
        taskYIELD();
    }
}

void busy_busy_thread(void *params){
    busy_busy();
}

void busy_yield_thread(void *params){
    busy_yield();
}

void thirdActivityExecuter(TaskFunction_t threadOneEntry, int threadOnePriority, configRUN_TIME_COUNTER_TYPE *threadOneRuntime,
TaskFunction_t threadTwoEntry, int threadTwoPriority, configRUN_TIME_COUNTER_TYPE *threadTwoRuntime)
{
    printf("Starting thirdActivityExecuter\n");
    TaskHandle_t threadOneHandler, threadTwoHandler;
    
    xTaskCreate(threadOneEntry, "ThreadOne", configMINIMAL_STACK_SIZE,
            NULL, threadOnePriority, &threadOneHandler);
    vTaskDelay(100);
    xTaskCreate(threadTwoEntry, "ThreadTwo", configMINIMAL_STACK_SIZE,
            NULL, threadTwoPriority, &threadTwoHandler);

    vTaskDelay(10000);

    vTaskSuspend(threadOneHandler);
    vTaskSuspend(threadTwoHandler);

    printf("thirdActivityExecuter Completed running logging time and closing threads\n");
    *threadOneRuntime = ulTaskGetRunTimeCounter(threadOneHandler);
    *threadTwoRuntime = ulTaskGetRunTimeCounter(threadTwoHandler);
    // printf("Thread one runtime: %u\n", (unsigned int)ulTaskGetRunTimeCounter(threadOneHandler));
    // printf("Thread two runtime: %u\n", (unsigned int)ulTaskGetRunTimeCounter(threadTwoHandler));

    vTaskDelete(threadOneHandler);
    vTaskDelete(threadTwoHandler);
}