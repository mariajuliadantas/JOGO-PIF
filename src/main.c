#include <stdio.h>
#include <stdlib.h> 
#include <time.h>   
#include <unistd.h> 
#include "keyboard.h"
#include "timer.h"
#include <string.h>  
#include "screen.h"
#undef MAXX
#undef MAXY

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
#define INVULNERABILITY_DURATION 10  


char map[MAP_HEIGHT][MAP_WIDTH] = {
    "#######################################################",
    "#######################################################",
    "##                 #                   #              #",
    "##    #      P     #                   #       P      #",
    "##    #            #                                  #",
    "##########   ################          ################",
    "##    #                     #                         #",
    "##    #######               #                         #",
    "##                          #                         #",
    "##      ########   #####    ################      #  ##",
    "##                 #        #                     #   #",
    "##      ########   #        #                     #   #",
    "##      #                                    ######   #",
    "##      #      #########    #   ####         #        #",
    "##  #####      #            #      #         #  #######",
    "##             #            #      #         #        #",
    "##                                 #         #######  #",
    "###  ##    ####     ###    ###     #####     #        #",
    "##    #             #        #               #  #######",
    "## P  #             #        #                        #",
    "#######################################################"
};


typedef struct {
    int x;
    int y;
    int dx; 
    int dy; 
    int isHorizontal;
} Enemy;


void displayStats(int lives, int keys);


time_t iniciarCronometro() {
    return time(NULL); 
}

char jogador[50]; 

void finalizarCronometro(time_t tempoInicial) {
    time_t tempoFinal = time(NULL); 
    double tempoDecorrido = difftime(tempoFinal, tempoInicial);  


    FILE *file = fopen("tempos_tesouro.txt", "a");
    if (file != NULL) {
        fprintf(file, "Tempo para encontrar o tesouro: %.0f segundos\n", tempoDecorrido);
        fclose(file);
    } else {
        printf("Erro ao salvar o tempo.\n");
    }


    printf("Você encontrou o tesouro em %.0f segundos!\n", tempoDecorrido);
}

void exibirHistoria() {
    screenClear();
    screenGotoxy(0, 0);
    printf("Bem-vindo ao grande desafio do Caça ao Tesouro!\n");
    printf("Você é um aventureiro corajoso em busca de um lendário tesouro escondido.\n");
    printf("Dizem que ele está guardado neste mapa misterioso, mas há perigos à espreita.\n\n");
    printf("Pressione 'Enter' para continuar...\n");
    getchar();
}


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


void drawmap(int treasureX, int treasureY, int lives, int keys) {
    screenClear();
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            screenGotoxy(x, y);
            char cell = map[y][x];
            if (cell == '#') {
                screenSetColor(COLOR_WALL, BLACK);
                printf("▓");  
            } else if (cell == 'P') {
                screenSetColor(YELLOW, BLACK);
                printf("P"); 
            } else {
                screenSetColor(COLOR_FLOOR, BLACK);
                printf(" ");  
            }
        }
    }
   
    screenGotoxy(treasureX, treasureY);
    screenSetColor(YELLOW, BLACK);
    printf("T");
    screenSetColor(WHITE, BLACK);
    
   
    displayStats(lives, keys);
    fflush(stdout);
}


void displayStats(int lives, int keys) {
    screenGotoxy(0, MAXY + 1);
    printf("Vidas: %d | Chaves: %d", lives, keys);
    fflush(stdout);
}


int fazerPergunta() {
    int resposta;
    screenClear();
    screenGotoxy(0, 0);

    // Gera uma pergunta aleatória (1 a 3)
    int perguntaAleatoria = rand() % 3 + 1;

    switch (perguntaAleatoria) {
        case 1:
            printf("Pergunta 1:\n");
            printf("Qual é a capital da França?\n");
            printf("1. Berlim\n2. Madri\n3. Paris\n");
            break;
        case 2:
            printf("Pergunta 2:\n");
            printf("Qual é o maior planeta do sistema solar?\n");
            printf("1. Marte\n2. Júpiter\n3. Saturno\n");
            break;
        case 3:
            printf("Pergunta 3:\n");
            printf("Qual é o animal terrestre mais rápido?\n");
            printf("1. Guepardo\n2. Cavalo\n3. Leão\n");
            break;
        default:
            printf("Erro ao selecionar pergunta.\n");
            return 0;
    }

    printf("Digite a sua resposta (1, 2 ou 3): ");
    scanf("%d", &resposta);

    // Verifica se a resposta está correta
    int acertou = 0;
    if ((perguntaAleatoria == 1 && resposta == 3) ||  
        (perguntaAleatoria == 2 && resposta == 2) ||  
        (perguntaAleatoria == 3 && resposta == 1)) {  
        acertou = 1;
    }

    screenClear();
    return acertou;
}

void movePlayer(int *playerX, int *playerY, char direction, int *lives, int *keys, int treasureX, int treasureY) {
    int newX = *playerX;
    int newY = *playerY;

   
    switch (direction) {
        case 'w': case 'A': newY--; break;  
        case 's': case 'B': newY++; break;  
        case 'a': case 'D': newX--; break;  
        case 'd': case 'C': newX++; break;  
    }

   
    if (newX < MINX || newX >= MAXX || newY < MINY || newY >= MAXY || map[newY][newX] == '#') {
        (*lives)--;  
        displayStats(*lives, *keys);

        
        if (*lives <= 0) {
            screenClear();
            screenGotoxy(0, 0);
            printf("Game Over! Você perdeu todas as vidas.\n");
            fflush(stdout);
            usleep(3000000);
            exit(0);
        }
    } else {
        
        if (map[newY][newX] == 'P') {
            screenClear();
            screenGotoxy(0, 0);
            printf("Você encontrou uma pista!\n");

            if (fazerPergunta()) {
                (*keys)++;  
                printf("Resposta correta! Você ganhou uma chave.\n");
            } else {
                (*lives)--;  
                printf("Resposta incorreta! Você perdeu uma vida.\n");
            }
            displayStats(*lives, *keys);
            usleep(2000000); 

          
            map[newY][newX] = ' ';
            drawmap(treasureX, treasureY, *lives, *keys);  
        }

        
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


void moveEnemy(Enemy *enemy) {
   
    if (enemy->dx == 0 && enemy->dy == 0) {
       
        enemy->dx = (rand() % 2 == 0) ? 1 : -1;  
        enemy->dy = 0;  
    }

   
    int newX = enemy->x + enemy->dx;
    int newY = enemy->y;  

   
    if (newX >= MINX && newX < MAXX && newY >= MINY && newY < MAXY) {
        char nextPos = map[newY][newX];

        
        if (nextPos == '#') {
           
            enemy->dx = -enemy->dx;
        } else if (nextPos == 'T' || nextPos == 'P') {
          
            enemy->dx = -enemy->dx;
        } else {
           
            screenGotoxy(enemy->x, enemy->y);
            printf(" ");
            fflush(stdout);

            enemy->x = newX;

           
            screenGotoxy(enemy->x, enemy->y);
            screenSetColor(RED, BLACK);
            printf("E");
            screenSetColor(WHITE, BLACK);
            fflush(stdout);
        }
    } else {
        
        enemy->dx = -enemy->dx;
    }
}



int checkCollision(int playerX, int playerY, Enemy enemies[], int *lives, int *invulnerable_counter) {
    if (*invulnerable_counter > 0) {  
        (*invulnerable_counter)--;
        return 0;
    }

    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (playerX == enemies[i].x && playerY == enemies[i].y) {
            (*lives)--;  
            displayStats(*lives, 0);
            *invulnerable_counter = INVULNERABILITY_DURATION;  
            if (*lives <= 0) {
                return 1;  
            }
            break;
        }
    }
    return 0;
}


void exibirUltimaLinha() {
    FILE *file = fopen("tempos_tesouro.txt", "r");
    
    if (file == NULL) {
        printf("Nenhum tempo registrado ou erro ao abrir o arquivo.\n");
        return;
    }

    char linha[256];
    char ultimaLinha[256] = ""; 

    
    while (fgets(linha, sizeof(linha), file)) {
        strcpy(ultimaLinha, linha);
    }

   
    printf("\n--- Último Tempo Registrado ---\n");
    printf("%s", ultimaLinha);
    printf("-------------------------------\n");

    fclose(file);
}




int main() {
    srand(time(NULL));
    
    
    screenInit(0);
    keyboardInit();
    timerInit(20);

    
    exibirHistoria();
    exibirInstrucoes();
    time_t tempoInicial = iniciarCronometro();
    
    int playerX = MINX + 1;
    int playerY = MINY + 1;
    int treasureX = 50;  
    int treasureY = 19;
    int lives = INITIAL_LIVES;
    int keys = 0; 
    int invulnerable_counter = 0;  
    char input;
  
    
    
    drawmap(treasureX, treasureY, lives, keys);

   
    screenGotoxy(playerX, playerY);
    printf("X");
    fflush(stdout);

   
Enemy *enemies = (Enemy *)malloc(NUM_ENEMIES * sizeof(Enemy));
if (enemies == NULL) {    
    exit(EXIT_FAILURE); 
}


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




free(enemies);


   
    while (lives > 0) {
 
        if (keyhit()) {
            input = readch();
            movePlayer(&playerX, &playerY, input, &lives, &keys, treasureX, treasureY);

           
            if (playerX == treasureX && playerY == treasureY) {
                 finalizarCronometro(tempoInicial);
                if (keys >= 3) {
                    screenClear();
                    screenGotoxy(0, 0);
                    printf("Parabéns, você resgatou o tesouro perdido!\n");
                    exibirUltimaLinha();
                    fflush(stdout);
                    usleep(3000000);  
                    break;  
                } else {
                    printf("\nVocê encontrou o baú, mas precisa de 3 chaves para abrir!\n");
                }
            }
        }

       
        for (int i = 0; i < NUM_ENEMIES; i++) {
            
            moveEnemy(&enemies[i]);
            usleep(10000); 
        }

       
        if (checkCollision(playerX, playerY, enemies, &lives, &invulnerable_counter)) {
            break;  
        }

        usleep(5000); 
    }
    
   
    screenDestroy();
    keyboardDestroy();
    timerDestroy();
    return 0;
}
