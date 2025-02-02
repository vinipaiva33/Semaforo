#include <stdio.h> // Biblioteca padrão do C.
#include "pico/stdlib.h" // Biblioteca padrão do Raspberry Pi Pico para controle de GPIO, temporização e comunicação serial.
#include "pico/time.h"   // Biblioteca para gerenciamento de temporizadores e alarmes.

#define LED_RED 13
#define LED_GREEN 11
#define LED_BLUE 12   // Define o pino GPIO 11 para controlar o LED.
const uint BUTTON_PIN = 5;  // Define o pino GPIO 5 para ler o estado do botão.
int a = 0;
static volatile uint i = 0;
absolute_time_t turn_off_time;  // Variável para armazenar o tempo em que o LED deve ser desligado (não utilizada neste código).
bool led_active = false;

// Função de callback para desligar o LED após o tempo programado.
int64_t turn_off_callback(alarm_id_t id, void *user_data) {

    // Desliga o LED configurando o pino LED_PIN para nível baixo.
    switch (i)
    {
        case 0:
            gpio_put(LED_GREEN,false);
            i++;
            return 3000*1000;
            break;
        case 1:
            gpio_put(LED_RED,false);
            i++;
            return 3000*1000;
            break;
        case 2:
            gpio_put(LED_BLUE,false);
            led_active = false;
            i=0;
            return 0;
            break;
    }
    // Retorna 0 para indicar que o alarme não deve se repetir.
}


bool debounce(volatile uint32_t *last_time, uint32_t debounce_time) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    if (current_time - *last_time > debounce_time) {
        *last_time = current_time;
        return true;
    }
    return false;
}


void gpio_irq_handler(uint gpio, uint32_t events)
{
    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    static volatile uint32_t last_timeA = 0; // Armazena o tempo do último evento (em microssegundos)
    static volatile uint32_t last_timeB = 0; // Armazena o tempo do último evento (em microssegundos)

    // Verifica se passou tempo suficiente desde o último evento
    if (gpio == BUTTON_PIN && debounce(&last_timeA, 200000) && led_active == false)
    {
         // Debouncing de 200ms
            last_timeA = current_time;
            printf("A: %d\n", a); // Para controle quando se usa o monitor serial para verificar se há bouncing
            a++;
            gpio_put(LED_RED,true);
            gpio_put(LED_BLUE,true);
            gpio_put(LED_GREEN,true);
            led_active = true;
            add_alarm_in_ms(3000, turn_off_callback, NULL, false);
    }
}


int main() {
    // Inicializa a comunicação serial para permitir o uso de printf.
    // Isso é útil para depuração, embora não seja usado neste exemplo.
    stdio_init_all();

    // Configura o pino LED_PIN (11) como saída digital.
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);

    // Configura o pino BUTTON_PIN (5) como entrada digital.
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);

    // Habilita o resistor pull-up interno para o pino do botão.
    // Isso garante que o pino seja lido como alto (3,3 V) quando o botão não está pressionado.
    gpio_pull_up(BUTTON_PIN);
    // Loop principal do programa que verifica continuamente o estado do botão.
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

while (true)
{
    sleep_ms(1000);
}

    // Retorno de 0, que nunca será alcançado devido ao loop infinito.
    // Isso é apenas uma boa prática em programas com um ponto de entrada main().
    return 0;
}