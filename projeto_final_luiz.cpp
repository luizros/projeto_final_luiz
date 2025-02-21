// #include <stdio.h>
// #include "pico/stdlib.h"
// #include "pico-ssd1306/ssd1306.h"
// #include "pico-ssd1306/textRenderer/TextRenderer.h"
// #include "hardware/i2c.h"
// #include "ws2818b.pio.h"
// #include "include/my_constanst.h"
// #include "include/matriz_rgb.h"
// #include "include/joystick.h"
// #include "pico/stdlib.h"
// #include "hardware/adc.h"
// #include <stdlib.h>
// #include <time.h>

// using namespace pico_ssd1306;

// void exibirErro() {
//     int spriteErro[5][5][3] = {
//         {{245, 4, 4}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}},
//         {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
//         {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
//         {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
//         {{255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}}
//     };
    
//     for(int i = 0; i < 5; i++) {
//         for(int j = 0; j < 5; j++) {
//             npSetLED(getIndex(i, j), spriteErro[i][j][0], spriteErro[i][j][1], spriteErro[i][j][2]);
//         }
//     }
//     npWrite();
//     sleep_ms(2000);
//     npClear();
// }

// void atualizarDisplay(SSD1306 &display, int cursorX, int cursorY, int etapa, bool erro) {
//     display.clear();

//     if (erro) {
//         drawText(&display, font_12x16, "ERRO!", 40, 20);
//     } else {
//         char texto[20];
//         sprintf(texto, "X:%d Y:%d [%d]", cursorX, cursorY, etapa);
//         drawText(&display, font_12x16, texto, 10, 20);
//     }

//     display.sendBuffer();
// }

// void jogarMemoria(Joystick &joystick, SSD1306 &display) {
//     srand(time(NULL));
//     int cursorX = 2, cursorY = 2;
//     int x, y;
//     const int ZONA_MORTA = 10;
//     int maxSequencia = 1;

//     while (true) {
//         int sequencia[maxSequencia][2];

//         for (int i = 0; i < maxSequencia; i++) {
//             sequencia[i][0] = rand() % 5;
//             sequencia[i][1] = rand() % 5;
//         }

//         for (int i = 0; i < maxSequencia; i++) {
//             npClear();
//             npSetLED(getIndex(sequencia[i][1], sequencia[i][0]), 0, 5, 0);
//             npWrite();
//             sleep_ms(1000);
//             npClear();
//             npWrite();
//             sleep_ms(500);
//         }

//         bool acertouTudo = true;

//         for (int i = 0; i < maxSequencia; i++) {
//             while (true) {
//                 joystick.read(x, y);

//                 if (x > -ZONA_MORTA && x < ZONA_MORTA) x = 0;
//                 if (y > -ZONA_MORTA && y < ZONA_MORTA) y = 0;

//                 if (y > 50 && cursorY < 4) cursorY++;
//                 if (y < -50 && cursorY > 0) cursorY--;
//                 if (x > 50 && cursorX > 0) cursorX--;
//                 if (x < -50 && cursorX < 4) cursorX++;

//                 cursorX = cursorX < 0 ? 0 : (cursorX > 4 ? 4 : cursorX);
//                 cursorY = cursorY < 0 ? 0 : (cursorY > 4 ? 4 : cursorY);

//                 npClear();
//                 npSetLED(getIndex(cursorY, cursorX), 0, 0, 5);
//                 npWrite();

//                 atualizarDisplay(display, cursorX, cursorY, i + 1, false);
//                 sleep_ms(200);

//                 if (gpio_get(BUTTON_A) == 0) {
//                     sleep_ms(100);
//                     if (gpio_get(BUTTON_A) == 0) {
//                         if (cursorX == sequencia[i][0] && cursorY == sequencia[i][1]) {
//                             atualizarDisplay(display, cursorX, cursorY, i + 1, false);
//                             sleep_ms(300);
//                             while (joystick.isPressed());
//                             break;
//                         } else {
//                             exibirErro();
//                             atualizarDisplay(display, cursorX, cursorY, i + 1, true);
//                             sleep_ms(1000);
//                             maxSequencia = 1;
//                             cursorX = 2;
//                             cursorY = 2;
//                             acertouTudo = false;
//                             while (joystick.isPressed());
//                             break;
//                         }
//                     }
//                 }
//             }
//             if (!acertouTudo) break;
//         }

//         if (acertouTudo && maxSequencia < 5) {
//             maxSequencia++;
//         }
//     }
// }


// int main() {
//     stdio_init_all();
//     // Init i2c0 controller
//     i2c_init(i2c1, 1000000);
//     // Set up pins 12 and 13
//     gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
//     gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
//     gpio_pull_up(SDA_PIN);
//     gpio_pull_up(SCL_PIN);

//     gpio_init(BUTTON_A);
//     gpio_set_dir(BUTTON_A, GPIO_IN);
//     gpio_pull_up(BUTTON_A);


//     // If you don't do anything before initializing a display pi pico is too fast and starts sending
//     // commands before the screen controller had time to set itself up, so we add an artificial delay for
//     // ssd1306 to set itself up
//     sleep_ms(250);
//     SSD1306 display = SSD1306(i2c1, 0x3C, Size::W128xH64);
//     // Inicializa o joystick
//     Joystick joystick(vRx_PIN, vRy_PIN, SW, 0, 1);
//     joystick.begin();
//     display.setOrientation(0);

//     // Inicializa a matriz de LEDs
//     npInit(LED_PIN);
//     npClear();
//     npWrite();

//     // Inicializa o gerador de números aleatórios
//     srand(time(NULL));

//     // Inicia o jogo de memória
//     jogarMemoria(joystick, display);

//     return 0;
// }
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico-ssd1306/ssd1306.h"
#include "pico-ssd1306/textRenderer/TextRenderer.h"
#include "hardware/i2c.h"
#include "ws2818b.pio.h"
#include "include/my_constanst.h"
#include "include/matriz_rgb.h"
#include "include/joystick.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <stdlib.h>
#include <time.h>

using namespace pico_ssd1306;

void exibirErro() {
    int spriteErro[5][5][3] = {
        {{245, 4, 4}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}},
        {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
        {{255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}}
    };
    
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            npSetLED(getIndex(i, j), spriteErro[i][j][0], spriteErro[i][j][1], spriteErro[i][j][2]);
        }
    }
    npWrite();
    sleep_ms(2000);
    npClear();
}

void atualizarDisplay(SSD1306 &display, int cursorX, int cursorY, int etapa, int itensRestantes, bool erro) {
    display.clear();
    if (erro) {
        drawText(&display, font_8x8, "ERRO!", 40, 20);
    } else {
        char texto[30];
        sprintf(texto, "X:%d Y:%d", cursorX, cursorY);
        drawText(&display, font_8x8, texto, 10, 10);
        sprintf(texto, "PONTOS: %d", etapa);
        drawText(&display, font_8x8, texto, 10, 20);
        sprintf(texto, "ITENS: %d", itensRestantes);
        drawText(&display, font_8x8, texto, 10, 30);
    }
    display.sendBuffer();
}

void jogarMemoria(Joystick &joystick, SSD1306 &display) {
    srand(time(NULL));
    int cursorX = 2, cursorY = 2;
    int x, y;
    const int ZONA_MORTA = 10;
    int maxSequencia = 1;

    while (true) {
        int sequencia[maxSequencia][2];

        for (int i = 0; i < maxSequencia; i++) {
            sequencia[i][0] = rand() % 5;
            sequencia[i][1] = rand() % 5;
        }

        for (int i = 0; i < maxSequencia; i++) {
            npClear();
            npSetLED(getIndex(sequencia[i][1], sequencia[i][0]), 0, 5, 0);
            npWrite();
            sleep_ms(1000);
            npClear();
            npWrite();
            sleep_ms(500);
        }

        int itensRestantes = maxSequencia;
        bool acertouTudo = true;

        for (int i = 0; i < maxSequencia; i++) {
            while (true) {
                joystick.read(x, y);
                if (x > -ZONA_MORTA && x < ZONA_MORTA) x = 0;
                if (y > -ZONA_MORTA && y < ZONA_MORTA) y = 0;
                if (y > 50 && cursorY < 4) cursorY++;
                if (y < -50 && cursorY > 0) cursorY--;
                if (x > 50 && cursorX > 0) cursorX--;
                if (x < -50 && cursorX < 4) cursorX++;
                cursorX = cursorX < 0 ? 0 : (cursorX > 4 ? 4 : cursorX);
                cursorY = cursorY < 0 ? 0 : (cursorY > 4 ? 4 : cursorY);
                npClear();
                npSetLED(getIndex(cursorY, cursorX), 0, 0, 5);
                npWrite();
                atualizarDisplay(display, cursorX, cursorY, i + 1, itensRestantes, false);
                sleep_ms(200);

                if (gpio_get(BUTTON_A) == 0) {
                    sleep_ms(150);
                    if (gpio_get(BUTTON_A) == 0) {
                        display.clear();
                        char texto[30];
                        sprintf(texto, "POS \nX:%d\nY:%d", cursorX, cursorY);
                        drawText(&display, font_8x8, texto, 10, 10);
                        drawText(&display, font_8x8, "CONFIRMAR? B", 10, 30);
                        drawText(&display, font_8x8, "MARCAR NOVAMENTE A", 10, 40);                        
                        display.sendBuffer();
                        while (gpio_get(BUTTON_A) == 0);
                        sleep_ms(250);
                        while (true) {
                            if (gpio_get(BUTTON_A) == 0) break;
                            sleep_ms(150);
                            if (gpio_get(BUTTON_B) == 0) {
                                sleep_ms(150);
                                itensRestantes--;
                                break;
                            }
                        }
                        if (cursorX == sequencia[i][0] && cursorY == sequencia[i][1]) {
                            itensRestantes--;
                            break;
                        } else {
                            exibirErro();
                            acertouTudo = false;
                            maxSequencia = 1;
                            cursorX = 2;
                            cursorY = 2;
                            break;
                        }
                    }
                }
            }
            if (!acertouTudo) break;
        }

        if (acertouTudo && maxSequencia < 5) {
            maxSequencia++;
        }
    }
}

int main() {
    stdio_init_all();
    i2c_init(i2c1, 1000000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
    sleep_ms(250);
    SSD1306 display = SSD1306(i2c1, 0x3C, Size::W128xH64);
    Joystick joystick(vRx_PIN, vRy_PIN, SW, 0, 1);
    joystick.begin();
    display.setOrientation(0);
    npInit(LED_PIN);
    npClear();
    npWrite();
    srand(time(NULL));
    jogarMemoria(joystick, display);
    return 0;
}
