#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#ifdef _WIN32
#include <windows.h>
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

void fill_Screen(int _FGColor, int _BGColor, char* _Char);
void draw_Rectangle(int _xPos1, int _yPos1, int _xPos2, int _yPos2, int _FGColor, int _BGColor, char* _Char);
void draw_Filled_Rectangle(int _xPos1, int _yPos1, int _xPos2, int _yPos2, int _FGColor, int _BGColor, char* _Char);
void draw_Line(int _xPos, int _yPos, int _xSize, int _ySize, int _FGColor, int _BGColor, char* _Char);
void draw_Pixel(int _xPos, int _yPos, int _FGColor, int _BGColor, char* _Char);

int main(){
    setlocale(LC_ALL, "en_us.UTF-8");
    fill_Screen(34, 34, u8"█");
    printf("\033[?25l");
    draw_Line(6, 4, 70, 0, 96, 34, u8"─");
    //printf("\033[3;6H\033[96;44mMicrosoft ScanDisk\033[0m");
    draw_Line(6, 23, 70, 0, 96, 34, u8"─");

    // Botão Pause
    draw_Line(6, 21, 9, 0, 90, 39, u8"█");
    draw_Line(7, 22, 9, 0, 30, 34, u8"▀");
    draw_Pixel(15, 21, 30, 34, u8"▄");
    draw_Pixel(6, 21, 97, 90, u8"◄");
    draw_Pixel(14, 21, 97, 90, u8"►");

    // Botão More Info
    draw_Line(18, 21, 13, 0, 90, 39, u8"█");
    draw_Line(19, 22, 13, 0, 30, 34, u8"▀");
    draw_Pixel(31, 21, 30, 34, u8"▄");
    draw_Pixel(18, 21, 37, 90, u8"<");
    draw_Pixel(30, 21, 37, 90, u8">");

    // Botão Exit
    draw_Line(34, 21, 8, 0, 90, 39, u8"█");
    draw_Line(35, 22, 8, 0, 30, 34, u8"▀");
    draw_Pixel(42, 21, 30, 34, u8"▄");
    draw_Pixel(34, 21, 37, 90, u8"<");
    draw_Pixel(41, 21, 37, 90, u8">");

    // Barra de Progresso
    draw_Line(21, 24, 31, 0, 93, 93, u8"█");
    draw_Line(52, 24, 24, 0, 93, 34, u8"░");

    // Outras frescuras na tela
    draw_Pixel(14, 8, 96, 34, u8"√");
    draw_Pixel(14, 9, 96, 34, u8"√");
    draw_Pixel(14, 10, 96, 34, u8"√");
    draw_Pixel(14, 11, 96, 34, u8"»");

    getchar();
    system(CLEAR);
    return 0;
}

void draw_Rectangle(int x1, int y1, int x2, int y2, int color, int bgcolor, char* block){
    int R_WIDTH = abs(x2 - x1) + 1;
    int R_HEIGHT = abs(y2 - y1) + 1;

    char* buffer = malloc(R_WIDTH * strlen(block) + 1);
    //if (buffer == NULL){ bugCheck(0x1A);}     // 0x1A = OUT_OF_MEMORY

    for(int i = 0 ; i < R_WIDTH ; i++)
        strncpy(buffer + i * strlen(block), block, strlen(block));
    buffer[R_WIDTH * strlen(block)] = '\0';

    printf("\033[%d;%dm", color, bgcolor + 10);
    printf("\033[%d;%dH%s", y1, x1, buffer);
    for (int H = y1 + 1; H < y2; H++) {
        printf("\033[%d;%dH%s", H, x1, block); // Left border
        printf("\033[%d;%dH%s", H, x2, block); // Right border
    }

    printf("\033[%d;%dH%s", y2, x1, buffer);

    printf("\033[0m");
    free(buffer);
}

void draw_Filled_Rectangle(int x1, int y1, int x2, int y2, int color, int bgcolor, char* block){
    int R_WIDTH = abs(x2 - x1) + 1;
    int R_HEIGHT = abs(y2 - y1) + 1;
    char* buffer = malloc(R_WIDTH * strlen(block) + 1);
    //if (buffer == NULL){ bugCheck(0x1A);}     // 0x1A = OUT_OF_MEMORY
    
    for(int i = 0 ; i < R_WIDTH ; i++)
        strncpy(buffer + i * strlen(block), block, strlen(block));
    buffer[R_WIDTH * strlen(block)] = '\0';

    printf("\033[%d;%dm", color, bgcolor + 10);
    for(int H = y1 ; H <= y2 ; H++)
        printf("\033[%d;%dH%s", H, x1, buffer);

    printf("\033[0m");
    free(buffer);
}

void fill_Screen(int color, int bgcolor, char* block){
    char* buffer = malloc(WIDTH * strlen(block));
    //if (buffer == NULL){ bugCheck(0x1A);}     // 0x1A = OUT_OF_MEMORY

    for(int i = 0 ; i < WIDTH ; i++)
        strncpy(buffer + i * strlen(block), block, strlen(block));

    printf("\033[%d;%dm", color, bgcolor + 10);
    for(int H = 1 ; H <= HEIGHT ; H++)
        printf("\033[%d;%dH%s", H, 1, buffer);

    printf("\033[0m");
    free(buffer);
}

void draw_Line(int x, int y, int xSize, int ySize, int color, int bgcolor, char* block){
    printf("\033[%d;%dm", color, bgcolor + 10);
    char* buffer = malloc(xSize * strlen(block) + 1);

    if(xSize > 0 && ySize == 0){
        for(int i = 0 ; i < xSize ; i++)
            strncpy(buffer + i * strlen(block), block, strlen(block));
        buffer[xSize * strlen(block)] = '\0';
        printf("\033[%d;%dH%s", y, x, buffer);
        free(buffer);
    }   
    else if(ySize > 0 && xSize == 0)
        for(int i = 0 ; i < ySize ; i++)
            printf("\033[%d;%dH%s", y + i, x, block);
    printf("\033[0m");
}

void draw_Pixel(int x, int y, int color, int bgcolor, char* block){
    printf("\033[%d;%dm", color, bgcolor + 10);
    printf("\033[%d;%dH%s", y, x, block);
    printf("\033[0m");
}

static void bufferScreen(){
}

/*  Protótipo de uma implementação que desenharia todo a tela de uma vez
    usando só um buffer. A performance é a mesma da função que desenha
    linha por linha, então abandonei essa função.

void fillScreenFULL(int color, int bgcolor, const char *block){
    char* bufferX = malloc(WIDTH * strlen(block) + 1);
    char* bufferY = malloc(HEIGHT * strlen(bufferX));
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