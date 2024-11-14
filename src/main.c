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

// Mapa do jogo com pistas
char map[MAP_HEIGHT][MAP_WIDTH] = {
    "#######################################################",
    "#######################################################",
    "##                 #                   #              #",
    "##    #      P     #                   #       P      #",
    "##    #            #                                  #",
    "##    ####   ################          ################",
    "##    #      P              #                         #",
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

// Declaração da função para evitar avisos de declaração implícita
void displayStats(int lives, int keys);

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
    printf("- Você tem %d vidas. Cada encontro com um inimigo, uma parede ou uma resposta errada a uma pergunta custa uma vida.\n", INITIAL_LIVES);
    printf("- Encontre o tesouro para vencer!\n- Encontre pistas (marcadas como 'P') para responder perguntas e acumular chaves.\n\n");
    printf("Pressione 'Enter' para começar a aventura...\n");
    getchar();
}

// Função para desenhar o mapa na tela e exibir tesouro, vidas e chaves
void drawmap(int treasureX, int treasureY, int lives, int keys) {
    screenClear();
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            screenGotoxy(x, y);
            char cell = map[y][x];
            if (cell == '#') {
                screenSetColor(COLOR_WALL, BLACK);
                printf("▓");  // Desenha parede
            } else if (cell == 'P') {
                screenSetColor(YELLOW, BLACK);
                printf("P");  // Desenha pista
            } else {
                screenSetColor(COLOR_FLOOR, BLACK);
                printf(" ");  // Desenha espaço vazio
            }
        }
    }
    // Desenha o tesouro
    screenGotoxy(treasureX, treasureY);
    screenSetColor(YELLOW, BLACK);
    printf("T");
    screenSetColor(WHITE, BLACK);
    
    // Exibe o número de vidas e chaves
    displayStats(lives, keys);
    fflush(stdout);
}

// Função para exibir o número de vidas e chaves acumuladas
void displayStats(int lives, int keys) {
    screenGotoxy(0, MAXY + 1);
    printf("Vidas: %d | Chaves: %d", lives, keys);
    fflush(stdout);
}

// Função para apresentar uma pergunta ao jogador
int fazerPergunta() {
    int resposta;
    screenClear();
    screenGotoxy(0, 0);
    printf("Pergunta:\n");
    printf("Qual é a capital da França?\n");
    printf("1. Berlim\n2. Madri\n3. Paris\n");
    printf("Digite a sua resposta (1, 2 ou 3): ");
    scanf("%d", &resposta);
    
    int acertou = (resposta == 3); // 1 se a resposta estiver correta, 0 caso contrário
    
    screenClear(); // Limpa a tela para retornar ao mapa
    
    return acertou;
}

// Função para movimentar o jogador com verificações de limites, paredes, pistas e perda de vida
void movePlayer(int *playerX, int *playerY, char direction, int *lives, int *keys, int treasureX, int treasureY) {
    int newX = *playerX;
    int newY = *playerY;

    // Define a nova posição com base na direção de movimento
    switch (direction) {
        case 'w': case 'A': newY--; break;  // Move para cima
        case 's': case 'B': newY++; break;  // Move para baixo
        case 'a': case 'D': newX--; break;  // Move para a esquerda
        case 'd': case 'C': newX++; break;  // Move para a direita
    }

    // Verifica se o jogador colidiu com uma parede
    if (newX < MINX || newX >= MAXX || newY < MINY || newY >= MAXY || map[newY][newX] == '#') {
        (*lives)--;  // Reduz uma vida ao colidir com uma parede ou sair dos limites
        displayStats(*lives, *keys);

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
        // Verifica se o jogador encontrou uma pista
        if (map[newY][newX] == 'P') {
            screenClear();
            screenGotoxy(0, 0);
            printf("Você encontrou uma pista!\n");

            if (fazerPergunta()) {
                (*keys)++;  // Ganha uma chave ao responder corretamente
                printf("Resposta correta! Você ganhou uma chave.\n");
            } else {
                (*lives)--;  // Perde uma vida ao errar a resposta
                printf("Resposta incorreta! Você perdeu uma vida.\n");
            }
            displayStats(*lives, *keys);
            usleep(2000000);  // Pausa para mostrar o resultado da pergunta

            // Remove a pista do mapa para que não seja ativada novamente
            map[newY][newX] = ' ';
            drawmap(treasureX, treasureY, *lives, *keys);  // Redesenha o mapa com o tesouro e stats após a interação
        }

        // Move o jogador para a nova posição
        screenGotoxy(*playerX, *playerY);
        printf(" ");
        fflush(stdout);

        *playerX = newX;
        *playerY = newY;

        screenGotoxy(*playerX, *playerY);
        printf("X");
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
            displayStats(*lives, 0);
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
    int treasureX = 50;  // Posição fixa do tesouro
    int treasureY = 19;
    int lives = INITIAL_LIVES;
    int keys = 0;  // Contador de chaves coletadas
    int invulnerable_counter = 0;  // Contador de invulnerabilidade
    char input;
    
    // Desenha o mapa com o tesouro e as estatísticas iniciais
    drawmap(treasureX, treasureY, lives, keys);

    // Posiciona o jogador
    screenGotoxy(playerX, playerY);
    printf("X");
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

    // Loop principal do jogo
    while (lives > 0) {
        if (keyhit()) {
            input = readch();
            movePlayer(&playerX, &playerY, input, &lives, &keys, treasureX, treasureY);

            // Verifica se o jogador encontrou o tesouro e tem 3 chaves
            if (playerX == treasureX && playerY == treasureY) {
                if (keys >= 3) {
                    screenClear();
                    screenGotoxy(0, 0);
                    printf("Parabéns, você resgatou o tesouro perdido!\n");
                    fflush(stdout);
                    usleep(3000000);  // Mantém a mensagem por 3 segundos
                    break;  // Encerra o loop e o jogo
                } else {
                    printf("\nVocê encontrou o baú, mas precisa de 3 chaves para abrir!\n");
                }
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
