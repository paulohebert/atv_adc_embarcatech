#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "button.h"
#include "ssd1306.h"

static volatile uint32_t last_time = 0;
static volatile bool pwm_enabled = true;
volatile uint8_t border = 0;

// Interrupção ao botão ser pressionado
void button_irq_handler(unsigned int gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    // Tratamento do bouncing
    if (current_time - last_time > 250000)
    {
        last_time = current_time;

        // Verifica o botão pressionado
        if (gpio == BTN_JOYSTICK_PIN)
        {
            // Alterna o estado do LED Verde
            gpio_put(LED_G_PIN, !gpio_get(LED_G_PIN));
            printf("Botão do Joystick Pressionado\n");
            printf("LED VERDE %s\n", gpio_get(LED_G_PIN) ? "LIGADO" : "DESLIGADO");
            draw_border(border, false);
            border = (border + 1) % 4;
        }
        else if (gpio == BTN_A_PIN)
        {
            printf("Botão A Pressionado\n");

            // Alterna o estado do PWM
            pwm_enabled = !pwm_enabled;
            printf("PWM = %s\n",
                   pwm_enabled ? "Ativado" : "Desativado");

            pwm_set_enabled(pwm_gpio_to_slice_num(LED_R_PIN), pwm_enabled);
            pwm_set_enabled(pwm_gpio_to_slice_num(LED_B_PIN), pwm_enabled);
        }
    }
}