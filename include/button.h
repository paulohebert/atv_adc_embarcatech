#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <stdint.h>

// Define os pinos dos LEDs
#define LED_R_PIN 13
#define LED_G_PIN 11
#define LED_B_PIN 12

// Define os pinos dos botões
#define BTN_JOYSTICK_PIN 22
#define BTN_A_PIN 5

extern volatile uint8_t border;

void button_irq_handler(unsigned int gpio, uint32_t events);

#endif // BUTTON_HANDLER_H