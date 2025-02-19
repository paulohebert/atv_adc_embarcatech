#ifndef JOYSTICK_H
#define JOYSTICK_H

// Definições dos pinos para o joystick
#define VRX_PIN 26
#define VRY_PIN 27

#define ADC_MAX 4095

void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value);

#endif // JOYSTICK_H