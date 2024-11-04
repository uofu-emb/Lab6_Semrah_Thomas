#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/sync.h>

#define OUT_PIN 1
#define DELAY_MS 500
#define IN_PIN 4

struct task_args
{
    bool recieved_signal;
    uint gpio;
};

int toggle = 1;
void irq_callback(uint gpio, uint32_t event_mask)
{
    //Work
    int count = 0;
    for(int i = 0; i < 1000000; i++)
    {
        count++;
    }

    if (gpio != IN_PIN) return;
    toggle = !toggle;
    if (event_mask & GPIO_IRQ_EDGE_RISE) {
        gpio_put(OUT_PIN, true);
    } else if (event_mask & GPIO_IRQ_EDGE_FALL) {
        gpio_put(OUT_PIN, false);
    }
}

int main(void)
{
    stdio_init_all();

    gpio_init(IN_PIN);
    gpio_set_dir(IN_PIN, GPIO_IN);

    gpio_init(OUT_PIN);
    gpio_set_dir(OUT_PIN, GPIO_OUT);
    gpio_put(OUT_PIN, toggle);

    gpio_set_irq_enabled_with_callback(IN_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL , true, irq_callback);
    
    while(1) __nop();
    
    return 0;
}