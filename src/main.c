#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "ssd1306.h"
#include "button.h"
#include "joystick.h"

#define PWM_CLKDIV 16.0
#define PWM_WRAP 4096

// Configura o pino GPIO
void gpio_setup(uint8_t pin, bool dir)
{
    gpio_init(pin);
    gpio_set_dir(pin, dir);
    if (dir == GPIO_IN)
        gpio_pull_up(pin);
}

// Configura o pino GPIO com PWM
void pwm_setup(uint8_t pin)
{
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_set_wrap(slice_num, PWM_WRAP);
    pwm_set_clkdiv(slice_num, PWM_CLKDIV);
    pwm_set_enabled(slice_num, true);
}

void setup()
{
    stdio_init_all();

    // Configura a entrada analógica do joystick
    adc_init();
    adc_gpio_init(VRX_PIN);
    adc_gpio_init(VRY_PIN);

    // Configura o botão do joystick e o botão A
    gpio_setup(BTN_JOYSTICK_PIN, GPIO_IN);
    gpio_setup(BTN_A_PIN, GPIO_IN);

    // Configura o LED RGB
    gpio_setup(LED_R_PIN, GPIO_OUT);
    gpio_setup(LED_G_PIN, GPIO_OUT);
    gpio_setup(LED_B_PIN, GPIO_OUT);
    pwm_setup(LED_R_PIN);
    pwm_setup(LED_B_PIN);

    // Configura as interrupções dos botões
    gpio_set_irq_enabled_with_callback(BTN_JOYSTICK_PIN, GPIO_IRQ_EDGE_FALL, true, &button_irq_handler);
    gpio_set_irq_enabled(BTN_A_PIN, GPIO_IRQ_EDGE_FALL, true);

    // Configura a comunicação I2C com SSD1306
    // Inicializa o I2C em 400Khz
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                 // Define o SDA do I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                 // Define o SCL do I2C
    gpio_pull_up(I2C_SDA);                                     // Colocar um Resitor Pull Up para o data line
    gpio_pull_up(I2C_SCL);                                     // Colocar um Resitor Pull Up para o clock line
    ssd1306_init(WIDTH, HEIGHT, false, I2C_ADDRESS, I2C_PORT); // Inicializa o display
    ssd1306_config();                                          // Configura o display
    ssd1306_send_data();                                       // Envia os dados para o display

    // Limpa o display SSD1306
    ssd1306_fill(false);
    ssd1306_send_data();
}

int main()
{
    setup();

    uint16_t vrx_value, vry_value; // Variáveis para armazenar os valores do joystick (eixos X e Y)
    uint8_t x = 8, y = 4; // Posição central do quadrado no display 
    while (true)
    {
        joystick_read_axis(&vrx_value, &vry_value); // Lê os valores dos eixos do joystick
        printf("X = %04d | Y = %04d\n", vrx_value, vry_value);

        // Muda a potência dos LEDs
        pwm_set_gpio_level(LED_B_PIN, abs(vry_value - 2048) > 150 ? abs(vry_value - 2048) * 2 : 0);
        pwm_set_gpio_level(LED_R_PIN, abs(vrx_value - 2048) > 150 ? abs(vrx_value - 2048) * 2 : 0);

        // Apaga, no display, o quadrado na posição anterior
        ssd1306_rect(8 * y, 8 * x, 8, 8, false, true);

        // Mudar a posição do quadrado baseado no joystick
        if (vrx_value > 2200 && x > 0)
        {
            x--;
        }
        else if (vrx_value < 1900 && x < 15)
        {
            x++;
        }

        if (vry_value > 2200 && y > 0)
        {
            y--;
        }
        else if (vry_value < 1900 && y < 7)
        {
            y++;
        }

        // Desenha o tipo de borda atual
        draw_border(border, true);

        // Desenha o quadrado na nova posição
        ssd1306_rect(8 * y, 8 * x, 8, 8, true, true);
        // Atualiza o display
        ssd1306_send_data(); 

        sleep_ms(100);
    }
}
