#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <unity.h>

#define SUPERVISOR_TASK_PRIORITY (tskIDLE_PRIORITY + 10)
#define SUPERVISOR_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

#define WORKER_TASK_LOWER_PRIORITY (tskIDLE_PRIORITY + 3)
#define WORKER_TASK_LOWER_STACK_SIZE configMINIMAL_STACK_SIZE

#define WORKER_TASK_HIGHER_PRIORITY (tskIDLE_PRIORITY + 4)
#define WORKER_TASK_HIGHER_STACK_SIZE configMINIMAL_STACK_SIZE

void side_thread_lower(void *params);
void side_thread_higher(void *params);
void supervisor_thread(void *params);
void startInversion(SemaphoreHandle_t *inputSemaphore, int* lower, int* higher);
void busy_busy(void);
void busy_yield(void);
void busy_busy_thread(void *params);
void busy_yield_thread(void *params);
void thirdActivityExecuter(TaskFunction_t threadOneEntry, int threadOnePriority, configRUN_TIME_COUNTER_TYPE *threadOneRuntime,
TaskFunction_t threadTwoEntry, int threadTwoPriority, configRUN_TIME_COUNTER_TYPE *threadTwoRuntime);