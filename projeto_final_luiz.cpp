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
    npWrite();
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
    int sequencia[5][2];
    bool acertouTudo = true;
    int itensRestantes;
    
    enum Estado {BEM_VINDO, GERAR_POSICAO, MOVIMENTAR_CURSOR, CONFIRMAR_ESCOLHA, ATUALIZAR_DISPLAY };
    Estado estado = BEM_VINDO;

    while (true) {
        switch (estado) {
            case BEM_VINDO:
                npClear();
                display.clear();
                drawText(&display, font_8x8, "JOGO DA MEMORIA", 0, 20);
                drawText(&display, font_8x8, "APERTE O BOTAO A", 0, 30);
                display.sendBuffer();

                // Verifica se o botão A foi pressionado
                if (gpio_get(BUTTON_A) == 0) { // Corrigido aqui
                    sleep_ms(100); // Debounce
                    if (gpio_get(BUTTON_A) == 0) { // Corrigido aqui
                        estado = GERAR_POSICAO; // Avança para o próximo estado
                    }
                } else {
                    estado = BEM_VINDO; // Permanece no estado atual
                }
                break;
            case GERAR_POSICAO:
                for (int i = 0; i < maxSequencia; i++) {
                    sequencia[i][0] = rand() % 5;
                    sequencia[i][1] = rand() % 5;
                }
                
                for (int i = 0; i < maxSequencia; i++) {
                    npClear();
                    npSetLED(getIndex(sequencia[i][1], sequencia[i][0]), 0, 20, 0);
                    npWrite();
                    sleep_ms(1000);
                    npClear();
                    npWrite();
                    sleep_ms(500);
                }
                
                itensRestantes = maxSequencia;
                acertouTudo = true;
                estado = MOVIMENTAR_CURSOR;
                break;

            case MOVIMENTAR_CURSOR:
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
                npSetLED(getIndex(cursorY, cursorX), 0, 0, 20);
                npWrite();
                atualizarDisplay(display, cursorX, cursorY, maxSequencia - itensRestantes + 1, itensRestantes, false);
                sleep_ms(200);

                if (gpio_get(BUTTON_B) == 0) {
                    sleep_ms(150);
                    if (gpio_get(BUTTON_B) == 0) {
                        estado = CONFIRMAR_ESCOLHA;
                    }
                }
                break;

            case CONFIRMAR_ESCOLHA:
                display.clear();
                char texto[30];
                sprintf(texto, "POS \nX:%d\nY:%d", cursorX, cursorY);
                drawText(&display, font_8x8, texto, 10, 10);
                drawText(&display, font_8x8, "Confirmar: B", 10, 30);
                drawText(&display, font_8x8, "Reescolher: A", 10, 40);                
                display.sendBuffer();
                
                while (true) {
                    if (gpio_get(BUTTON_A) == 0) {
                        sleep_ms(150);
                        if (gpio_get(BUTTON_A) == 0) {
                            estado = MOVIMENTAR_CURSOR;
                            break;
                        }
                    }
                    if (gpio_get(BUTTON_B) == 0) {
                        sleep_ms(150);
                        if (gpio_get(BUTTON_B) == 0) {
                            estado = ATUALIZAR_DISPLAY;
                            break;
                        }
                    }
                }
                break;

            case ATUALIZAR_DISPLAY:
                if (cursorX == sequencia[maxSequencia - itensRestantes][0] && cursorY == sequencia[maxSequencia - itensRestantes][1]) {
                    itensRestantes--;
                    if (itensRestantes == 0) {
                        if (acertouTudo && maxSequencia < 5) {
                            maxSequencia++;
                        }
                        estado = GERAR_POSICAO;
                    } else {
                        estado = MOVIMENTAR_CURSOR;
                    }
                } else {
                    exibirErro();
                    maxSequencia = 1;
                    estado = BEM_VINDO;
                }
                break;
        }
    }
}

void config(){
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
}


int main() {
    config();
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
