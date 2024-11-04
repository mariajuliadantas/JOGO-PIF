#include <stdio.h>
#include <stdlib.h> // Para rand() e srand()
#include <time.h>   // Para time()
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define MINX 1
#define MAXX 80
#define MINY 1
#define MAXY 24

void drawTreasure(int treasureX, int treasureY) {
    screenGotoxy(treasureX, treasureY);
    screenSetColor(YELLOW, DARKGRAY); // Definindo a cor do tesouro
    printf("T");
    fflush(stdout);
}

int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    int playerX = MINX + 1;  // Posição inicial do jogador
    int playerY = MINY + 1;
    int treasureX = rand() % (MAXX - MINX - 1) + MINX + 1; // Gera uma posição aleatória para o tesouro
    int treasureY = rand() % (MAXY - MINY - 1) + MINY + 1;
    char input;
    int moveDelay = 20; // Tempo de delay entre movimentos, em ms

    // Inicializar a tela, o teclado e o temporizador
    screenInit(1);           // Desenha as bordas
    keyboardInit();
    timerInit(moveDelay);     // Define o delay entre movimentos

    // Posiciona o jogador inicial
    screenGotoxy(playerX, playerY);
    printf("P");        // Desenha o jogador
    fflush(stdout);

    // Desenha o tesouro na tela
    drawTreasure(treasureX, treasureY);

    while (1) {
        if (timerTimeOver()) {  // Verifica se o delay foi atingido

            if (keyhit()) {     // Verifica se uma tecla foi pressionada
                input = readch();

                // Se a tecla for a seta direcional (Escape sequence)
                if (input == 27 && readch() == '[') {
                    input = readch();
                }

                // Verifica as teclas WASD ou setas para mover
                int newX = playerX;
                int newY = playerY;

                switch (input) {
                    case 'w': newY--; break;  // Move para cima (W)
                    case 's': newY++; break;  // Move para baixo (S)
                    case 'a': newX--; break;  // Move para esquerda (A)
                    case 'd': newX++; break;  // Move para direita (D)
                    case 'A': newY--; break;  // Seta para cima
                    case 'B': newY++; break;  // Seta para baixo
                    case 'D': newX--; break;  // Seta para esquerda
                    case 'C': newX++; break;  // Seta para direita
                }

                // Verifica se a nova posição está dentro das bordas
                if (newX > MINX && newX < MAXX && newY > MINY && newY < MAXY) {
                    // Apaga o jogador na posição atual
                    screenGotoxy(playerX, playerY);
                    printf(" ");
                    fflush(stdout);

                    // Atualiza a posição do jogador
                    playerX = newX;
                    playerY = newY;

                    // Desenha o jogador na nova posição
                    screenGotoxy(playerX, playerY);
                    printf("P");
                    fflush(stdout);
                }

                // Verifica se o jogador alcançou o tesouro
                if (playerX == treasureX && playerY == treasureY) {
                    screenGotoxy(MINX, MAXY); // Move o cursor para a linha abaixo do mapa
                    printf("Você encontrou o tesouro! Jogo encerrado.\n");
                    fflush(stdout);
                    break; // Sai do loop e encerra o jogo
                }

                // Reseta o temporizador após o movimento
                timerUpdateTimer(moveDelay);

                if (input == 'q') { // Sair do loop se 'q' for pressionado
                    screenDestroy();
                    keyboardDestroy();
                    timerDestroy();
                    return 0;
                }
            }
        }

        usleep(5000);  // Intervalo pequeno para evitar sobrecarga de CPU
    }

    screenDestroy();
    keyboardDestroy();
    timerDestroy();
    return 0;
}
