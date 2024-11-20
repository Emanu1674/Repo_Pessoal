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

#define LARGURA 80
#define ALTURA 25


// Essa função obrigatoriamente precisa de comentário pra ser entendida.
//
// A função recebe um ponteiro para char chamado "caractere" que contém o
//    caractere que será usado para preencher a tela, exemplo "█".
// É então definido um ponteiro para char com tamanho alocado = [LARGURA]
//    (do terminal) * tamanho de caractere, chamado buffer.
// Conteúdo de caractere é então copiado para dentro de buffer [LARGURA]
//    vezes, criando um vetor de char que preenche uma linha inteira.
// Esse vetor é impresso na tela [ALTURA] vezes, para encher o terminal
//    linha por linha, ao invés de caractere por caractere (muito lento).
//

void preenche_Tela(int _FGColor, int _corFundo, char* _Char);
void d_Retangulo(int _xPos1, int _yPos1, int _xPos2, int _yPos2, int _FGColor, int _corFundo, char* _Char);
void d_retangulo_preenchido(int _xPos1, int _yPos1, int _xPos2, int _yPos2, int _FGColor, int _corFundo, char* _Char);
void d_Linha(int _xPos, int _yPos, int _xSize, int _ySize, int _FGColor, int _corFundo, char* _Char);
void d_Pixel(int _xPos, int _yPos, int _FGColor, int _corFundo, char* _Char);

int main(){
    setlocale(LC_ALL, "en_us.UTF-8");
    preenche_Tela(34, 34, u8"█");
    printf("\033[?25l");
    d_Linha(6, 4, 70, 0, 96, 34, u8"─");
    //printf("\033[3;6H\033[96;44mMicrosoft ScanDisk\033[0m");
    d_Linha(6, 23, 70, 0, 96, 34, u8"─");

    // Botão Pause
    d_Linha(6, 21, 9, 0, 90, 39, u8"█");
    d_Linha(7, 22, 9, 0, 30, 34, u8"▀");
    d_Pixel(15, 21, 30, 34, u8"▄");
    d_Pixel(6, 21, 97, 90, u8"◄");
    d_Pixel(14, 21, 97, 90, u8"►");

    // Botão More Info
    d_Linha(18, 21, 13, 0, 90, 39, u8"█");
    d_Linha(19, 22, 13, 0, 30, 34, u8"▀");
    d_Pixel(31, 21, 30, 34, u8"▄");
    d_Pixel(18, 21, 37, 90, u8"<");
    d_Pixel(30, 21, 37, 90, u8">");

    // Botão Exit
    d_Linha(34, 21, 8, 0, 90, 39, u8"█");
    d_Linha(35, 22, 8, 0, 30, 34, u8"▀");
    d_Pixel(42, 21, 30, 34, u8"▄");
    d_Pixel(34, 21, 37, 90, u8"<");
    d_Pixel(41, 21, 37, 90, u8">");

    // Barra de Progresso
    d_Linha(21, 24, 31, 0, 93, 93, u8"█");
    d_Linha(52, 24, 24, 0, 93, 34, u8"░");

    // Outras frescuras na tela
    d_Pixel(14, 8, 96, 34, u8"√");
    d_Pixel(14, 9, 96, 34, u8"√");
    d_Pixel(14, 10, 96, 34, u8"√");
    d_Pixel(14, 11, 96, 34, u8"»");

    getchar();
    system(CLEAR);
    return 0;
}

void d_Retangulo(int x1, int y1, int x2, int y2, int cor, int corFundo, char* caractere){
    int R_LARGURA = abs(x2 - x1) + 1;
    int R_ALTURA = abs(y2 - y1) + 1;

    char* buffer = malloc(R_LARGURA * strlen(caractere) + 1);

    for(int i = 0 ; i < R_LARGURA ; i++)
        strncpy(buffer + i * strlen(caractere), caractere, strlen(caractere));
    buffer[R_LARGURA * strlen(caractere)] = '\0';

    printf("\033[%d;%dm", cor, corFundo + 10);
    printf("\033[%d;%dH%s", y1, x1, buffer);
    for (int H = y1 + 1; H < y2; H++) {
        printf("\033[%d;%dH%s", H, x1, caractere); // Borda esquerda
        printf("\033[%d;%dH%s", H, x2, caractere); // Borda direita
    }

    printf("\033[%d;%dH%s", y2, x1, buffer);

    printf("\033[0m");
    free(buffer);
}

void d_retangulo_preenchido(int x1, int y1, int x2, int y2, int cor, int corFundo, char* caractere){
    int R_LARGURA = abs(x2 - x1) + 1;
    int R_ALTURA = abs(y2 - y1) + 1;
    char* buffer = malloc(R_LARGURA * strlen(caractere) + 1);

    for(int i = 0 ; i < R_LARGURA ; i++)
        strncpy(buffer + i * strlen(caractere), caractere, strlen(caractere));
    buffer[R_LARGURA * strlen(caractere)] = '\0';

    printf("\033[%d;%dm", cor, corFundo + 10);
    for(int H = y1 ; H <= y2 ; H++)
        printf("\033[%d;%dH%s", H, x1, buffer);

    printf("\033[0m");
    free(buffer);
}

void preenche_Tela(int cor, int corFundo, char* caractere){
    char* buffer = malloc(LARGURA * strlen(caractere));
    //if (buffer == NULL){ bugCheck(0x1A);}     // 0x1A = OUT_OF_MEMORY

    for(int i = 0 ; i < LARGURA ; i++)
        strncpy(buffer + i * strlen(caractere), caractere, strlen(caractere));

    printf("\033[%d;%dm", cor, corFundo + 10);
    for(int H = 1 ; H <= ALTURA ; H++)
        printf("\033[%d;%dH%s", H, 1, buffer);

    printf("\033[0m");
    free(buffer);
}

void d_Linha(int x, int y, int xSize, int ySize, int cor, int corFundo, char* caractere){
    printf("\033[%d;%dm", cor, corFundo + 10);
    char* buffer = malloc(xSize * strlen(caractere) + 1);

    if(xSize > 0 && ySize == 0){
        for(int i = 0 ; i < xSize ; i++)
            strncpy(buffer + i * strlen(caractere), caractere, strlen(caractere));
        buffer[xSize * strlen(caractere)] = '\0';
        printf("\033[%d;%dH%s", y, x, buffer);
        free(buffer);
    }   
    else if(ySize > 0 && xSize == 0)
        for(int i = 0 ; i < ySize ; i++)
            printf("\033[%d;%dH%s", y + i, x, caractere);
    printf("\033[0m");
}

void d_Pixel(int x, int y, int cor, int corFundo, char* caractere){
    printf("\033[%d;%dm", cor, corFundo + 10);
    printf("\033[%d;%dH%s", y, x, caractere);
    printf("\033[0m");
}

static void bufferScreen(){
}

/*  Protótipo de uma implementação que desenharia todo a tela de uma vez
    usando só um buffer. A performance é a mesma da função que desenha
    linha por linha, então abandonei essa função.

void fillScreenFULL(int cor, int corFundo, const char *caractere){
    char* bufferX = malloc(LARGURA * strlen(caractere) + 1);
    char* bufferY = malloc(ALTURA * strlen(bufferX));
    for(int i = 0 ; i < LARGURA ; i++)
        strncpy(bufferX + i * strlen(caractere), caractere, strlen(caractere));
    bufferX[LARGURA * strlen(caractere)] = '\0';
    for(int i = 0 ; i < ALTURA ; i++)
        strncpy(bufferY + i * strlen(bufferX), bufferX, strlen(bufferX));
    bufferY[ALTURA * strlen(bufferX)] = '\0';
    printf("\033[%d;%dm", cor, corFundo+10);
    printf("\033[1;1H%s", bufferY);
    printf("\033[0m");
    free(bufferX);
    free(bufferY);
    fflush(stdin);
}*/

/*  Função velha, preenche a tela um caractere pro vez
    Lenta pra caramba

void fillScreen_OLD(int cor, int corFundo, const char *caractere){
    printf("\033[%d;%dm", cor, corFundo+10);
    for(int H = 0 ; H <= ALTURA ; H++){
        for(int W = 0 ; W <= LARGURA ; W++)
            printf("\033[%d;%dH%s", H, W, caractere);
    }
}*/