
#include <stdio.h>
#include <stdlib.h> // Para rand() e srand()
#include <time.h>   // Para time()
#include <unistd.h> // Para usleep()
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define MINX 1
#define MAXX 55 
#define MINY 1
#define MAXY 21
#define INITIAL_LIVES 3
#define NUM_ENEMIES 3
#define MAP_WIDTH 55
#define MAP_HEIGHT 21
#define COLOR_WALL BLUE
#define COLOR_FLOOR LIGHTGRAY

char map[MAP_HEIGHT][MAP_WIDTH] = {
    "#######################################################",
    "#######################################################",
    "##                 #                   #              #",
    "##    #            #                   #              #",
    "##    #            #                                  #",
    "##    ####   ################          ################",
    "##    #                     #                         #",
    "##    #######               #                         #",
    "##                                                    #",
    "##      ########   #####    ##########            #####",
    "##                 #                              #   #",
    "##      ########   #                              #   #",
    "##      #                                    ######   #",
    "##      #      #########    #   ####         #        #",
    "##             #            #      #         #  #######",
    "##             #            #      #         #        #",
    "##                                 #         #######  #",
    "#######    ####     ###    ###     ###       #        #",
    "##                  #        #               #  #######",
    "##                  #        #                        #",
    "#######################################################"
};


typedef struct {
    int x;
    int y;
    int dx; // Direção no eixo X (-1 para esquerda, 1 para direita)
    int dy; // Direção no eixo Y (-1 para cima, 1 para baixo)
    int isHorizontal; // 1 se mover horizontalmente, 0 se verticalmente
} Enemy;

void drawmap(){

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            char cell = map[y][x];
            screenGotoxy(x, y);
            switch(cell) {
                case '#':
                    screenSetColor(COLOR_WALL, BLACK);
                    printf("▓");
                    break;
                default:
                    screenSetColor(COLOR_FLOOR, BLACK);
                    printf(" ");
                    break;
            }
        }
    }
    screenSetColor(WHITE, BLACK);
    fflush(stdout);
}

void drawTreasure(int treasureX, int treasureY) {
    screenGotoxy(treasureX, treasureY);
    screenSetColor(YELLOW, DARKGRAY);
    printf("T");
    fflush(stdout);
}

void drawEnemy(Enemy *enemy) {
    screenGotoxy(enemy->x, enemy->y);
    screenSetColor(RED, DARKGRAY);
    printf("E");
    fflush(stdout);
}

void clearEnemy(Enemy *enemy) {
    screenGotoxy(enemy->x, enemy->y);
    printf(" ");
    fflush(stdout);
}

int canMoveTo(int x, int y, int treasureX, int treasureY) {
    return (x > MINX && x < MAXX && y > MINY && y < MAXY && (x != treasureX || y != treasureY));
}

void moveEnemy(Enemy *enemy, int treasureX, int treasureY) {
    clearEnemy(enemy);

    int newX = enemy->x + enemy->dx;
    int newY = enemy->y + enemy->dy;

    if (enemy->isHorizontal) {
        if (newX >= MAXX || newX <= MINX) {
            enemy->dx = -enemy->dx;
            newX = enemy->x + enemy->dx;
        }
    } else {
        if (newY >= MAXY || newY <= MINY) {
            enemy->dy = -enemy->dy;
            newY = enemy->y + enemy->dy;
        }
    }

    enemy->x = newX;
    enemy->y = newY;

    if (canMoveTo(enemy->x, enemy->y, treasureX, treasureY)) {
        drawEnemy(enemy);
    } else {
        if (enemy->isHorizontal) {
            enemy->dx = -enemy->dx;
        } else {
            enemy->dy = -enemy->dy;
        }
    }
}

void displayLives(int remainingLives) {
    screenGotoxy(2, MAXY + 1);
    printf("Vidas restantes: %d  ", remainingLives);
    fflush(stdout);
}

void gameOver() {
    screenClear();
    screenGotoxy(MAXX / 2 - 5, MAXY / 2);
    printf("Game Over! Suas vidas acabaram.\n");
    fflush(stdout);
    getchar();
    screenDestroy();
    keyboardDestroy();
    timerDestroy();
    exit(0);
}

void checkCollisionWithEnemies(int *playerX, int *playerY, Enemy enemies[], int *remainingLives) {
    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (*playerX == enemies[i].x && *playerY == enemies[i].y) {
            (*remainingLives)--;  // Reduz uma vida ao colidir com um inimigo
            displayLives(*remainingLives);

            if (*remainingLives <= 0) {
                gameOver();  // Termina o jogo apenas se as vidas acabarem
            } else {
                // Coloca o jogador de volta no ponto inicial seguro
                screenGotoxy(*playerX, *playerY);
                printf(" ");
                *playerX = MINX + 1;
                *playerY = MINY + 1;
                screenGotoxy(*playerX, *playerY);
                printf("P");
                fflush(stdout);
            }
            break;  // Sai do loop para evitar múltiplas reduções de vida no mesmo ciclo
        }
    }
}


int main() {
    srand(time(NULL));

    int playerX = MINX + 1;
    int playerY = MINY + 1;
    int treasureX = rand() % (MAXX - MINX - 1) + MINX + 1;
    int treasureY = rand() % (MAXY - MINY - 1) + MINY + 1;
    char input;
    int moveDelay = 20;
    int remainingLives = INITIAL_LIVES;

    screenInit(0);
    keyboardInit();
    timerInit(moveDelay); //delay

    drawmap();

    drawTreasure(treasureX, treasureY);
    displayLives(remainingLives);

    screenGotoxy(playerX, playerY);
    printf("P");
    fflush(stdout);

    
    Enemy enemies[NUM_ENEMIES];
    for (int i = 0; i < NUM_ENEMIES; i++) {
        do {
            enemies[i].x = rand() % (MAXX - MINX - 1) + MINX + 1;
            enemies[i].y = rand() % (MAXY - MINY - 1) + MINY + 1;
        } while (!canMoveTo(enemies[i].x, enemies[i].y, treasureX, treasureY));

        enemies[i].isHorizontal = rand() % 2;
        enemies[i].dx = enemies[i].isHorizontal ? 1 : 0;
        enemies[i].dy = enemies[i].isHorizontal ? 0 : 1;
        drawEnemy(&enemies[i]);
    }

    while (1) {
        if (timerTimeOver()) {
            for (int i = 0; i < NUM_ENEMIES; i++) {
                moveEnemy(&enemies[i], treasureX, treasureY);
                checkCollisionWithEnemies(&playerX, &playerY, enemies, &remainingLives);
            }

            if (keyhit()) {
                input = readch();

                if (input == 27 && readch() == '[') {
                    input = readch();
                }

                int newX = playerX;
                int newY = playerY;

                switch (input) {
                    case 'w': newY--; break;
                    case 's': newY++; break;
                    case 'a': newX--; break;
                    case 'd': newX++; break;
                    case 'A': newY--; break;
                    case 'B': newY++; break;
                    case 'D': newX--; break;
                    case 'C': newX++; break;
                }

                if (newX <= MINX || newX >= MAXX || newY <= MINY || newY >= MAXY) {
                    remainingLives--;
                    displayLives(remainingLives);
                    if (remainingLives <= 0) {
                        gameOver();
                    }
                } else {
                    screenGotoxy(playerX, playerY);
                    printf(" ");
                    fflush(stdout);

                    playerX = newX;
                    playerY = newY;
                                                                                                                                                                                                     
                    screenGotoxy(playerX, playerY);
                    printf("P");
                    fflush(stdout);
                }

                if (playerX == treasureX && playerY == treasureY) {
                    screenGotoxy(MINX, MAXY);
                    printf("Você encontrou o tesouro! Jogo encerrado.\n");
                    fflush(stdout);
                    break;
                }

                checkCollisionWithEnemies(&playerX, &playerY, enemies, &remainingLives);
            }

            usleep(5000);
        }
    }

    screenDestroy();
    keyboardDestroy();
    timerDestroy();
    return 0;
}