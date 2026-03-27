#include "hardware/adc.h"
#include "stdio.h"
#include "adc-task.h"
#include "pico/stdlib.h"

#include "hardware/gpio.h"

const uint ADC_PIN = 26;
const uint ADC_CHANNEL = 0;
const uint ADC_temp = 4;

void adc_task_init()
{
	adc_init();
	adc_gpio_init(ADC_PIN);
	adc_set_temp_sensor_enabled(true);
}
float adc_task_get_voltage()
{
	adc_select_input(ADC_CHANNEL);
	uint16_t voltage_counts = adc_read();
	float voltage_V =  (float)voltage_counts / 4095.0f * 3.3f;
	return voltage_V;
}

float adc_task_get_temp()
{
	adc_select_input(ADC_temp);
	uint16_t temp_counts = adc_read();
	float temp_V =  (float)temp_counts / 4095.0f * 3.3f;
	float temp_C = 27.0f - (temp_V - 0.706f) / 0.001721f;
	return temp_C;
}

