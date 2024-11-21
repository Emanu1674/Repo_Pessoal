#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <locale.h>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

#define LARGURA 80
#define ALTURA 25
#define COL 18
#define LN 6

typedef unsigned char u8;
typedef signed char s8;
typedef struct{
    u8 hora, min, seg;
} horario;

typedef struct{
    int tabuleiro[5][5];
    short pecas;
    long tempoJogo;
} jogo;

void preenche_Tela(int _FGColor, int _corFundo, char* _Char);
void d_Retangulo(int _xPos1, int _yPos1, int _xPos2, int _yPos2, int _FGColor, int _corFundo, char* _Char);
void d_Retangulo_Preenchido(int _xPos1, int _yPos1, int _xPos2, int _yPos2, int _FGColor, int _corFundo, char* _Char);
void d_Linha(int _xPos, int _yPos, int _xSize, int _ySize, int _FGColor, int _corFundo, char* _Char);
void d_Pixel(int _xPos, int _yPos, int _FGColor, int _corFundo, char* _Char);

void desenhaMenu(){
d_Retangulo_Preenchido(62, 1, 79, 2, 35, 35, u8"█");
    d_Retangulo_Preenchido(62, 3, 79, 24, 94, 94, u8"█");
    d_Linha(80, 1, 0, 2, 35, 30, u8"░");
    d_Linha(62, 25, 19, 0, 94, 34, u8"░");
    d_Linha(80, 3, 0, 22, 94, 34, u8"░");
}

void desenhaTabuleiro(jogo *partida){
    d_Retangulo_Preenchido(16, 5, 47, 20, 96, 90, u8"░");
    for(int i = 0 ; i <= 4 ; i++){
        for(int j = 0 ; j <= 4 ; j++){
            if(partida->tabuleiro[j][i] == 0)
                d_Retangulo(COL + (i*6), LN + (j*3), COL+3 + (i*6), LN+1 + (j*3), 30, 30, u8"█");
            else if(partida->tabuleiro[j][i] == 1)
                d_Retangulo(COL + (i*6), LN + (j*3), COL+3 + (i*6), LN+1 + (j*3), 34, 30, u8"█");
            else if(partida->tabuleiro[j][i] == 2)
                d_Retangulo(COL + (i*6), LN + (j*3), COL+3 + (i*6), LN+1 + (j*3), 31, 30, u8"█");
        }
    }

}

int moverCursor(u8* rX, u8* rY, jogo *partida){
    u8 tabuleiro[5][5];
    s8 Y = 0;
    s8 X = 0;
    u8 Cor = 0;
    int tecla = 0;
    while(tecla != 13){     // 13 = Enter

        if(partida->tabuleiro[Y][X] == 0)
            Cor = 30;
        else if(partida->tabuleiro[Y][X] == 1)
            Cor = 34;
        else if(partida->tabuleiro[Y][X] == 2)
            Cor = 31;

        d_Retangulo_Preenchido(COL + X*6, LN + Y*3, COL+3 + X*6, LN+1 + Y*3, Cor+60, Cor+60, u8"█");
        tecla = getch();
        if(tecla == 0 || tecla == 224) {
            tecla = getch();
            switch (tecla) {
                case 72:
                    d_Retangulo_Preenchido(COL + X*6, LN + Y*3, COL+3 + X*6, LN+1 + Y*3, Cor, Cor, u8"█");
                    Y -= 1;
                    if(Y < 0)
                        Y = 4;
                break;
                case 80:
                    d_Retangulo_Preenchido(COL + X*6, LN + Y*3, COL+3 + X*6, LN+1 + Y*3, Cor, Cor, u8"█");
                    Y += 1;
                    if(Y > 4)
                        Y = 0;
                break;
                case 75:
                    d_Retangulo_Preenchido(COL + X*6, LN + Y*3, COL+3 + X*6, LN+1 + Y*3, Cor, Cor, u8"█");
                    X -= 1;
                    if(X < 0)
                        X = 4;
                break;
                case 77:
                    d_Retangulo_Preenchido(COL + X*6, LN + Y*3, COL+3 + X*6, LN+1 + Y*3, Cor, Cor, u8"█");
                    X += 1;
                    if(X > 4)
                        X = 0;    
                break;
                default: continue;
            }
        }
    }
    *rX = X;
    *rY = Y;
}

void loopJogo(jogo *partida){
    u8 X, Y;

    // Desenha Menu
    desenhaMenu();

    // Desenha Tabuleiro
    desenhaTabuleiro(partida);

    printf("\033[3;27fJogador 1");
    d_Linha(37, 3, 2, 0, 31, 31, u8"█");
    printf("\033[0m");

    // Loop de mover cursor
    moverCursor(&X, &Y, partida);
}

int main(){
    setlocale(LC_ALL, "en_us.UTF-8");
    printf("\033[?25l");

    // Inicializa Struct
    jogo partida = {
        .tabuleiro = {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}
        },
        .pecas = 0,
        .tempoJogo = 0
    };

    loopJogo(&partida);

    //getchar();
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

void d_Retangulo_Preenchido(int x1, int y1, int x2, int y2, int cor, int corFundo, char* caractere){
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