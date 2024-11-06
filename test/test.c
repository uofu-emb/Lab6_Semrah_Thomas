#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/cyw43_arch.h>
#include <unity.h>
#include "unity_config.h"

#include "scheduling.h"

void setUp(void) {}

void tearDown(void) {}

void test_priority_inversion(void){
    SemaphoreHandle_t semaphore = xSemaphoreCreateBinary();
    int lower = 0;
    int higher = 0;

    startInversion(&semaphore, &lower, &higher);

    sleep_ms(10000);

    TEST_ASSERT_TRUE_MESSAGE(lower > higher, "Priority inversion did not occur.");
}


int main(void)
{
    stdio_init_all();
    sleep_ms(10000);
    UNITY_BEGIN();
    RUN_TEST(test_priority_inversion);
    vTaskStartScheduler();
	return UNITY_END();
}