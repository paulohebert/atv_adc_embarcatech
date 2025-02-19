# Atividade - Conversores A/D

## 🎯 Descrição
Este projeto tem como objetivo utilizar um joystick analógico para controlar a intensidade de dois LEDs RGB e a posição de um quadrado em um display SSD1306. Além disso, botões adicionais permitem alternar estados dos LEDs e modificar a borda do display.

## 🔧 Componentes Utilizados
- **Placa BitDogLab com RP2040**
- **LED RGB** (conectado às GPIOs 11, 12 e 13)
- **Joystick** (conectado aos GPIOs 26 e 27)
- **Botão do Joystick** (conectado à GPIO 22)
- **Botão A** (conectado à GPIO 5)
- **Display SSD1306** (conectado via I2C nas GPIOs 14 e 15)

## 🚀 Funcionalidades
- O LED **Azul** ajusta seu brilho conforme o eixo **Y** do joystick.
- O LED **Vermelho** ajusta seu brilho conforme o eixo **X** do joystick.
- Um quadrado de **8x8 pixels** se move no **display SSD1306** proporcionalmente aos valores do joystick.
- O **botão do joystick**:
  - Alterna o estado do **LED Verde** a cada acionamento.
  - Modifica a borda do display a cada acionamento.
- O **botão A** ativa ou desativa o PWM dos LEDs.

## 💻 Configuração do Ambiente
1. Instale o **Pico SDK** conforme a documentação oficial do Raspberry Pi.
2. Configure o **CMake** e o **Visual Studio Code** para compilar códigos para o Raspberry Pi Pico.
3. Integre o **Wokwi** ao VS Code para simulação do circuito.
4. Clone este repositório:
   ```sh
   git clone https://github.com/paulohebert/atv_adc_embarcatech
   cd atv_adc_embarcatech
   ```
---

## ⚙️ Compilação e Upload
1. Compile o código utilizando o CMake:
   ```sh
   mkdir build
   cd build
   cmake ..
   ninja
   ```
2. Transfira o arquivo `.uf2` gerado para o BitDogLab.
