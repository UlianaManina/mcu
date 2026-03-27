#include "stdio.h"
#include "stdlib.h"
#include "pico/stdlib.h"
#include "stdio-task/stdio-task.h"
#include "adc-task/adc-task.h"
#include "protocol-task/protocol-task.h"
#include "led-task/led-task.h"
#include <string.h>


#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"



uint32_t mem(uint32_t addr)
{
    return *(volatile uint32_t*)addr;
}
void wmem(uint32_t addr, uint32_t data)
{
    *(volatile uint32_t*)addr = data;
}

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
void help_callback(const char* args)
{
    printf("\nAvailable commands:\n");
    printf("--------------------\n");
    help_handle();
    printf("--------------------\n");
}
void mem_callback(const char* args)
{
    uint32_t addr;

    // Читаем адрес в шестнадцатеричном формате
    if (sscanf(args, "%x", &addr) == 1) //==1 проверка что считан 1 элемент
    {  
        // Читаем значение по адресу
        printf("addr: 0x%08x -> value: 0x%08x (%u)\n", addr, read(addr), read(addr));
    } 
    else 
    {
        printf("Usage: mem <hex_address>\n");
        printf("Example: mem 20001000\n");
        return;
    }
}
void wmem_callback(const char* args)
{
    uint32_t addr, value;
    if (sscanf(args, "%x %x", &addr, &value) == 2) //==2 проверка что считано 2 элемент
    {  
        *(volatile uint32_t*)addr = value;
        printf(" in addr: 0x%08x -> new value: 0x%08x (%u)\n", addr, value, value);
    } 
    else 
    {
        printf("error\n");
        return;
    }
}

void adc_callback(const char* args)
{
    float voltage_V = adc_task_get_voltage();
    printf("%f\n", voltage_V);
}
void adc_temp_callback(const char* args)
{
    float temp_C = adc_task_get_temp();
    printf("%f\n", temp_C);
}


api_t device_api[] =
{
    {"off", led_off_callback, "stop working on the LED"},
    {"blink", led_blink_callback, "start blinkig on the LED"},
    {"on", led_on_callback, "start working on the LED"},
	{"version", version_callback, "get device name and firmware version"},
    {"set_period", led_blink_set_period_ms_callback, "change the period of blinking"},
    {"help", help_callback, "list all commands"},
    {"mem", mem_callback, "get data att address"},
    {"wmem", wmem_callback, "write data to address"},
    {"get_adc", adc_callback, "count adc" },
    {"get_temp", adc_temp_callback, "get temperature"},
	{NULL, NULL, NULL},
};


int main()
{   
    stdio_task_init();
    stdio_init_all();
    protocol_task_init(device_api);
    led_task_init();
    adc_task_init();
    
    while (1)
    {
        
        protocol_task_handle(stdio_task_handle());
        led_task_handle();
        
    }
}
