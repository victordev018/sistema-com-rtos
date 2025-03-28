# Projeto para controle de Led a partir de um botão com FreeFTOS utilizando o Raspberry pi pico W

O intúito deste projeto é desenvolver uma aplicação embarcada no FreeRTOS com 3 tarefas que faz o monitoramento de um sistema com sensores (botão e um LED). A ideia é fazer com que tarefas cooperem para realizar diferentes funções, como ler o estado do botão e controlar o LED.

## Componentes necessários
- __Raspberry pi pico__
- __Push Button__ (conectado ao gpio 5)
- __LED__ (conectado ao gpio 11)
- __Resistor Pull-Up__ para manter o estado do botão alto (quando não estiver pressionado)

> Uma outra alternativa seria uma placa BitDogLab, ela já possui todos esses dispositivos integrados

## Estrutura do código

`Setup()`
Função reponspavel por fazer a configuração e inicialização do botão e led
- __Botão__: Pino GPIO 5, configurado como dispositvo de entrada e com resistor de pull-up ativo.
-__Led__: Pino GPIO 12, configurado como um dispositivo de saída.

`task1ReadButton()`
- Função responsável por fazer a leitura dos botões
- Realiza a leitura do estado do botão a cada 100ms
- Envia o estado do botão para uma fila (`queueButton`)

`task2ProcessButton()`
- Função responsável por processar o dado enviado pela tarefa 1
- Verifica se a fila recebeu algum dado
- Caso tenha recebido aciona a função `task3TurningOnLed()` para ativar o led

`task3TurningOnLed()`
- Função responsável por fazer a ativação do led
- Mantém o led ligado por 200ms e em seguida o desliga

## Como Executar

1. Clone o repositório utilizando o seguinte comando:
```bash
git clone https://github.com/victordev018/sistema-com-rtos
```
2. Certifique-se de ter o SDK do Raspberry Pi Pico configurado corretamente.
3. Compile o código utilizando o sistema de build do Raspberry Pi Pico (CMake).
4. Carregue o código no Raspberry Pi Pico utilizando a ferramenta de gravação de firmware.

### Dependências necessárias
> FreeRTOS: Utilizado para gerenciamento das tarefas.<br>
> Pico SDK: Necessário para compilar e rodar o código no Raspberry Pi Pico.

## Vídeo visualizando código e execução na placa

__Link do video__: [clique aqui](https://www.youtube.com/watch?v=anWB50QbQZo)

## Autor
João Victor