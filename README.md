
# Atividade 1 – Temporizador Periódico

## Introdução

Este projeto tem como objetivo implementar um semáforo eletrônico utilizando o microcontrolador Raspberry Pi Pico W. A aplicação simula um semáforo, alternando os sinais dos LEDs (vermelho, amarelo e verde) com um atraso de 3 segundos entre cada mudança de estado. Além disso, o programa imprime uma mensagem a cada segundo na saída serial, demonstrando o funcionamento do temporizador periódico.

## Descrição do Projeto

O projeto utiliza a função `add_repeating_timer_ms()` do Pico SDK para configurar um temporizador que dispara um callback a cada 3000 milissegundos (3 segundos). A função de callback, `repeating_timer_callback()`, implementa a lógica do semáforo, alternando os LEDs da seguinte forma:

- **Estado 0:** Acende o LED vermelho e desliga o LED verde.
- **Estado 1:** Acende o LED verde.
- **Estado 2:** Desliga o LED vermelho e reinicia o ciclo.

Além disso, no loop principal do programa é impresso um “rotina de repetição” a cada 1 segundo, para evidenciar a execução contínua do código.

O projeto foi desenvolvido utilizando a Ferramenta Educacional BitDogLab, onde o LED RGB é conectado aos pinos GPIO 11 (LED verde), 12 (LED azul) e 13 (LED vermelho).

## Componentes Utilizados

- **Microcontrolador:** Raspberry Pi Pico W
- **LEDs:** 03 LEDs (vermelho, amarelo e verde)  
  *(Observação: No projeto, os LEDs utilizados são aqueles conectados aos pinos definidos: LED_RED (GPIO 13), LED_GREEN (GPIO 11) e LED_BLUE (GPIO 12). Note que, na lógica do callback, somente os LEDs vermelho e verde são alternados conforme o semáforo. O LED azul é inicializado, mas não é manipulado pela rotina.)*
- **Resistores:** 03 Resistores de 330 Ω (um para cada LED)
- **Ferramenta de Simulação e Desenvolvimento:** BitDogLab e o simulador Wokwi integrado ao VS Code

## Bibliotecas Utilizadas

As bibliotecas utilizadas neste projeto são as seguintes:

- **stdio.h:** Biblioteca padrão de entrada e saída em C, utilizada para funções como `printf` que possibilitam a depuração via comunicação serial.
- **pico/stdlib.h:** Biblioteca padrão do Pico SDK que fornece funções essenciais para a manipulação de GPIOs, temporização e inicialização da comunicação serial.
- **hardware/timer.h:** Biblioteca do Pico SDK responsável pelo gerenciamento dos temporizadores de hardware, utilizada para configurar e operar o temporizador periódico com a função `add_repeating_timer_ms()`.


## Visão Geral do Código

O código principal é estruturado da seguinte forma:

1. **Inclusão de Bibliotecas:**  
   São incluídas as bibliotecas padrão (`stdio.h`, `pico/stdlib.h`, `hardware/timer.h`) e as bibliotecas próprias (`inicia.h` e `iniciapino.h`).

2. **Definição de Pinos e Variáveis:**  
   São definidos os pinos dos LEDs (LED_RED, LED_GREEN, LED_BLUE) e declarada uma variável global `i` (usada para controlar o estado do semáforo).  
   Também são definidas variáveis para a intensidade de cada cor do LED RGB, embora neste exercício a lógica do semáforo manipule apenas os estados de ligar e desligar (no caso, os LEDs vermelho e verde).

3. **Função de Callback (`repeating_timer_callback`):**  
   Esta função é chamada a cada 3 segundos pelo temporizador configurado. Dentro dela, um `switch` baseado na variável `i` alterna o estado dos LEDs:
   - **Caso 0:** Liga o LED vermelho, desliga o LED verde e incrementa `i`.
   - **Caso 1:** Liga o LED verde e incrementa `i`.
   - **Caso 2:** Desliga o LED vermelho e reinicia `i` para 0.
   A função imprime uma mensagem na saída serial ("3 segundo passou") para indicar a execução do callback e retorna `true` para manter o temporizador ativo.

4. **Função Principal (`main`):**  
   - Inicializa a comunicação serial com `stdio_init_all()`.
   - Inicializa os pinos dos LEDs e os configura como saídas.
   - Configura os temporizadores, utilizando `add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer)` para chamar o callback a cada 3 segundos.
   - Entra em um loop infinito onde, a cada 1 segundo, é impressa a mensagem "rotina de repetição" (usando `sleep_ms(1000)`), apenas para demonstrar a atividade do loop principal.

## Como Executar o Projeto

1. **Configuração do Ambiente:**
   - Certifique-se de ter o VS Code instalado com as extensões necessárias para o desenvolvimento com Raspberry Pi Pico (por exemplo, a extensão “Raspberry Pi Pico”).
   - Instale o Pico SDK e configure o ambiente de desenvolvimento conforme as instruções da BitDogLab.

2. **Simulação e Testes:**
   - Utilize o simulador Wokwi integrado ao VS Code para testar o código.
   - Verifique o funcionamento dos LEDs conforme a lógica do semáforo.
   - Use a ferramenta BitDogLab para testar o código na placa de desenvolvimento física (Raspberry Pi Pico W).

3. **Compilação e Upload:**
   - Compile o projeto utilizando o sistema de build do Pico SDK (usando CMake).
   - Coloque o microcontrolador no modo BOOTSEL e faça o upload do arquivo UF2 gerado.

4. **Verificação via Monitor Serial:**
   - Conecte um monitor serial (por exemplo, utilizando o PuTTY) para observar as mensagens de depuração (como "3 segundo passou" e "rotina de repetição").



## Conclusão

Este projeto demonstra a implementação de um semáforo eletrônico utilizando temporizadores do Pico SDK. Com o emprego da função `add_repeating_timer_ms()`, o sistema alterna os sinais dos LEDs (representando as cores do semáforo) a cada 3 segundos, enquanto o loop principal imprime mensagens de status a cada 1 segundo. A solução apresentada integra de forma eficiente o controle de GPIOs e a temporização, utilizando a plataforma BitDogLab e o Raspberry Pi Pico W.

---
