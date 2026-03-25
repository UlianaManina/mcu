#pragma once
typedef enum
{
	LED_STATE_OFF = 0,
	LED_STATE_ON = 1,
	LED_STATE_BLINK = 2,
} led_state_t;


// typedef void(*command_callback_t)(const char* args);
// typedef struct
// {
// 	const char* command_name;
// 	command_callback_t command_callback;
// 	const char* command_help;
// } api_t;

void led_task_init();
void led_task_handle();
void led_task_state_set(led_state_t state);