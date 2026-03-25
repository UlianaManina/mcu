#include "stdio.h"
#include "stdlib.h"
#include "pico/stdlib.h"
#include "stdio-task/stdio-task.h"

#include "protocol-task/protocol-task.h"
#include "led-task/led-task.h"


#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"

void version_callback(const char* args)
{
	printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
}

void led_on_callback(const char* args) 
{
    led_task_state_set(LED_STATE_ON);
    printf("LED ON\n");
}
void led_off_callback(const char* args) 
{
    led_task_state_set(LED_STATE_OFF);
    printf("LED OFF\n");
}
void led_blink_callback(const char* args) 
{
    led_task_state_set(LED_STATE_BLINK);
    printf("LED BLINK\n");
}
api_t device_api[] =
{
    {"off", led_off_callback, "stop working od the LED"},
    {"blink", led_blink_callback, "start blinkig of the LED"},
    {"on", led_on_callback, "start working od the LED"},
	{"version", version_callback, "get device name and firmware version"},
	{NULL, NULL, NULL},
};

int main()
{
    stdio_init_all();
    
    stdio_task_init();
    protocol_task_init(device_api);
    led_task_init();

    
    while (1)
    {
        
        protocol_task_handle(stdio_task_handle());
        led_task_handle();
    }
}
