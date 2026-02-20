#include "stdio.h"
#include "stdlib.h"
#include "pico/stdlib.h"
#include "stdio-task/stdio-task.h"

const uint LED_PIN = 25;
#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"

int main()
{
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    stdio_task_init();

    
    while (1)
    {
        stdio_task_handle();
        // char symbol = getchar();
        // sleep_ms(1000);
        // printf("received char: %c [ ASCII code: %d ]\n", symbol, symbol);
        // switch(symbol)
        // {
        // case 'e':
        //     gpio_put(LED_PIN, true);
        //     printf("led enable done\n");
        //     break;
        // case 'd':
        //     gpio_put(LED_PIN, false);
        //     printf("led disable done\n");
        //     break;
        // case 'v':
        //     printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
         
        //     break;

        // default:
        //     break;
        // }
    }
}
