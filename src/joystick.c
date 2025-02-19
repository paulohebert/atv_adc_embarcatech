#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "joystick.h"

#define ADC_CHANNEL_X 0
#define ADC_CHANNEL_Y 1

// Função para ler os valores dos eixos do joystick (X e Y)
void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value)
{
    // Ler o eixo X do joystick
    adc_select_input(ADC_CHANNEL_X);
    sleep_us(2);
    *vrx_value = adc_read();

    // Ler o eixo Y do joystick
    adc_select_input(ADC_CHANNEL_Y);
    sleep_us(2);
    *vry_value = adc_read();
}