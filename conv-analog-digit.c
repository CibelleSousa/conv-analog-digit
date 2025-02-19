#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"

// Defines para o Display
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
#define WIDTH 128
#define HIGH 64
#define SQUARE_SIZE 8

// Defines para o Joystick
#define VRX_PIN 26    
#define VRY_PIN 27

// Defines para LEDs, Botões e PWM
#define LED_V 13
#define LED_B 12
#define LED_G 11
#define WRAP 4095  
#define BTN_A 5
#define BTN_JOY 22

// Variáveis Globais
static volatile uint32_t last_time = 0;
static volatile uint slice_v;
static volatile uint slice_b;
static volatile bool flag = false;
static uint8_t border_style = 0;
ssd1306_t ssd;

// Prototipação das funções
void setup();
void setupDisplay();
void updateDisplay(uint16_t square_y, uint16_t square_x);
void setDutyCycle(uint16_t vry_value, uint16_t vrx_value);
void gpio_irq_handler(uint gpio, uint32_t events);
void draw_border(uint8_t style);
uint pwm_init_gpio(uint gpio, uint wrap);


int main() {
    // Variáveis para definir os valores lidos no Joystick
    static volatile uint16_t vry_value;
    static volatile uint16_t vrx_value;
    
    // Variável para armazenar o tempo da última impressão na serial
    uint32_t last_print_time = 0;

    // Variáveis para determinar a posição do quadrado 8x8 no display
    uint16_t square_y, square_x;

    // Inicializações
    stdio_init_all();
    setup();
    setupDisplay();

    // Desenho da borda
    draw_border(border_style);

    while (true) {
        // Leituras dos valores do ADC para VRX (Eixo X do joystick) e VRY (Eixo Y do joystick)
        adc_select_input(0); 
        vry_value = adc_read();
        adc_select_input(1); 
        vrx_value = adc_read();

        // Rotina responsável por ajustar o duty cycle do LED conforme VRX e VRY
        setDutyCycle(vry_value, vrx_value);

        // Cálculo da posição do quadrado 8x8 no Display
        square_x = (vrx_value * (WIDTH - SQUARE_SIZE)) / WRAP;
        square_y = ((WRAP - vry_value) * (HIGH - SQUARE_SIZE)) / WRAP;

        // Rotina responsável pelo o que será exibido no Display
        updateDisplay(square_y, square_x);

        sleep_ms(100);  // Pausa o programa por 100ms para evitar leituras e impressões muito rápidas
    }

    return 0;
}

void setup(){
    // Configura LED GREEN
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);

    // Inicializa o PWM para os LEDs no pino GP12 e GP13
    slice_b = pwm_init_gpio(LED_B, WRAP);
    slice_v = pwm_init_gpio(LED_V, WRAP);

    // Configura o ADC
    adc_init();
    adc_gpio_init(VRX_PIN);
    adc_gpio_init(VRY_PIN);

    // Configura os botões
    gpio_init(BTN_A);
    gpio_set_dir(BTN_A, GPIO_IN);
    gpio_pull_up(BTN_A);

    gpio_init(BTN_JOY);
    gpio_set_dir(BTN_JOY, GPIO_IN);
    gpio_pull_up(BTN_JOY);

    // Habilita interrução nos botões
    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BTN_JOY, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
}

void setupDisplay(){
    // I2C Inicialização. Setando frequência a 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Seta a GPIO para I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Seta a GPIO para I2C
    gpio_pull_up(I2C_SDA); // Pull up a data line
    gpio_pull_up(I2C_SCL); // Pull up a clock line
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

void setDutyCycle(uint16_t vry_value, uint16_t vrx_value){
    // Se o PWM estiver habilitado, ajustamos o duty cycle 
    if(flag){
        uint16_t duty_cycle_b = (abs(vry_value - 2048) * 2);
        uint16_t duty_cycle_v = (abs(vrx_value - 2048) * 2);
        pwm_set_gpio_level(LED_B, (abs(vry_value - 2048) > 550) ? duty_cycle_b : 0);
        pwm_set_gpio_level(LED_V, (abs(vrx_value - 2048) > 550) ? duty_cycle_v : 0);
    }
}

void gpio_irq_handler(uint gpio, uint32_t events){
    // Debouncing para os botões
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    if (current_time - last_time > 200000){
        last_time = current_time;
        // Se for o botão A habilita o PWM
        if(gpio == BTN_A){
            flag = !flag;
            pwm_set_enabled(slice_v, flag);
            pwm_set_enabled(slice_b, flag);
        }
        // Se for o botão Joystick muda o estado do LED GREEN e muda a borda que deve ser desenhada no Display
        else{
            gpio_put(LED_G, !gpio_get(LED_G));
            border_style = (border_style + 1) % 3;
        }
    }
}

void draw_border(uint8_t style){
    // Alterna entre 3 diferentes estilos de borda
    switch (style) {
        case 0:
            ssd1306_rect(&ssd, 0, 0, WIDTH, HIGH, 1, false);
            break;
        case 1:
            for (int x = 0; x < WIDTH; x += 4) {
                ssd1306_pixel(&ssd, x, 0, 1);
                ssd1306_pixel(&ssd, x, HIGH - 1, 1);}
            for (int y = 0; y < HIGH; y += 4) {
                ssd1306_pixel(&ssd, 0, y, 1);
                ssd1306_pixel(&ssd, WIDTH - 1, y, 1);}
            break;
        case 2:
            ssd1306_rect(&ssd, 1, 1, WIDTH-2, HIGH-2, 1, false);
            ssd1306_rect(&ssd, 3, 3, WIDTH-6, HIGH-6, 1, false);
            break;
    }
}

void updateDisplay(uint16_t square_y, uint16_t square_x){
    // Limpa, desenha a borda, desenha o quadrado 8x8, manda para o display
    ssd1306_fill(&ssd, 0);
    draw_border(border_style);
    ssd1306_rect(&ssd, square_y, square_x, SQUARE_SIZE, SQUARE_SIZE, 1, true);
    ssd1306_send_data(&ssd);
}

uint pwm_init_gpio(uint gpio, uint wrap) {
    // Define o pino GPIO como saída de PWM
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    
    // Obtém o número do "slice" de PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    
    // Define o "wrap" do PWM, que define o valor máximo para o contador
    pwm_set_wrap(slice_num, wrap);
    
    // Deixa o PWM desabilitado no slice pois quem deve habilitar é o botão A
    pwm_set_enabled(slice_num, flag);  
    return slice_num;
}