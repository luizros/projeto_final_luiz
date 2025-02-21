#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

// Biblioteca gerada pelo arquivo .pio durante compilação.
#include "ws2818b.pio.h"


int main() {

  // Inicializa entradas e saídas.
  stdio_init_all();

  // Inicializa matriz de LEDs NeoPixel.
  npInit(LED_PIN);
  npClear();

  // Aqui, você desenha nos LEDs.

  npWrite(); // Escreve os dados nos LEDs.

  // Não faz mais nada. Loop infinito.
  while (true) {

    // Array do Sprite a ser exibido na matriz neopixel
  int matriz[5][5][3] = {
    {{255, 255, 255}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 255, 255}},
    {{255, 0, 0}, {0, 255, 0}, {255, 0, 0}, {0, 255, 0}, {255, 0, 0}},
    {{255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}},
    {{255, 0, 0}, {255, 0, 0}, {255, 255, 255}, {255, 0, 0}, {255, 0, 0}},
    {{255, 0, 0}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 0, 0}}
  };

  for(int i = 0; i < 5; i++) {
    for(int j = 0; j < 5; j++) {
      npSetLED(getIndex(i, j), matriz[j][i][0], matriz[j][i][1], matriz[j][i][2]);
    }
    npWrite();
    sleep_ms(1000);
    npClear();

  }
}
}