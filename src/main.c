#include <stdio.h>
#include <stdlib.h> // Para rand() e srand()
#include <time.h>   // Para time()
#include <unistd.h> // Para usleep()
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

// Definição de limites e configurações do jogo
#define MINX 1
#define MINY 1
#define MAXX MAP_WIDTH
#define MAXY MAP_HEIGHT
#define INITIAL_LIVES 3
#define NUM_ENEMIES 5
#define MAP_WIDTH 55
#define MAP_HEIGHT 21
#define COLOR_WALL BLUE
#define COLOR_FLOOR BLACK
#define INVULNERABILITY_DURATION 10  // Duração da invulnerabilidade temporária após uma colisão

// Mapa do jogo
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

// Estrutura para os inimigos
typedef struct {
    int x;
    int y;
    int dx; // Direção no eixo X (-1 ou 1)
    int dy; // Direção no eixo Y (-1 ou 1)
} Enemy;

// Função de exibição da história do jogo
void exibirHistoria() {
    screenClear();
    screenGotoxy(0, 0);
    printf("Bem-vindo ao grande desafio do Caça ao Tesouro!\n");
    printf("Você é um aventureiro corajoso em busca de um lendário tesouro escondido.\n");
    printf("Dizem que ele está guardado neste mapa misterioso, mas há perigos à espreita.\n\n");
    printf("Pressione 'Enter' para continuar...\n");
    getchar();
}

// Função de exibição das instruções do jogo
void exibirInstrucoes() {
    screenClear();
    screenGotoxy(0, 0);
    printf("Instruções do Jogo:\n");
    printf("- Use as teclas W, A, S, D ou as setas para se mover.\n");
    printf("- Você tem %d vidas. Cada encontro com um inimigo ou uma parede custa uma vida.\n", INITIAL_LIVES);
    printf("- Encontre o tesouro para vencer!\n\n");
    printf("Pressione 'Enter' para começar a aventura...\n");
    getchar();
}

// Função para desenhar o mapa na tela
void drawmap() {
    screenClear();
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            screenGotoxy(x, y);
            char cell = map[y][x];
            if (cell == '#') {
                screenSetColor(COLOR_WALL, BLACK);
                printf("▓");  // Desenha parede
            } else {
                screenSetColor(COLOR_FLOOR, BLACK);
                printf(" ");  // Desenha espaço vazio
            }
        }
    }
    screenSetColor(WHITE, BLACK);
    fflush(stdout);
}

// Função para exibir o número de vidas restantes
void displayLives(int lives) {
    screenGotoxy(0, MAXY + 1);
    printf("Vidas restantes: %d  ", lives);
    fflush(stdout);
}

// Função para movimentar o jogador com verificações de limites, paredes e perda de vida
void movePlayer(int *playerX, int *playerY, char direction, int *lives) {
    int newX = *playerX;
    int newY = *playerY;

    // Define a nova posição com base na direção de movimento
    switch (direction) {
        case 'w': case 'A': newY--; break;  // Move para cima
        case 's': case 'B': newY++; break;  // Move para baixo
        case 'a': case 'D': newX--; break;  // Move para a esquerda
        case 'd': case 'C': newX++; break;  // Move para a direita
    }

    // Verifica se o jogador colidiu com uma parede ou está fora dos limites
    if (newX < MINX || newX >= MAXX || newY < MINY || newY >= MAXY || map[newY][newX] == '#') {
        (*lives)--;  // Reduz uma vida ao colidir com uma parede ou sair dos limites
        displayLives(*lives);

        // Verifica se as vidas acabaram
        if (*lives <= 0) {
            screenClear();
            screenGotoxy(0, 0);
            printf("Game Over! Você perdeu todas as vidas.\n");
            fflush(stdout);
            usleep(3000000);
            exit(0);
        }
    } else {
        // Move o jogador para a nova posição
        screenGotoxy(*playerX, *playerY);
        printf(" ");
        fflush(stdout);

        *playerX = newX;
        *playerY = newY;

        screenGotoxy(*playerX, *playerY);
        printf("P");
        fflush(stdout);
    }
}

// Função para movimentar os inimigos com verificações de limites e paredes
void moveEnemy(Enemy *enemy) {
    int newX = enemy->x + enemy->dx;
    int newY = enemy->y + enemy->dy;

    // Verifica se o novo movimento é válido (dentro do mapa e sem parede)
    if (newX >= MINX && newX < MAXX && newY >= MINY && newY < MAXY && map[newY][newX] != '#') {
        screenGotoxy(enemy->x, enemy->y);
        printf(" ");
        fflush(stdout);

        enemy->x = newX;
        enemy->y = newY;

        screenGotoxy(enemy->x, enemy->y);
        screenSetColor(RED, BLACK);
        printf("E");
        screenSetColor(WHITE, BLACK);
        fflush(stdout);
    } else {
        // Inverte a direção se houver uma parede ou se sair dos limites
        enemy->dx = -enemy->dx;
        enemy->dy = -enemy->dy;
    }
}

// Função para verificar colisão entre o jogador e os inimigos com invulnerabilidade temporária
int checkCollision(int playerX, int playerY, Enemy enemies[], int *lives, int *invulnerable_counter) {
    if (*invulnerable_counter > 0) {  // Verifica se o jogador está invulnerável
        (*invulnerable_counter)--;
        return 0;
    }

    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (playerX == enemies[i].x && playerY == enemies[i].y) {
            (*lives)--;  // Reduz uma vida ao colidir com um inimigo
            displayLives(*lives);
            *invulnerable_counter = INVULNERABILITY_DURATION;  // Define invulnerabilidade temporária
            if (*lives <= 0) {
                return 1;  // Game over
            }
            break;
        }
    }
    return 0;
}

// Função principal
int main() {
    srand(time(NULL));
    
    // Inicializa as bibliotecas
    screenInit(0);
    keyboardInit();
    timerInit(20);

    // Exibe a tela de história e instruções
    exibirHistoria();
    exibirInstrucoes();

    // Configurações iniciais do jogador, tesouro e inimigos
    int playerX = MINX + 1;
    int playerY = MINY + 1;
    int treasureX = rand() % (MAXX - MINX - 1) + MINX + 1;
    int treasureY = rand() % (MAXY - MINY - 1) + MINY + 1;
    int lives = INITIAL_LIVES;
    int invulnerable_counter = 0;  // Contador de invulnerabilidade
    char input;
    
    // Desenha o mapa
    drawmap();

    // Posiciona o jogador e o tesouro
    screenGotoxy(playerX, playerY);
    printf("P");
    fflush(stdout);

    screenGotoxy(treasureX, treasureY);
    screenSetColor(YELLOW, BLACK);
    printf("T");
    screenSetColor(WHITE, BLACK);
    fflush(stdout);

    // Inicializa os inimigos
    Enemy enemies[NUM_ENEMIES];
    for (int i = 0; i < NUM_ENEMIES; i++) {
        enemies[i].x = rand() % (MAXX - MINX - 1) + MINX + 1;
        enemies[i].y = rand() % (MAXY - MINY - 1) + MINY + 1;
        enemies[i].dx = (rand() % 2 == 0) ? 1 : -1;
        enemies[i].dy = (rand() % 2 == 0) ? 1 : -1;

        screenGotoxy(enemies[i].x, enemies[i].y);
        screenSetColor(RED, BLACK);
        printf("E");
        screenSetColor(WHITE, BLACK);
        fflush(stdout);
    }

    // Exibe o número de vidas iniciais
    displayLives(lives);

    // Loop principal do jogo
    while (lives > 0) {
        if (keyhit()) {
            input = readch();
            movePlayer(&playerX, &playerY, input, &lives);

            // Verifica se o jogador encontrou o tesouro
            if (playerX == treasureX && playerY == treasureY) {
                screenClear();
                screenGotoxy(0, 0);
                printf("Parabéns, você resgatou o tesouro perdido!\n");
                fflush(stdout);
                usleep(3000000);  // Mantém a mensagem por 3 segundos
                break;  // Encerra o loop e o jogo
            }
        }

        // Movimenta cada inimigo e verifica colisões
        for (int i = 0; i < NUM_ENEMIES; i++) {
            moveEnemy(&enemies[i]);
        }

        // Verifica colisão com inimigos e aplica invulnerabilidade temporária
        if (checkCollision(playerX, playerY, enemies, &lives, &invulnerable_counter)) {
            break;  // Game over
        }

        usleep(50000); // Atraso reduzido para dar fluidez ao jogo
    }

    // Finaliza o jogo e limpa a tela
    screenDestroy();
    keyboardDestroy();
    timerDestroy();
    return 0;
}
