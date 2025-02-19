# Conversor Analógico-Digital com Display OLED e Joystick

## Sumário

1. [💡 O que é este projeto](#1--o-que-é-este-projeto)
2. [🎥 Vídeo demonstrativo](#2--vídeo-demonstrativo)
3. [🔎 Funcionalidades disponíveis](#3--funcionalidades-disponíveis)
4. [🧰 Pré-requisitos para executar](#4--pré-requisitos-para-executar)
5. [💻 Como executar a simulação](#5--como-executar-a-simulação)
6. [👥 Colaboradores do projeto](#6--colaboradores-do-projeto)
7. [✨ Recursos para versionamento](#7--recursos-para-versionamento)

## 1. 💡 O que é este projeto

Este é um firmware desenvolvido em C para o Raspberry Pi Pico, que realiza a leitura de um joystick analógico via ADC e exibe sua posição em um display OLED SSD1306. Além disso, o projeto controla LEDs via PWM e responde a eventos de botões físicos, incluindo alteração do brilho dos LEDs e troca do estilo da borda no display.

## 2. 🎥 Vídeo demonstrativo

Uma demonstração do projeto em funcionamento pode ser assistida no link abaixo:

[inserir link]

## 3. 🔎 Funcionalidades disponíveis

Por meio dos botões, o usuário pode realizar as seguintes ações:

- Pressionar `Botão A` habilita/desabilita o PWM nos LEDs azul e vermelho;
- Pressionar `Botão Joystick` altera o estado do LED verde e permite alternar entre diferentes estilos de borda no display;
- Altera o brilho dos LEDs com base na posição do joystick (via PWM);


## 4. 🧰 Pré-requisitos para executar

A configuração sugerida para executar o projeto é:

1. Ter o [Pico SDK](https://github.com/raspberrypi/pico-sdk) instalado na sua máquina;
2. Ter o [ARM GNU Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain) instalado na sua máquina;
3. Ter o [Visual Studio Code](https://code.visualstudio.com/download) instalado na sua máquina;
4. Ter este repositório clonado na sua máquina;
5. Ter as seguintes extensões instaladas no seu VS Code:
- [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools);
- [CMake](https://marketplace.visualstudio.com/items?itemName=twxs.cmake);
- [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools);
- [Raspberry Pi Pico](https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico);
- [Wokwi Simulator](https://marketplace.visualstudio.com/items?itemName=Wokwi.wokwi-vscode).

## 5. 💻 Como executar a simulação

Com os pré-requisitos atendidos, siga os passos a seguir:

1. Utilize o VS Code para abrir a pasta raiz do projeto clonado:

![Abrir pasta...](assets/open_folder.png)

2. Clique em "Compile", à direita da barra de status e aguarde o processo:

![Compile](assets/compile_button.png)

3. Conecte sua placa BitDogLab via USB apertando o botão bootsel:

![Placa_BitDogLab](assets/placa.png)

4. Clique em "Run", à direita da barra de status e aguarde o processo:
![Run](assets/run_button.PNG)

## 6. 👥 Colaboradores do projeto

Autora do projeto:

### Desenvolvedora

#### Cibelle Sousa Rodrigues

## 7. ✨ Recursos para versionamento

### Conventional Commits

Conventional Commits seguem uma estrutura fixa como feat, fix, docs e style, o que facilita identificar o escopo das mudanças. Isso ajuda a organizar o histórico e facilita o versionamento (por exemplo, ao definir o número da próxima versão do software).

### Branches nomeadas segundo o escopo

A nomeação de branches como `feature/...`, `docs/...` ou `bugfix/...` organiza o fluxo de trabalho e indica o propósito da branch, facilitando o entendimento. Portanto, foi um recurso adotado durante o desenvolvimento do projeto.

### Diretivas de pré-processamento

Neste projeto, utilizamos diretivas de pré-processamento para definir os pinos e periféricos utilizados. O display OLED está configurado para comunicação via I2C, utilizando os pinos GPIO 14 e GPIO 15 para SDA e SCL, respectivamente. O joystick analógico tem seus eixos X e Y conectados aos pinos GPIO 26 e GPIO 27, que são utilizados como entradas analógicas para leitura do ADC.

Os LEDs estão associados aos GPIOs 11, 12 e 13, sendo controlados via PWM para ajuste de brilho. O projeto também conta com botões físicos, como o botão A, conectado ao GPIO 5, e o botão do próprio joystick, que está no GPIO 22. Para garantir um controle eficiente do PWM, utilizamos um valor de "wrap" de 4095, possibilitando ajustes suaves na intensidade dos LEDs.
