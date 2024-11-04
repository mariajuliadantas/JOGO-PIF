/**
 * main.h
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
*/

#include <string.h> /**commit teste */

#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include <stdio.h>
#include "keyboard.h"
#include "screen.h"
#include "timer.h"

int main() {
    int playerX = MINX + 1;  // Posição inicial do jogador
    int playerY = MINY + 1;
    char input;
    int moveDelay = 200; // Tempo de delay entre movimentos, em ms

    // Inicializar a tela, o teclado e o temporizador
    screenInit(1);           // Desenha as bordas
    keyboardInit();
    timerInit(moveDelay);     // Define o delay entre movimentos

    // Posiciona o jogador inicial
    screenGotoxy(playerX, playerY);
    printf("O");        // Desenha o jogador
    fflush(stdout);

    while (1) {
        if (timerTimeOver()) {  // Verifica se o delay foi atingido

            if (keyhit()) {     // Verifica se uma tecla foi pressionada
                input = readch();

                // Calcula a nova posição do jogador com base na entrada
                int newX = playerX;
                int newY = playerY;

                switch (input) {
                    case 'w': newY--; break;  // Cima
                    case 's': newY++; break;  // Baixo
                    case 'a': newX--; break;  // Esquerda
                    case 'd': newX++; break;  // Direita
                    case 'q':  // Sair do loop se 'q' for pressionado
                        screenDestroy();
                        keyboardDestroy();
                        timerDestroy();
                        return 0;
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
                    printf("O");
                    fflush(stdout);
                }

                // Reseta o temporizador após o movimento
                timerUpdateTimer(moveDelay);
            }
        }

        usleep(5000);  // Intervalo pequeno para evitar sobrecarga de CPU
    }

    screenDestroy();
    keyboardDestroy();
    timerDestroy();
    return 0;
}
