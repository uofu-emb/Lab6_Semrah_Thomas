#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <unity.h>
#include "unity_config.h"

#include "scheduling.h"

#define RUNNNER_TASK_PRIORITY (tskIDLE_PRIORITY + 20)
#define RUNNNER_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

configRUN_TIME_COUNTER_TYPE threadOneRuntime, threadTwoRuntime;

void setUp(void) {}

void tearDown(void) {}

void test_priority_inversion(void){
    SemaphoreHandle_t semaphore = xSemaphoreCreateBinary();
    int lower = 0;
    int higher = 0;

    printf("Testing startInversion in test_priority_inversion.\n");
    startInversion(&semaphore, &lower, &higher);

    printf("Lower: %d\nHigher: %d\n", lower, higher);
    TEST_ASSERT_TRUE_MESSAGE(lower > higher, "Priority inversion did not occur.");
}

void test_mutex(void){
    SemaphoreHandle_t semaphore = xSemaphoreCreateMutex();
    int lower = 0;
    int higher = 0;

    printf("Testing startInversion in tset_mutex.\n");
    startInversion(&semaphore, &lower, &higher);

    printf("Lower: %d\nHigher: %d\n", lower, higher);
    TEST_ASSERT_TRUE_MESSAGE(lower > higher, "Priority inversion did not occur.");
}

void test_samepri_busy_busy(void){
    printf("Testing test_samepri_busy_busy.\n");
    thirdActivityExecuter(busy_busy_thread, tskIDLE_PRIORITY + 3, &threadOneRuntime, busy_busy_thread, tskIDLE_PRIORITY + 3, &threadTwoRuntime);

    // printf("Thread one runtime: %llu\n", threadOneRuntime);
    // printf("Thread two runtime: %llu\n", threadTwoRuntime);

    TEST_ASSERT_TRUE_MESSAGE(threadOneRuntime > 4000000, "Thread one ran too short.");
    TEST_ASSERT_TRUE_MESSAGE(threadTwoRuntime > 4000000, "Thread two ran too short.");
}

void test_samepri_yield_yield(void){
    printf("Testing test_samepri_yield_yield.\n");
    thirdActivityExecuter(busy_yield_thread, tskIDLE_PRIORITY + 3, &threadOneRuntime, busy_yield_thread, tskIDLE_PRIORITY + 3, &threadTwoRuntime);
    
    // printf("Thread one runtime: %llu\n", threadOneRuntime);
    // printf("Thread two runtime: %llu\n", threadTwoRuntime);

    TEST_ASSERT_TRUE_MESSAGE(threadOneRuntime > 4000000, "Thread one ran too short.");
    TEST_ASSERT_TRUE_MESSAGE(threadTwoRuntime > 4000000, "Thread two ran too short.");
}

void test_samepri_busy_yield(void){
    printf("Testing test_samepri_busy_yield.\n");
    thirdActivityExecuter(busy_busy_thread, tskIDLE_PRIORITY + 3, &threadOneRuntime, busy_yield_thread, tskIDLE_PRIORITY + 3, &threadTwoRuntime);
    
    // printf("Thread one runtime: %llu\n", threadOneRuntime);
    // printf("Thread two runtime: %llu\n", threadTwoRuntime);

    TEST_ASSERT_TRUE_MESSAGE(threadOneRuntime > 4000000, "Thread one ran too short.");
    TEST_ASSERT_TRUE_MESSAGE(threadTwoRuntime < 4000000, "Thread two ran too long.");
}

void test_diffpri_busy_busy(void){
    printf("Testing test_diffpri_busy_busy.\n");
    thirdActivityExecuter(busy_busy_thread, tskIDLE_PRIORITY + 4, &threadOneRuntime, busy_busy_thread, tskIDLE_PRIORITY + 3, &threadTwoRuntime);
    
    // printf("Thread one runtime: %llu\n", threadOneRuntime);
    // printf("Thread two runtime: %llu\n", threadTwoRuntime);

    TEST_ASSERT_TRUE_MESSAGE(threadOneRuntime > 4000000, "Thread one ran too short.");
    TEST_ASSERT_TRUE_MESSAGE(threadTwoRuntime < 4000000, "Thread two ran too long.");
}

void test_diffpri_busy_busy2(void){
    printf("Testing test_diffpri_busy_busy2.\n");
    thirdActivityExecuter(busy_busy_thread, tskIDLE_PRIORITY + 3, &threadOneRuntime, busy_busy_thread, tskIDLE_PRIORITY + 3, &threadTwoRuntime);
    
    // printf("Thread one runtime: %llu\n", threadOneRuntime);
    // printf("Thread two runtime: %llu\n", threadTwoRuntime);

    TEST_ASSERT_TRUE_MESSAGE(threadOneRuntime > 4000000, "Thread one ran too short.");
    TEST_ASSERT_TRUE_MESSAGE(threadTwoRuntime > 4000000, "Thread two ran too short.");
}

void test_diffpri_yield_yield(void){
    printf("Testing test_diffpri_yield_yield.\n");
    thirdActivityExecuter(busy_yield_thread, tskIDLE_PRIORITY + 4, &threadOneRuntime, busy_yield_thread, tskIDLE_PRIORITY + 3, &threadTwoRuntime);
    
    // printf("Thread one runtime: %llu\n", threadOneRuntime);
    // printf("Thread two runtime: %llu\n", threadTwoRuntime);

    TEST_ASSERT_TRUE_MESSAGE(threadOneRuntime > 4000000, "Thread one ran too short.");
    TEST_ASSERT_TRUE_MESSAGE(threadTwoRuntime < 4000000, "Thread two ran too long.");
}

void runner_thread(void *params){
    for(;;){
        UNITY_BEGIN();
        printf("Starting Unity Testing\n");
        UNITY_BEGIN();
        RUN_TEST(test_priority_inversion);
        RUN_TEST(test_mutex);
        RUN_TEST(test_samepri_busy_busy);
        RUN_TEST(test_samepri_yield_yield);
        RUN_TEST(test_samepri_busy_yield);
        RUN_TEST(test_diffpri_busy_busy);
        RUN_TEST(test_diffpri_busy_busy2);
        RUN_TEST(test_diffpri_yield_yield);
        UNITY_END();
        vTaskDelay(1000);
    }
}


int main(void)
{
    stdio_init_all();
    sleep_ms(10000);
    xTaskCreate(runner_thread, "runner_thread", RUNNNER_TASK_STACK_SIZE, NULL, RUNNNER_TASK_PRIORITY, NULL);
    vTaskStartScheduler();
	return 0;
}