/*
*   PROJETO COM RTOS
*   O intuito do projeto é Criar uma aplicação embarcada no FreeRTOS com 3 tarefas que simulam o 
*   monitoramento de um sistema com sensores (como um botão e um LED). As tarefas irão cooperar 
*   para realizar diferentes funções, como ler o estado do botão e controlar o LED.
*/

// Inclusões de bibliotecas
#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

// Definições dos pinos do botão A e o led vermelho da placa BitDogLab
#define PIN_BTN_A 5
#define PIN_LED_GREN 11

// Declaração da fila para comunicação entre as tarefas 1 e 2
QueueHandle_t queueButton;

// Função que inicializa o botão e o led
void setup() {

    // inicializando o botão A
    gpio_init(PIN_BTN_A);
    gpio_set_dir(PIN_BTN_A, GPIO_IN);
    gpio_pull_up(PIN_BTN_A);            // mantém o botão no estado HIGH (ALTO)

    // inicializando o led verde
    gpio_init(PIN_LED_GREN);
    gpio_set_dir(PIN_LED_GREN, GPIO_OUT);
}

// Tarefa 1: reponsável por fazer a leitura do botão
void task1ReadButton(void *vParamters) {

    // variável para armazenar o estado do botão
    int state_button;

    for(;;) {
        // obtém o estado atual do botão
        state_button = gpio_get(PIN_BTN_A);

        // verifica se foi pressionado
        if (state_button == 0) {

            // aguarda um pequeno delay e verifica o estado novamente para tratar o bounce
            vTaskDelay(50);
            if (gpio_get(PIN_BTN_A) == 0) {
                // envia o estado do botão para a fila para poder ser processado na tarefa 2
                xQueueSend(queueButton, &state_button, portMAX_DELAY);
            }
        }

        // aplicando o atraso de 100ms entre cada leitura
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

// Tarefa 3: reponsável por acionar o led
void task3TurningOnLed() {
    gpio_put(PIN_LED_GREN, 1);      // liga o led
    vTaskDelay(200);                // mantém ligado por 200ms
    gpio_put(PIN_LED_GREN, 0);      // desliga o led
}

// Tarefa 2: reponsável por receber o sinal da tarefa 1 e acionar a tarefa 3 caso o botão tenha sido pressionado
void task2ProcessButton(void *vParamters) {

    // variável para armazenar o dado da fila
    int data_queue;

    for(;;) {
        
        // verificando se a fila recebeu algum dado
        if (xQueueReceive(queueButton, &data_queue, portMAX_DELAY) == pdTRUE) {

            // acionando tarefa 3 que controla o led
            task3TurningOnLed();
        }
    }
}   

int main()
{

    // inicializando comunicação serial
    stdio_init_all();

    // inicializando os dispositovos (botão e led)
    setup();

    // criando a fila que para armazenar o estado do botão
    queueButton = xQueueCreate(5, sizeof(int));

    // verificando se a fila foi criada com sucesso
    if (queueButton != NULL) {

        // criando as tarefas
        xTaskCreate(task1ReadButton, "Task leitura botao", 128, NULL, 1, NULL);         // cria tarefa que vai ler o botão
        xTaskCreate(task2ProcessButton, "Task processar leituras", 128, NULL, 1, NULL); // cria tarefa que vai processar as leituras
        vTaskStartScheduler();                                                          // inicia o agendador do FreeRTOS
    }

    // caso o agendador não inicie com sucesso, o sistema cairá no loop abaixo
    while (true) {
        printf("Erro ao criar tarefas e iniciar o escalonador\n");
        vTaskDelay(500);
    }
}
