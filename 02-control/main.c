#include "stdio.h"
#include "stdlib.h"
#include "pico/stdlib.h"
#include "stdio-task/stdio-task.h"

#include "protocol-task/protocol-task.h"
#include "led-task/led-task.h"


#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"

uint32_t read(uint32_t addr)
{
    return *(volatile uint32_t*)addr; // принимает адресс превращает его в указатель и возвращает прочитанный
}
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

void led_blink_set_period_ms_callback(const char* args)
{
    uint32_t period_ms = 0;
    sscanf(args, "%u", &period_ms);
    if (period_ms == 0)
    {
        printf("error: period = 0 ms\n");
        return;
    }
    led_task_set_blink_period_ms(period_ms);
    printf("period has been changed \n");
}
api_t device_api[] =
{
    {"off", led_off_callback, "stop working on the LED"},
    {"blink", led_blink_callback, "start blinkig on the LED"},
    {"on", led_on_callback, "start working on the LED"},
	{"version", version_callback, "get device name and firmware version"},
    {"set_period", led_blink_set_period_ms_callback, "change the period of blinking"},
	{NULL, NULL, NULL},
};

int main()
{   
     stdio_task_init();
    stdio_init_all();
    

    protocol_task_init(device_api);
    led_task_init();
    
    
    while (1)
    {
        
        protocol_task_handle(stdio_task_handle());
        led_task_handle();
        
    }
}
