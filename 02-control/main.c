#include "stdio.h"
#include "stdlib.h"
#include "pico/stdlib.h"
#include "stdio-task/stdio-task.h"

#include "protocol-task/protocol-task.h"
#include "led-task/led-task.h"
#include "string.h"


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
// void mem_callback(const char* args)
// {
//     if (args == NULL || strlen(args) == 0) {
//         printf("Usage: mem <hex_address>\r\n");
//         return;
//     }
    
//     // Используем strtoul для парсинга hex (поддерживает 0x префикс)
//     char *endptr;
//     uint32_t addr = (uint32_t)strtoul(args, &endptr, 0);  // 0 = автоопределение формата
    
//     // Проверяем, что парсинг успешен
//     if (endptr == args) {
//         printf("invalid address format\r\n");
//         return;
//     }
    
//     // Проверяем выравнивание
//     if (addr % 4 != 0) {
//         printf("address not word-aligned (must be multiple of 4)\r\n");
//         return;
//     }
    
//     // Читаем и выводим значение в hex и decimal
//     uint32_t value = mem(addr);
//     printf("0x%08X = 0x%08X (%u)\r\n", addr, value, value);
// }
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

// void wmem_callback(const char* args)
// {
//     if (args == NULL || strlen(args) == 0) {
//         printf("Usage: wmem <hex_address> <hex_value>\r\n");
//         return;
//     }
    
//     uint32_t addr, value;
//     char *endptr;
    
//     // Парсим первый аргумент (адрес) - поддерживаем hex с 0x
//     addr = (uint32_t)strtoul(args, &endptr, 0);
//     if (endptr == args) {
//         printf("invalid address format\r\n");
//         return;
//     }
    
//     // Пропускаем пробелы и парсим второй аргумент (значение)
//     while (*endptr == ' ') endptr++;
//     if (*endptr == '\0') {
//         printf("missing value argument\r\n");
//         return;
//     }
    
//     value = (uint32_t)strtoul(endptr, NULL, 0);
    
//     // Проверяем выравнивание адреса
//     if (addr % 4 != 0) {
//         printf("address not word-aligned (must be multiple of 4)\r\n");
//         return;
//     }
    
//     wmem(addr, value);
//     printf("wrote 0x%08X to 0x%08X\r\n", value, addr);
// }
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
