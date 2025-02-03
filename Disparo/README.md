
# Atividade 2 – Temporizador de Um Disparo (One Shot)

## Introdução

Este projeto tem como objetivo implementar um sistema de temporização "one shot" utilizando a função `add_alarm_in_ms()` do Pico SDK. A solução foi desenvolvida para que, quando o usuário clicar em um botão (pushbutton), os três LEDs (LED RGB) sejam acionados simultaneamente, iniciando uma sequência de mudança de estados. Após 3 segundos, uma rotina de callback do temporizador altera o estado dos LEDs de forma progressiva (primeiro dois LEDs permanecem acesos e, depois, apenas um), até que o ciclo seja completado. Durante a execução do temporizador, o botão não pode reiniciar a sequência, garantindo que o disparo ocorra somente quando o sistema estiver livre.

## Descrição do Projeto

A aplicação utiliza:
- Um temporizador de disparo único configurado com `add_alarm_in_ms()`, que é iniciado quando o botão é pressionado.
- Uma rotina de callback (`turn_off_callback()`) que realiza a mudança de estado dos LEDs a cada 3.000 ms.
- Uma função de interrupção (`gpio_irq_handler()`) com debounce implementado para garantir que o botão só altere o estado dos LEDs quando o sistema não estiver executando uma sequência ativa.

### Lógica de Funcionamento

1. **Inicialização:**  
   - A comunicação serial é iniciada para facilitar a depuração (uso de `printf()`).
   - Os pinos dos LEDs (LED_RED, LED_GREEN e LED_BLUE) são configurados como saídas.
   - O pino do botão (BUTTON_PIN) é configurado como entrada com resistor pull-up para garantir um estado estável (nível alto) quando não pressionado.

2. **Interrupção do Botão:**  
   - Quando o botão (GPIO 5) é pressionado (detecção de borda de descida), a função `gpio_irq_handler()` é acionada.
   - Um mecanismo de debounce (com 200 ms de atraso) impede múltiplas ativações devido a ruídos.
   - Se o LED não estiver ativo (`led_active == false`), a rotina ativa os três LEDs (todos em nível alto) e marca o sistema como ativo (`led_active = true`).

3. **Temporizador One Shot:**  
   - A função `add_alarm_in_ms()` agenda a execução da função `turn_off_callback()` após 3.000 ms.
   - Na função de callback, um `switch` baseado na variável `i` é utilizado para realizar a sequência:
     - **Caso 0:** Desliga o LED verde, e retorna um atraso de 3.000 ms para manter a contagem.
     - **Caso 1:** Desliga o LED vermelho, e retorna um atraso de 3.000 ms.
     - **Caso 2:** Desliga o LED azul, marca o sistema como inativo (`led_active = false`) e reinicia a contagem (i = 0), retornando 0 para não repetir o alarme.
     
4. **Loop Principal:**  
   - O loop principal mantém o programa em execução e inclui uma pausa de 1.000 ms (1 segundo) a cada iteração, imprimindo uma mensagem na saída serial para indicar que o programa continua rodando.

## Componentes Utilizados

- **Microcontrolador:** Raspberry Pi Pico W
- **LEDs:** 3 LEDs (representando as cores do LED RGB)
- **Resistores:** 3 Resistores de 330 Ω (um para cada LED)
- **Botão:** 1 Pushbutton (utilizado para disparar a sequência)
- **Ferramentas de Desenvolvimento:**
  - Ambiente de desenvolvimento VS Code com integração ao simulador Wokwi.
  - Ferramenta educacional BitDogLab para experimentação física e virtual.

## Bibliotecas Utilizadas

- `stdio.h`: Biblioteca padrão para entrada e saída, utilizada para depuração com `printf()`.
- `pico/stdlib.h`: Biblioteca do Pico SDK que fornece funções essenciais para configuração de GPIO, temporização e comunicação serial.
- `pico/time.h`: Biblioteca que permite o gerenciamento de temporizadores e alarmes no microcontrolador.
  
## Visão Geral do Código

1. **Inclusão de Bibliotecas e Definições:**
   - As bibliotecas necessárias são incluídas no início.
   - Os pinos dos LEDs e do botão são definidos via macros (`LED_RED`, `LED_GREEN`, `LED_BLUE`, `BUTTON_PIN`).

2. **Função `turn_off_callback()`:**
   - Implementa a lógica de desligamento dos LEDs em etapas:
     - No primeiro disparo, desliga o LED verde e retorna um atraso de 3.000 ms.
     - No segundo disparo, desliga o LED vermelho e retorna outro atraso de 3.000 ms.
     - No terceiro disparo, desliga o LED azul, define o sistema como inativo (`led_active = false`) e reinicia o contador.
   - Retorna 0 para finalizar a sequência de temporização.

3. **Função `debounce()`:**
   - Implementa uma verificação simples de debounce para evitar múltiplas ativações do botão devido ao bouncing.
   - Utiliza o tempo atual em microssegundos e um atraso configurado (200 ms) para confirmar se o botão está realmente pressionado.

4. **Função `gpio_irq_handler()`:**
   - É chamada quando ocorre uma interrupção no pino do botão (configurado para borda de descida).
   - Se o botão for pressionado e o sistema não estiver ativo (`led_active == false`), a rotina:
     - Executa o debounce.
     - Aciona os três LEDs (todos em nível alto).
     - Define `led_active` como verdadeiro.
     - Agenda o temporizador one shot chamando `add_alarm_in_ms(3000, turn_off_callback, NULL, false)`.

5. **Função `main()`:**
   - Inicializa a comunicação serial.
   - Configura os pinos dos LEDs como saída e o pino do botão como entrada (com pull-up interno).
   - Configura a interrupção do botão com a função `gpio_set_irq_enabled_with_callback()`.
   - Entra em um loop infinito com um `sleep_ms(1000)` a cada iteração, garantindo que o programa continue executando.

## Como Executar o Projeto

1. **Configuração do Ambiente:**
   - Utilize o VS Code com a extensão “Raspberry Pi Pico” instalada.
   - Configure o Pico SDK conforme as instruções da BitDogLab.
   - Certifique-se de que o simulador Wokwi esteja integrado ao ambiente de desenvolvimento.

2. **Compilação e Upload:**
   - Compile o projeto utilizando o sistema de build (CMake) do Pico SDK.
   - Coloque o Raspberry Pi Pico W em modo BOOTSEL e faça o upload do arquivo UF2 gerado.

3. **Testes e Simulação:**
   - Use o simulador Wokwi e/ou a Ferramenta Educacional BitDogLab para simular o circuito com o LED RGB (GPIO 11, 12 e 13) e o Botão A (GPIO 5).
   - Verifique a operação do sistema: ao pressionar o botão, os três LEDs devem ser acionados e, após 3 segundos, a sequência de desligamento deve ocorrer conforme a lógica implementada.
   - Observe as mensagens impressas na saída serial para confirmar a execução correta do temporizador e do debounce.

## Conclusão

Este projeto implementa um sistema de temporização one shot utilizando o Pico SDK para o Raspberry Pi Pico W. Ao pressionar o botão, os três LEDs são acionados e, com um atraso de 3 segundos, uma rotina de callback altera o estado dos LEDs progressivamente. A implementação inclui uma rotina de debounce para minimizar o efeito do bouncing do botão, garantindo a robustez da solução.  
A integração com a Ferramenta Educacional BitDogLab e o simulador Wokwi facilita o desenvolvimento e a experimentação, permitindo testar a aplicação tanto em ambiente virtual quanto em hardware físico.
