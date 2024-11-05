#include <stdio.h>
#include <stdlib.h> // Para rand() e srand()
#include <time.h>   // Para time()
#include <unistd.h> // Para usleep()
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define MINX 1
#define MAXX 80 
#define MINY 1
#define MAXY 24
#define INITIAL_LIVES 3
#define NUM_ENEMIES 3

typedef struct {
    int x;
    int y;
    int dx; // Direção no eixo X (-1 para esquerda, 1 para direita)
    int dy; // Direção no eixo Y (-1 para cima, 1 para baixo)
    int isHorizontal; // 1 se mover horizontalmente, 0 se verticalmente
} Enemy;

void drawTreasure(int treasureX, int treasureY) {
    screenGotoxy(treasureX, treasureY);
    screenSetColor(YELLOW, DARKGRAY); // Definindo a cor do tesouro
    printf("T");
    fflush(stdout);
}

void drawEnemy(Enemy *enemy) {
    screenGotoxy(enemy->x, enemy->y);
    screenSetColor(RED, DARKGRAY); // Definindo a cor do inimigo
    printf("E");
    fflush(stdout);
}

void clearEnemy(Enemy *enemy) {
    screenGotoxy(enemy->x, enemy->y);
    printf(" "); // Limpa a posição do inimigo
    fflush(stdout);
}

int canMoveTo(int x, int y, int treasureX, int treasureY) {
    // Verifica se a posição está dentro dos limites e não colide com o tesouro
    return (x > MINX && x < MAXX && y > MINY && y < MAXY && (x != treasureX || y != treasureY));
}

void moveEnemy(Enemy *enemy, int treasureX, int treasureY) {
    clearEnemy(enemy); // Limpa a posição atual do inimigo

    int newX = enemy->x + enemy->dx; // Nova posição X
    int newY = enemy->y + enemy->dy; // Nova posição Y

    // Verifica limites e muda direção se necessário
    if (enemy->isHorizontal) {
        // Movimentação horizontal
        if (newX >= MAXX || newX <= MINX) {
            enemy->dx = -enemy->dx; // Inverte a direção
            newX = enemy->x + enemy->dx; // Recalcula a nova posição X
        }
    } else {
        // Movimentação vertical
        if (newY >= MAXY || newY <= MINY) {
            enemy->dy = -enemy->dy; // Inverte a direção
            newY = enemy->y + enemy->dy; // Recalcula a nova posição Y
        }
    }

    // Atualiza a posição do inimigo
    enemy->x = newX;
    enemy->y = newY;

    // Desenha o inimigo na nova posição se for um movimento válido
    if (canMoveTo(enemy->x, enemy->y, treasureX, treasureY)) {
        drawEnemy(enemy);
    } else {
        // Caso não possa se mover, apenas inverte a direção
        if (enemy->isHorizontal) {
            enemy->dx = -enemy->dx;
        } else {
            enemy->dy = -enemy->dy;
        }
    }
}

void displayLives(int remainingLives) {
    screenGotoxy(2, MAXY + 1);  // Exibe embaixo da borda, ajustável conforme o layout
    printf("Vidas restantes: %d  ", remainingLives);
    fflush(stdout);
}

void gameOver() {
    screenClear();
    screenGotoxy(MAXX / 2 - 5, MAXY / 2);  // Centraliza o texto na tela
    printf("Game Over! Suas vidas acabaram.\n");
    fflush(stdout);
    getchar(); // Aguarda uma entrada para encerrar o jogo
    screenDestroy();
    keyboardDestroy();
    timerDestroy();
    exit(0);
}

int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    int playerX = MINX + 1;  // Posição inicial do jogador
    int playerY = MINY + 1;
    int treasureX = rand() % (MAXX - MINX - 1) + MINX + 1; // Gera uma posição aleatória para o tesouro
    int treasureY = rand() % (MAXY - MINY - 1) + MINY + 1;
    char input;
    int moveDelay = 20; // Tempo de delay entre movimentos, em ms
    int remainingLives = INITIAL_LIVES;

    // Inicializar inimigos
    Enemy enemies[NUM_ENEMIES];
    for (int i = 0; i < NUM_ENEMIES; i++) {
        do {
            enemies[i].x = rand() % (MAXX - MINX - 1) + MINX + 1;
            enemies[i].y = rand() % (MAXY - MINY - 1) + MINY + 1;
        } while (!canMoveTo(enemies[i].x, enemies[i].y, treasureX, treasureY)); // Garante que o inimigo inicie em uma posição válida

        enemies[i].isHorizontal = rand() % 2; // Alterna entre movimento horizontal (1) e vertical (0)
        enemies[i].dx = enemies[i].isHorizontal ? 1 : 0; // Define a direção inicial
        enemies[i].dy = enemies[i].isHorizontal ? 0 : 1; // Define a direção inicial
        drawEnemy(&enemies[i]);
    }

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

    // Exibe as vidas iniciais
    displayLives(remainingLives);

    while (1) {
        if (timerTimeOver()) {  // Verifica se o delay foi atingido

            // Move os inimigos
            for (int i = 0; i < NUM_ENEMIES; i++) {
                moveEnemy(&enemies[i], treasureX, treasureY);
            }

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
                if (newX <= MINX || newX >= MAXX || newY <= MINY || newY >= MAXY) {
                    remainingLives--;  // Perde uma vida ao colidir com a borda
                    displayLives(remainingLives);  // Atualiza as vidas na tela
                    if (remainingLives <= 0) {
                        gameOver();  // Encerra o jogo se as vidas acabarem
                    }
                } else {
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

                // Verifica colisão com inimigos
                for (int i = 0; i < NUM_ENEMIES; i++) {
                    if (playerX == enemies[i].x && playerY == enemies[i].y) {
                        remainingLives--; // Perde uma vida ao colidir com um inimigo
                        displayLives(remainingLives);  // Atualiza as vidas na tela
                        if (remainingLives <= 0) {
                            gameOver();  // Encerra o jogo se as vidas acabarem
                        }
                        break; // Sai do loop para evitar múltiplas contagens de vida
                    }
                }
            }

            usleep(5000); // Pequena pausa para evitar sobrecarga da CPU
        }
    }

    screenDestroy();
    keyboardDestroy();
    timerDestroy();
    return 0;
}
