// Inclusão de bibliotecas
#include <stdio.h>
#include "pico/stdlib.h"   
#include "hardware/timer.h" 

//Definição dos pinos e inicialização de iterador
#define LED_RED 13
#define LED_GREEN 11
#define LED_BLUE 12

static volatile uint i = 0;

// Função de callback que será chamada repetidamente pelo temporizador
// O tipo bool indica que a função deve retornar verdadeiro ou falso para continuar ou parar o temporizador.
bool repeating_timer_callback(struct repeating_timer *t) {
    // Imprime uma mensagem na saída serial indicando que 1 segundo se passou.
    printf("3 segundo passou\n");
    //Liga ou desliga o led.

    switch (i)
    {
        case 0:
            gpio_put(LED_RED,true);
            gpio_put(LED_GREEN,false);
            i++;
            break;
        case 1:
            gpio_put(LED_GREEN,true);
            i++;
            break;
        case 2:
            gpio_put(LED_RED,false);
            i=0;
            break;
    }
    // Retorna true para manter o temporizador repetindo. Se retornar false, o temporizador para.
    return true;
}

int main() {
    // Inicializa a comunicação serial, permitindo o uso de funções como printf.
    stdio_init_all();

    // Inicializar os pinos dos leds
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED,true);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN,true);
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE,true);
    

    // Declaração de uma estrutura de temporizador de repetição.
    // Esta estrutura armazenará informações sobre o temporizador configurado.
    struct repeating_timer timer;

    // Configura o temporizador para chamar a função de callback a cada 1 segundo.
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);

    // Loop infinito que mantém o programa em execução.
    while (true) {
        // Pausa de 10 segundos para reduzir o uso da CPU.
        sleep_ms(1000);
        printf("rotina de repetição\n");
    }
    return 0;
}