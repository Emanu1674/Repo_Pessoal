#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

#define WIDTH 80
#define HEIGHT 25


// Essa função obrigatoriamente precisa de comentário pra ser entendida.
//
// A função recebe um ponteiro para char chamado "block" que contém o
//    caractere que será usado para preencher a tela, exemplo "█".
// É então definido um ponteiro para char com tamanho alocado = [LARGURA]
//    (do terminal) * tamanho de block, chamado bufferX.
// Conteúdo de block é então copiado para dentro de bufferX [LARGURA]
//    vezes, criando um vetor de char que preenche uma linha inteira.
// Esse vetor é impresso na tela [ALTURA] vezes, para encher o terminal
//    linha por linha, ao invés de caractere por caractere (muito lento).
//

void fillScreen(int color, int bgcolor, const char *block){
    char* bufferX = malloc(WIDTH * strlen(block));
    //if (bufferX == NULL){ bugCheck(0x1A);}     // 0x1A = OUT_OF_MEMORY
    for(int i = 0 ; i < WIDTH ; i++)
        strncpy(bufferX + i * strlen(block), block, strlen(block));
    printf("\033[%d;%dm", color, bgcolor+10);
    for(int H = 1 ; H <= HEIGHT ; H++)
        printf("\033[%d;%dH%s", H, 1, bufferX);
    printf("\033[0m");
    free(bufferX);
}

void drawPixel(int x, int y, int color, int bgcolor, const char *block){
    printf("\033[%d;%dm", color, bgcolor+10);
    printf("\033[%d;%dH%s", y, x, block);
    printf("\033[0m");
}

int main(){
    setlocale(LC_ALL, "en_us.UTF-8");
    fillScreen(34, 30, u8"█");
    //drawPixel(18, 5, 33, 31, u8"▄");
    //setColor(18, 5, 33, 31);
    getchar();
    system(CLEAR);
    return 0;
}

/*  Prototype of new implementation that fills the entire screen using
    a single buffer, same performance as filling one line at a time.
    Broken/Unused.

void fillScreenFULL(int color, int bgcolor, const char *block){
    char* bufferX = malloc(WIDTH * strlen(block) + 1);
    char* bufferY = malloc(HEIGHT * strlen(bufferX));
    //if (bufferX == NULL){ bugCheck(0x1A);}     // 0x1A = OUT_OF_MEMORY
    //if (bufferY == NULL){ bugCheck(0x1A);}
    for(int i = 0 ; i < WIDTH ; i++)
        strncpy(bufferX + i * strlen(block), block, strlen(block));
    bufferX[WIDTH * strlen(block)] = '\0';
    for(int i = 0 ; i < HEIGHT ; i++)
        strncpy(bufferY + i * strlen(bufferX), bufferX, strlen(bufferX));
    bufferY[HEIGHT * strlen(bufferX)] = '\0';
    printf("\033[%d;%dm", color, bgcolor+10);
    printf("\033[1;1H%s", bufferY);
    printf("\033[0m");
    free(bufferX);
    free(bufferY);
    fflush(stdin);
}*/

/*  Old function, fills the screen one char at a time

void fillScreen_OLD(int color, int bgcolor, const char *block){
    printf("\033[%d;%dm", color, bgcolor+10);
    for(int H = 0 ; H <= HEIGHT ; H++){
        for(int W = 0 ; W <= WIDTH ; W++)
            printf("\033[%d;%dH%s", H, W, block);
    }
}*/