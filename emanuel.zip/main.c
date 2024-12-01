#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// conio.h não existe em sistemas Unix. A única função utulizada desta
// biblioteca é getch(). Existe uma implementação de getch na biblioteda
// <ncurses.h> que pode ser usada em Linux. Mais detalhes no LEIA-ME.
#include <conio.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

#define LARGURA 90
#define ALTURA 25
#define COR_JGDR_1 31
#define COR_JGDR_2 34
#define TAM_TABULEIRO 5
#define LN 6
#define COL 26



typedef struct{
    int hora, min, seg;
} horario;

typedef struct{
    int tabuleiro[5][5];
    short pecasTotal;
    short turnoIndice;
    int pecasJogador1;
    int pecasJogador2;
    int jogador;
    long tempoJogo;
    time_t tempoInicial;
    double tempoDecorrido;
    bool isPausado;
    int modo;
} jogo;

typedef struct {
    short jogador;
    char origem[3];
    char destino[3];
    char capturas[3][3];
    short numCapturas;
    bool turnoExtra;
} turno;

void d_preenche_Tela(int _Cor, int _corFundo, char* _Char);
void d_Retangulo(int _yPos1, int _xPos1, int _yPos2, int _xPos2, int _Cor, int _corFundo, char* _Char);
void d_Retangulo_Preenchido(int _yPos1, int _xPos1, int _yPos2, int _xPos2, int _Cor, int _corFundo, char* _Char);
void d_Linha(int _yPos, int _xPos, int _yTamanho, int _xTamanho, int _Cor, int _corFundo, char* _Char);
void d_Pixel(int _yPos, int _xPos, int _Cor, int _corFundo, char* _Char);

void desenhaMenuPrincipal();
void desenhaMenuLateral();
void desenhaHistorico();
void desenhaTabuleiro(jogo *_partida);
void desenhaPosicao(int _Y, int _X, int _Cor, int _corFundo);
void desenhaPosicoes(jogo* _partida);

void pausarJogo(jogo* _partida);
void salvarJogo(jogo* _partida, int _modo);
void carregarJogo(jogo* _partida, int modo);

void menuPrincipal(jogo* _partida);
void menuNovoJogo();
void menuCarregar(jogo* _partida);
void novoJogoInicializa(int _modo);
void fase1(jogo* _partida);
void fase2(jogo* _partida);
void fase1CPU(jogo* _partida);
void fase2CPU(jogo* _partida);
void CPUCriaPeca(int* _pY, int* _pX, jogo* partida);
bool dentroDoLimite(int _Y, int _X);
void avaliacao_destaque(int* _Y, int* _X, jogo* _partida, int _destaque[5][5]);
void avaliacao_desenha(int* _Y, int* _X, jogo* _partida);
void avaliacao_reseta(int* _Y, int* _X, jogo* _partida);
int selecionaPeca(int* _rY, int* _rX, jogo* _partida, int _modo);
int entradaTeclado(int* _rY, int* _rX, jogo* _partida, int _modo);
bool posicaoValida(int _Y, int _X, jogo* _partida);

void desenhaMenuLateral(){
d_Retangulo_Preenchido(1, 74, 2, 89, 35, 35, "█");
    d_Retangulo_Preenchido(3, 74, 24, 89, 94, 94, "█");
    d_Linha(1, 90, 2, 0, 35, 30, "░");
    d_Linha(25, 74, 0, 17, 94, 34, "░");
    d_Linha(3, 90, 22, 0, 94, 34, "░");
    d_Linha(25, 1, 0, 73, 90, 39, "░");
    d_Linha(25, 31, 0, 22, 96, 39, "█");

    printf("\033[4;75H\033[30;104mS\033[97;104malvar Jogo");
    printf("\033[5;75H\033[30;104mC\033[97;104marregar Jogo");
    printf("\033[6;75H\033[30;104mP\033[97;104mausar Jogo");
    printf("\033[9;75HVoltar ao \033[30;104mM\033[97;104menu");
    d_Linha(10, 75, 0, 14, 97, 94, "─");
    printf("\033[11;75H\033[97;104mSair do ");
    printf("\033[11;83H\033[30;104mJ\033[97;104mogo");

    printf("\033[0m");
}

void desenhaTabuleiro(jogo* partida){
    d_Pixel(7, 25, 97, 39, "A");
    d_Pixel(10, 25, 97, 39, "B");
    d_Pixel(13, 25, 97, 39, "C"); 
    d_Pixel(16, 25, 97, 39, "D");
    d_Pixel(19, 25, 97, 39, "E");
    d_Retangulo_Preenchido(6, 26, 21, 57, 96, 30, "░");
    desenhaPosicoes(partida);
    d_Pixel(5, 29, 97, 39, "1");
    d_Pixel(5, 35, 97, 39, "2");
    d_Pixel(5, 41, 97, 39, "3");
    d_Pixel(5, 47, 97, 39, "4");
    d_Pixel(5, 53, 97, 39, "5");
}

void desenhaPosicoes(jogo* partida){
    for(int i = 0 ; i <= 4 ; i++){
        for(int j = 0 ; j <= 4 ; j++){
            if(partida->tabuleiro[i][j] == 0)
                d_Retangulo(LN+1 + (i*3), COL+2 + (j*6), LN+2 + (i*3), COL+5 + (j*6), 30, 30, "█");
            else if(partida->tabuleiro[i][j] == 1)
                d_Retangulo(LN+1 + (i*3), COL+2 + (j*6), LN+2 + (i*3), COL+5 + (j*6), COR_JGDR_1, 30, "█");
            else if(partida->tabuleiro[i][j] == 2)
                d_Retangulo(LN+1 + (i*3), COL+2 + (j*6), LN+2 + (i*3), COL+5 + (j*6), COR_JGDR_2, 30, "█");
        }
    }
}

void desenhaPosicao(int Y, int X, int Cor, int corFundo){
    d_Retangulo_Preenchido(LN+1 + (Y*3), COL+2 + (X*6), LN+2 + (Y*3), COL+5 + (X*6), Cor, corFundo+10, "█");
}

int selecionaPeca(int* rY, int* rX, jogo* partida, int modo){
    int Y = *rY;
    int X = *rX;
    int cor = 0;
    int tecla = 0;

    while(tecla != 13){
        if(partida->tabuleiro[Y][X] == 0)
            cor = 30;
        else if(partida->tabuleiro[Y][X] == 2)
            cor = COR_JGDR_2;
        else if(partida->tabuleiro[Y][X] == 1)
            cor = COR_JGDR_1;

        d_Retangulo_Preenchido(LN+1 + Y*3, COL+2 + X*6, LN+2 + Y*3, COL+5 + X*6, cor+60, cor+60, "█");

        tecla = getch();
        if(tecla == 0 || tecla == 224) {
            tecla = getch();
            switch (tecla) {
                case 72:
                    d_Retangulo_Preenchido(LN+1 + Y*3, COL+2 + X*6, LN+2 + Y*3, COL+5 + X*6, cor, cor, "█");
                    avaliacao_reseta(&Y, &X, partida);
                    Y -= 1;
                    if(Y < 0)
                        Y = 4;
                    avaliacao_desenha(&Y, &X, partida);
                    break;
                case 80:
                    d_Retangulo_Preenchido(LN+1 + Y*3, COL+2 + X*6, LN+2 + Y*3, COL+5 + X*6, cor, cor, "█");
                    avaliacao_reseta(&Y, &X, partida);                                        
                    Y += 1;
                    if(Y > 4)
                        Y = 0;
                    avaliacao_desenha(&Y, &X, partida);
                    break;
                case 75:
                    d_Retangulo_Preenchido(LN+1 + Y*3, COL+2 + X*6, LN+2 + Y*3, COL+5 + X*6, cor, cor, "█");
                    avaliacao_reseta(&Y, &X, partida); 
                    X -= 1;
                    if(X < 0)
                        X = 4;
                    avaliacao_desenha(&Y, &X, partida);
                    break;
                case 77:
                    d_Retangulo_Preenchido(LN+1 + Y*3, COL+2 + X*6, LN+2 + Y*3, COL+5 + X*6, cor, cor, "█");
                    avaliacao_reseta(&Y, &X, partida);
                    X += 1;
                    if(X > 4)
                        X = 0;    
                    avaliacao_desenha(&Y, &X, partida);
                    break;
                default:
                    continue;
            } 
        } else {
            switch (tecla)
            {
            case 83:
            case 115:
                salvarJogo(partida, modo);
                break;
            case 67:
            case 99:
                carregarJogo(partida, partida->modo);
                break;
            case 80:
            case 112:
                pausarJogo(partida);
                break;
            case 77:
            case 109:
                printf("\033[0m");
                system(CLEAR);
                desenhaMenuPrincipal();
                menuPrincipal(partida);
                break;
            case 74:
            case 106:
                system(CLEAR);
                printf("\033[?25h\033[h\033[0m");
                printf("Obrigado por jogar!");
                exit(0);
                break;
            default:
                continue;
            }
        }
    }
    
    d_Retangulo_Preenchido(LN+1 + Y*3, COL+2 + X*6, LN+2 + Y*3, COL+5 + X*6, cor, cor, "█");

    *rY = Y;
    *rX = X;
}

int selecionaPecaPosicao(int* rY, int* rX, jogo* partida, int destaque[5][5]){
    int Y = *rY;
    int X = *rX;
    int cor = 0;
    int tecla = 0;

        while(tecla != 13){
            if (destaque[Y][X] == 3) {
                cor = 35;
            } else if (destaque[Y][X] == 2) {
                cor = 32;
            } else if (destaque[Y][X] == 1) {
                cor = 33;
            } else if (partida->tabuleiro[Y][X] == 0) {
                cor = 30;
            } else if (partida->tabuleiro[Y][X] == 2) {
                cor = COR_JGDR_2;
            } else if (partida->tabuleiro[Y][X] == 1) {
                cor = COR_JGDR_1;
            }

            d_Retangulo_Preenchido(LN+1 + Y*3, COL+2 + X*6, LN+2 + Y*3, COL+5 + X*6, cor+60, cor+60, "█");

            tecla = getch();
            if (tecla == 0 || tecla == 224) {
                tecla = getch();
                int prevY = Y, prevX = X;

                switch (tecla) {
                    case 72: Y = (Y - 1 + 5) % 5; break;
                    case 80: Y = (Y + 1) % 5; break;
                    case 75: X = (X - 1 + 5) % 5; break;
                    case 77: X = (X + 1) % 5; break;
                }

                int prevCor;
                if (destaque[prevY][prevX] == 3) {
                    prevCor = 35;
                } else if (destaque[prevY][prevX] == 2) {
                    prevCor = 32;
                } else if (destaque[prevY][prevX] == 1) {
                    prevCor = 33;
                } else if (partida->tabuleiro[prevY][prevX] == 0) {
                    prevCor = 30;
                } else if (partida->tabuleiro[prevY][prevX] == 2) {
                    prevCor = COR_JGDR_2;
                } else if (partida->tabuleiro[prevY][prevX] == 1) {
                    prevCor = COR_JGDR_1;
                }

                d_Retangulo_Preenchido(LN+1 + prevY*3, COL+2 + prevX*6, LN+2 + prevY*3, COL+5 + prevX*6, prevCor, prevCor, "█");
            }
        }
    *rY = Y;
    *rX = X;
}

int entradaTeclado(int* rY, int* rX, jogo* partida, int modo){
    int Y = *rY;
    int X = *rX;
    int cor = 0;
    int tecla = 0;

    while(tecla != 13){
        if(partida->tabuleiro[Y][X] == 0)
            cor = 30;
        else if(partida->tabuleiro[Y][X] == 2)
            cor = COR_JGDR_2;
        else if(partida->tabuleiro[Y][X] == 1)
            cor = COR_JGDR_1;

        d_Retangulo_Preenchido(LN+1 + Y*3, COL+2 + X*6, LN+2 + Y*3, COL+5 + X*6, cor+60, cor+60, "█");

        tecla = getch();
        if(tecla == 0 || tecla == 224) {
            tecla = getch();
            switch (tecla) {
                case 72:
                    d_Retangulo_Preenchido(LN+1 + Y*3, COL+2 + X*6, LN+2 + Y*3, COL+5 + X*6, cor, cor, "█");
                    Y -= 1;
                    if(Y < 0)
                        Y = 4;
                    break;
                case 80:
                    d_Retangulo_Preenchido(LN+1 + Y*3, COL+2 + X*6, LN+2 + Y*3, COL+5 + X*6, cor, cor, "█");
                    Y += 1;
                    if(Y > 4)
                        Y = 0;
                    break;
                case 75:
                    d_Retangulo_Preenchido(LN+1 + Y*3, COL+2 + X*6, LN+2 + Y*3, COL+5 + X*6, cor, cor, "█");
                    X -= 1;
                    if(X < 0)
                        X = 4;
                    break;
                case 77:
                    d_Retangulo_Preenchido(LN+1 + Y*3, COL+2 + X*6, LN+2 + Y*3, COL+5 + X*6, cor, cor, "█");
                    X += 1;
                    if(X > 4)
                        X = 0;    
                    break;
                default:
                    continue;
            }
        } else {
            switch (tecla)
            {
            case 83:
            case 115:
                salvarJogo(partida, modo);
                break;
            case 67:
            case 99:
                carregarJogo(partida, partida->modo);
                break;
            case 80:
            case 112:
                pausarJogo(partida);
                break;
            case 77:
            case 109:
                printf("\033[0m");
                system(CLEAR);
                desenhaMenuPrincipal();
                menuPrincipal(partida);
                break;
            case 74:
            case 106:
                system(CLEAR);
                printf("\033[?25h\033[h\033[0m");
                printf("Obrigado por jogar!");
                exit(0);
                break;
            default:
                continue;
            }
        }
    }

    d_Retangulo_Preenchido(LN+1 + Y*3, COL+2 + X*6, LN+2 + Y*3, COL+5 + X*6, cor, cor, "█");

    *rY = Y;
    *rX = X;
}

void CPUCriaPeca(int* pY, int* pX, jogo* partida) {
    int Y, X;
    int attempts = 0;
    const int maxAttempts = 25;

    while (attempts < maxAttempts) {
        Y = (rand() % 5);
        X = (rand() % 5);

        if (partida->tabuleiro[Y][X] == 0 && !(Y == 2 && X == 2)) {
            *pY = Y;
            *pX = X;
            return;
        }
        attempts++;
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (partida->tabuleiro[i][j] == 0) {
                *pY = i;
                *pX = j;
                return;
            }
        }
    }
}

bool dentroDoLimite(int Y, int X) {
    return Y >= 0 && Y < TAM_TABULEIRO && X >= 0 && X < TAM_TABULEIRO;
}

void avaliacao_destaque(int* Y, int* X, jogo* partida, int destaque[5][5]){
    int direcoes[4][2] = {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };
    int adversario = (partida->jogador == 1) ? 2 : 1;

    if (partida->tabuleiro[*Y][*X] == partida->jogador) {
        destaque[*Y][*X] = 1;
        for (int i = 0; i < 4; i++) {
            int dy = direcoes[i][0];
            int dx = direcoes[i][1];    

            if (dentroDoLimite(*Y + dy, *X + dx) &&
                partida->tabuleiro[*Y + dy][*X + dx] == 0) {
                destaque[*Y + dy][*X + dx] = 2;
                int verdeX = *X + dx;
                int verdeY = *Y + dy;   

                for (int j = 0; j < 4; j++){
                    int dy1 = direcoes[j][0];
                    int dx1 = direcoes[j][1];
                    int dy2 = 2 * direcoes[j][0];
                    int dx2 = 2 * direcoes[j][1];   

                    if (dentroDoLimite(verdeY + dy1, verdeX + dx1) &&
                        dentroDoLimite(verdeY + dy2, verdeX + dx2) &&
                        partida->tabuleiro[verdeY + dy1][verdeX + dx1] == adversario &&
                        partida->tabuleiro[verdeY + dy2][verdeX + dx2] == partida->jogador &&
                        (verdeY + dy1 != 2 || verdeX + dx1 != 2)) {
                            destaque[verdeY + dy1][verdeX + dx1] = 3;
                    }
                }
            }
        }
    }
}

void avaliacao_desenha(int* Y, int* X, jogo* partida) {
    int direcoes[4][2] = {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };
    int adversario = (partida->jogador == 1) ? 2 : 1;

    if (partida->tabuleiro[*Y][*X] == partida->jogador) {
        for (int i = 0; i < 4; i++) {
            int dy = direcoes[i][0];
            int dx = direcoes[i][1];

            if (dentroDoLimite(*Y + dy, *X + dx) &&
                partida->tabuleiro[*Y + dy][*X + dx] == 0) {

                desenhaPosicao(*Y + dy, *X + dx, 32, 32);
                int verdeX = *X + dx;
                int verdeY = *Y + dy;

                for (int j = 0; j < 4; j++){
                    int dy1 = direcoes[j][0];
                    int dx1 = direcoes[j][1];
                    int dy2 = 2 * direcoes[j][0];
                    int dx2 = 2 * direcoes[j][1];   

                    if (dentroDoLimite(verdeY + dy1, verdeX + dx1) &&
                        dentroDoLimite(verdeY + dy2, verdeX + dx2) &&
                        partida->tabuleiro[verdeY + dy1][verdeX + dx1] == adversario &&
                        partida->tabuleiro[verdeY + dy2][verdeX + dx2] == partida->jogador &&
                        (verdeY + dy1 != 2 || verdeX + dx1 != 2)) {
                        desenhaPosicao(verdeY + dy1, verdeX + dx1, 35, 35);
                    }
                }
            }
        }
    }
}

bool avaliacao_silenciosa_movimento(int Y, int X, jogo* partida){
    int adversario = (partida->jogador == 1) ? 2 : 1;
    int direcoes[4][2] = {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };

    if (partida->tabuleiro[Y][X] == partida->jogador) {
        for (int i = 0; i < 4; i++) {
            int dy = direcoes[i][0];
            int dx = direcoes[i][1];    

            if (dentroDoLimite(Y + dy, X + dx) &&
                partida->tabuleiro[Y + dy][X + dx] == 0)
                    return true;
        }
    }
    return false;
}

bool avaliacao_silenciosa(int* Y, int* X, jogo* partida){
    int adversario = (partida->jogador == 1) ? 2 : 1;
    int direcoes[4][2] = {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };

    if (partida->tabuleiro[*Y][*X] == partida->jogador) {
        for (int i = 0; i < 4; i++) {
            int dy = direcoes[i][0];
            int dx = direcoes[i][1];    

            if (dentroDoLimite(*Y + dy, *X + dx) &&
                partida->tabuleiro[*Y + dy][*X + dx] == 0) {
                int verdeX = *X + dx;
                int verdeY = *Y + dy;

                for (int j = 0; j < 4; j++){
                    int dy1 = direcoes[j][0];
                    int dx1 = direcoes[j][1];
                    int dy2 = 2 * direcoes[j][0];
                    int dx2 = 2 * direcoes[j][1];   

                    if (dentroDoLimite(verdeY + dy1, verdeX + dx1) &&
                        dentroDoLimite(verdeY + dy2, verdeX + dx2) &&
                        partida->tabuleiro[verdeY + dy1][verdeX + dx1] == adversario &&
                        partida->tabuleiro[verdeY + dy2][verdeX + dx2] == partida->jogador &&
                        (verdeY + dy1 != 2 || verdeX + dx1 != 2)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool avaliacao_silenciosa_captura(int Y, int X, jogo* partida){
    int adversario = (partida->jogador == 1) ? 2 : 1;
    int direcoes[4][2] = {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };
    for (int i = 0 ; i < 4 ; i++){
        int dy = direcoes[i][0];
        int dx = direcoes[i][1];

        for (int j = 0; j < 4; j++){
            int dy1 = direcoes[j][0]; 
            int dx1 = direcoes[j][1];
            int dy2 = 2 * direcoes[j][0];
            int dx2 = 2 * direcoes[j][1];

            if (dentroDoLimite(Y + dy1, X + dx1) &&
                dentroDoLimite(Y + dy2, X + dx2)){

                if (partida->tabuleiro[Y + dy1][X + dx1] == adversario &&
                    partida->tabuleiro[Y + dy2][X + dx2] == partida->jogador &&
                    !(Y + dy1 == 2 && X + dx1 == 2)){
                    return true;
                }
            }
        }
    }
    return false;
}

void avaliacao_reseta(int* Y, int* X, jogo* partida){
    int corCapturaOrig;
    int adversario;
    
    corCapturaOrig = partida->jogador == 1 ? 34 : 31;
    adversario = partida->jogador == 1 ? 2 : 1;

    if(partida->tabuleiro[*Y-1][*X] == 0 && (*Y-1 >= 0)){
        desenhaPosicao(*Y-1, *X, 30, 30);
        if(partida->tabuleiro[*Y-1][*X-2] == partida->jogador && partida->tabuleiro[*Y-1][*X-1] == adversario){
        if(dentroDoLimite(*Y-1, *X-1))
            desenhaPosicao(*Y-1, *X-1, corCapturaOrig, corCapturaOrig);
        }
        if(partida->tabuleiro[*Y-3][*X] == partida->jogador && partida->tabuleiro[*Y-2][*X] == adversario){
        if(dentroDoLimite(*Y-1, *X))
            desenhaPosicao(*Y-2, *X, corCapturaOrig, corCapturaOrig);
        }
        if(partida->tabuleiro[*Y-1][*X+2] == partida->jogador && partida->tabuleiro[*Y-1][*X+1] == adversario){
        if(dentroDoLimite(*Y-1, *X+1))
            desenhaPosicao(*Y-1, *X+1, corCapturaOrig, corCapturaOrig);
        }
    }
    if(partida->tabuleiro[*Y+1][*X] == 0 && (*Y+1 <= 4)){
        desenhaPosicao(*Y+1, *X, 30, 30);
        if(partida->tabuleiro[*Y+1][*X+2] == partida->jogador && partida->tabuleiro[*Y+1][*X+1] == adversario){
        if(dentroDoLimite(*Y+1, *X+1))
             desenhaPosicao(*Y+1, *X+1, corCapturaOrig, corCapturaOrig);
        }
        if(partida->tabuleiro[*Y+3][*X] == partida->jogador && partida->tabuleiro[*Y+2][*X] == adversario){
        if(dentroDoLimite(*Y+2, *X))
            desenhaPosicao(*Y+2, *X, corCapturaOrig, corCapturaOrig);
        }
        if(partida->tabuleiro[*Y+1][*X-2] == partida->jogador && partida->tabuleiro[*Y+1][*X-1] == adversario){
        if(dentroDoLimite(*Y+1, *X-1))
            desenhaPosicao(*Y+1, *X-1, corCapturaOrig, corCapturaOrig);
        }
    }
    if(partida->tabuleiro[*Y][*X-1] == 0 && (*X-1 >= 0)){
        desenhaPosicao(*Y, *X-1, 30, 30);
        if(partida->tabuleiro[*Y+2][*X-1] == partida->jogador && partida->tabuleiro[*Y+1][*X-1] == adversario){
        if(dentroDoLimite(*Y+1, *X-1))
            desenhaPosicao(*Y+1, *X-1, corCapturaOrig, corCapturaOrig);
        }
        if(partida->tabuleiro[*Y][*X-3] == partida->jogador && partida->tabuleiro[*Y][*X-2] == adversario){
        if(dentroDoLimite(*Y, *X-2))
            desenhaPosicao(*Y, *X-2, corCapturaOrig, corCapturaOrig);
        }
        if(partida->tabuleiro[*Y-2][*X-1] == partida->jogador && partida->tabuleiro[*Y-1][*X-1] == adversario){
        if(dentroDoLimite(*Y-1, *X-1))
            desenhaPosicao(*Y-1, *X-1, corCapturaOrig, corCapturaOrig);
        }
    }
    if(partida->tabuleiro[*Y][*X+1] == 0 && (*X+1 <= 4)){
        desenhaPosicao(*Y, *X+1, 30, 30);
        if(partida->tabuleiro[*Y+2][*X+1] == partida->jogador && partida->tabuleiro[*Y+1][*X+1] == adversario){
        if(dentroDoLimite(*Y+1, *X+1))
            desenhaPosicao(*Y+1, *X+1, corCapturaOrig, corCapturaOrig);
        }
        if(partida->tabuleiro[*Y][*X+3] == partida->jogador && partida->tabuleiro[*Y][*X+2] == adversario){
        if(dentroDoLimite(*Y, *X+2))
            desenhaPosicao(*Y, *X+2, corCapturaOrig, corCapturaOrig);
        }
        if(partida->tabuleiro[*Y-2][*X+1] == partida->jogador && partida->tabuleiro[*Y-1][*X+1] == adversario){
        if(dentroDoLimite(*Y-1, *X+1))
            desenhaPosicao(*Y-1, *X+1, corCapturaOrig, corCapturaOrig);
        }
    }
}

bool avaliacao_trancado(jogo* partida){
    for (int i = 0 ; i < TAM_TABULEIRO ; i++)
        for (int j = 0 ; j < TAM_TABULEIRO ; j++)
            if (partida->tabuleiro[i][j] == partida->jogador && avaliacao_silenciosa_movimento(i, j, partida))
                return false;
    return true;
}

void fimDeJogo(int modo, short vencedor, jogo* partida){
    bool botao = 0;
    int tecla = 0;
    short pixel = 0;

    if (partida->tabuleiro[1][4] == 0)
        pixel = 30;
    else if (partida->tabuleiro[1][4] == 1)
        pixel = 31;
    else if (partida->tabuleiro[1][4] == 2)
        pixel = 34;

    d_Linha(9, 28, 0, 28, 35, 35, "█");
    d_Retangulo_Preenchido(10, 28, 15, 55, 37, 35, "█");
    d_Linha(10, 56, 7, 0, 90, 30, "░");
    d_Linha(16, 28, 0, 28, 90, 30, "░");

    for(int i = 0 ; i < 5 ; i++){
        if (partida->tabuleiro[3][i] == 0)
            d_Linha(16, 28+(i*6), 0, 4, 30, 30, "█");
        else if (partida->tabuleiro[3][i] == 1)
            d_Linha(16, 28+(i*6), 0, 4, 31, 30, "░");
        else if (partida->tabuleiro[3][i] == 2)
            d_Linha(16, 28+(i*6), 0, 4, 34, 30, "░");
    }
    

    printf("\033[9;36H\033[97;45mFim de Jogo!");
    if (vencedor == 3)
        printf("\033[11;30H\033[30;47mO jogo terminou empatado");
    else
        printf("\033[11;30H\033[30;47mJogador %d venceu o jogo!", vencedor);

    printf("\033[12;34H\033[30;47mJogar novamente?");
    do {
        if(botao == 0){
            printf("\033[14;35H\033[30;107m Sim ");
            printf("\033[14;44H\033[97;100m Não ");
        } else {
            printf("\033[14;35H\033[97;100m Sim ");
            printf("\033[14;44H\033[30;107m Não ");
        }
        tecla = getch();
        if (tecla == 75 && botao > 0) {
            botao--;
        } else if (tecla == 77 && botao < 1) {
            botao++;
        }
    } while (tecla != 13);
    system(CLEAR);
    if(botao){
        printf("\033[0m");
        system(CLEAR);
        desenhaMenuPrincipal();
        menuPrincipal(partida);
    } else {
        printf("\033[0m");
        system(CLEAR);
        novoJogoInicializa(modo);
    }
}

void pausarJogo(jogo* partida){
    short pixel = 0;
    time_t tempoAtual = time(NULL);
    horario tempo = {0, 0, 0};

    if (partida->tabuleiro[1][4] == 0)
        pixel = 30;
    else if (partida->tabuleiro[1][4] == 1)
        pixel = 31;
    else if (partida->tabuleiro[1][4] == 2)
        pixel = 34;

    partida->tempoDecorrido += difftime(tempoAtual, partida->tempoInicial);
    partida->tempoInicial = tempoAtual;
    partida->isPausado = true;

    tempo.hora = (int)(partida->tempoDecorrido / (60 * 60));
    tempo.min = (int)(partida->tempoDecorrido / 60);
    tempo.seg = (int)(partida->tempoDecorrido) % 60;

    printf("\033[0m\033[2;4HRelógio: %02d:%02d:%02d", tempo.hora, tempo.min, tempo.seg);
    d_Linha(1, 4, 0, 17, 97, 39, "_");
    d_Linha(10, 30, 0, 24, 35, 35, "█");
    d_Retangulo_Preenchido(11, 30, 14, 53, 37, 37, "█");
    d_Linha(15, 30, 0, 25, 90, 30, "░");
    d_Linha(11, 54, 4, 0, 30, 30, "█");
    d_Pixel(12, 54, 90, 30, "░");
    d_Pixel(10, 54, pixel, 30, "▀");

    puts("\033[10;35H\033[97;45mJogo  Pausado!");
    puts("\033[12;33H\033[30;47mPessione qualquer");
    puts("\033[13;32H\033[30;47mtecla para continuar");
    
    getch();

    partida->isPausado = false;
    partida->tempoInicial = time(NULL);
    desenhaPosicoes(partida);
    d_Linha(1, 4, 0, 17, 39, 39, " ");
    d_Linha(2, 4, 0, 17, 39, 39, " ");
    d_Linha(12, 30, 0, 25, 96, 30, "░");
    d_Linha(15, 30, 0, 25, 96, 30, "░");
    d_Linha(10, 32, 6, 0, 96, 30, "░");
    d_Linha(10, 33, 6, 0, 96, 30, "░");
    d_Linha(10, 38, 6, 0, 96, 30, "░");
    d_Linha(10, 39, 6, 0, 96, 30, "░");
    d_Linha(10, 44, 6, 0, 96, 30, "░");
    d_Linha(10, 45, 6, 0, 96, 30, "░");
    d_Linha(10, 50, 6, 0, 96, 30, "░");
    d_Linha(10, 51, 6, 0, 96, 30, "░");
}

void salvarJogo(jogo* partida, int modo){
    if(partida->modo == 3){
        printf("\033[23;26HNão é possível salvar nesta etapa!");
        return;
    }

    FILE *arquivo;
    const char *nomeArquivo = partida->modo ? "SavePvP.bin" : "SaveCPU.bin";

    printf("\033[0m");
    arquivo = fopen(nomeArquivo, "wb");
    if (!arquivo) {
        perror("\033[23;31HErro ao abrir o arquivo!");
        return;
    }

    if (fwrite(partida, sizeof(jogo), 1, arquivo) != 1) {
        perror("\033[23;31HNão foi possível salvar!");
    } else {
        printf("\033[23;24HJogo salvo com sucesso em %s.", nomeArquivo);
    }

    fclose(arquivo);
}

void carregarJogo(jogo* partida, int modo){
    if(modo == 3){
        printf("\033[23;26HNão é possível carregar nesta etapa!");
        return;
    }
    FILE *arquivo;
    const char *arquivoModo = modo ? "savePvP.bin" : "saveCPU.bin";

    printf("\033[0m");

    arquivo = fopen(arquivoModo, "rb");
    if (!arquivo) {
        printf("\033[23;31HErro ao abrir o arquivo %s!\n", arquivoModo);
        perror("\033[24;31HDetalhes do erro");
        return;
    }

    if (fread(partida, sizeof(jogo), 1, arquivo) != 1) {
        printf("\033[23;31HErro ao carregar o jogo!\n");
        perror("\033[24;31HDetalhes do erro");
        fclose(arquivo);
        return;
    }

    if (partida->turnoIndice < 0 || partida->turnoIndice > 1) {
        printf("\033[23;31HEstado do jogo inválido! Não foi possível carregar.\n");
        fclose(arquivo);
        return;
    }

    printf("\033[23;24HJogo carregado com sucesso de %s.\n", arquivoModo);

    fclose(arquivo);

    if(partida->modo)
        fase2(partida);
    else
        fase2CPU(partida);
}

void removePeca(int Y, int X, jogo* partida, bool* pecaCapturada){
    short cont = 0;
    int adversario = (partida->jogador == 1) ? 2 : 1;
    int direcoes[4][2] = {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };
    for (int i = 0 ; i < 4 ; i++){
        int dy = direcoes[i][0];
        int dx = direcoes[i][1];

        for (int j = 0; j < 4; j++){
            int dy1 = direcoes[j][0]; 
            int dx1 = direcoes[j][1];
            int dy2 = 2 * direcoes[j][0];
            int dx2 = 2 * direcoes[j][1];
            if (dentroDoLimite(Y + dy1, X + dx1) &&
                dentroDoLimite(Y + dy2, X + dx2)){
                if (partida->tabuleiro[Y + dy1][X + dx1] == adversario &&
                    partida->tabuleiro[Y + dy2][X + dx2] == partida->jogador &&
                    !(Y + dy1 == 2 && X + dx1 == 2)){

                    partida->tabuleiro[Y + dy1][X + dx1] = 0;

                    if (partida->jogador == 1)
                        partida->pecasJogador2 -= 1;
                    else if (partida->jogador == 2)
                        partida->pecasJogador1 -= 1;

                    *pecaCapturada = 1;
                    desenhaPosicao(Y + dy1, X + dx1, 30, 30);
                }
            }
        }
    }
}

bool verifica_barreira(jogo* partida, int y, int x, bool direcao) {
    short adversario = (partida->jogador == 1) ? 2 : 1;
    int contJogador = 0;
    int contAdversario = 0;
    if (direcao){
        for (int Y = y+1  ; Y < 5 ; Y++){
            for (int X = 0 ; X < 5 ; X++){
                if(partida->tabuleiro[Y][X] == adversario){
                    contAdversario++;
                }
                if(partida->tabuleiro[Y][X] == partida->jogador){
                    contJogador++;
                }
            }
        }
        if(adversario == 1){
            if(partida->pecasJogador1 == contAdversario && contJogador == 0)
                return 1;
        } else {
            if(partida->pecasJogador2 == contAdversario && contJogador == 0)
                return 1;
        }
    } else {
         for (int X = x+1  ; X < 5 ; X++){
            for(int Y = 0 ; Y < 5 ; Y++){
                if(partida->tabuleiro[Y][X] == adversario){
                    contAdversario++;
                }
                if(partida->tabuleiro[Y][X] == partida->jogador){
                    contJogador++;
                }
            }
        }
        if(adversario == 1){
            if(partida->pecasJogador1 == contAdversario && contJogador == 0)
                return 1;
        } else {
            if(partida->pecasJogador2 == contAdversario && contJogador == 0)
                return 1;
        }

    }
    return 0;
}

int verificaVitoria(jogo* partida) {
    short adversario = (partida->jogador == 1) ? 2 : 1;
    short semCapturas = 0;

    if (partida->pecasJogador1 <= 0) {
        return 2;
    } else if (partida->pecasJogador2 <= 0) {
        return 1;
    }

    for (int y = 1; y < 4; y++) {
        int cont = 0;
        for (int x = 0; x < 5; x++) {
            if (partida->tabuleiro[y][x] == partida->jogador) {
                cont++;
            } else {
                cont = 0;
            }

            if (cont == 5) {
                    printf("\033[9;5Hcont = %d", cont);
                    printf("\033[15;5H ADV = %d", adversario);
                    printf("\033[10;5Hjogador = %d", partida->jogador);
                if (verifica_barreira(partida, y, x, 0)) {
                    printf("\033[13;5Hverifica_barreira -> %d", verifica_barreira(partida, y, x, 0));
                    return partida->jogador;
                }
            }
        }
    }

    for (int x = 1; x < 4; x++) {
        int cont = 0;
        for (int y = 0; y < 5; y++) {
            if (partida->tabuleiro[y][x] == partida->jogador) {
                cont++;
            } else {
                cont = 0;
            }

            if (cont == 5) {
                    printf("\033[9;5Hcont = %d", cont);
                    printf("\033[15;5H ADV = %d", adversario);
                    printf("\033[10;5Hjogador = %d", partida->jogador);
                if (verifica_barreira(partida, y, x, 1)) {
                    printf("\033[13;5Hverifica_barreira -> %d", verifica_barreira(partida, y, x, 0));
                    return partida->jogador;
                }
            }
        }
    }
    if (partida->pecasJogador1 <= 3 && partida->pecasJogador2 <= 3) {
        for (int y = 0; y < TAM_TABULEIRO; y++) {
            for (int x = 0; x < TAM_TABULEIRO; x++) {
                if (partida->tabuleiro[y][x] == partida->jogador) {
                    if (avaliacao_silenciosa_movimento(y, x, partida)) {
                        semCapturas++;
                    }
                }
            }
        }

        if (semCapturas == 0) {
            return 3;
        }
    }
    return 0;
}

void cpuRemovePecaPresa(int* Y, int* X, jogo* partida){
    int centroY = TAM_TABULEIRO / 2;
    int centroX = TAM_TABULEIRO / 2;
    int direcoes[4][2] = {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };

    int adversario = (partida->jogador == 1) ? 2 : 1;
    int melhorDistancia = 1000;
    int alvoY = -1, alvoX = -1;

    for (int y = 0; y < TAM_TABULEIRO; y++) {
        for (int x = 0; x < TAM_TABULEIRO; x++) {
            if (partida->tabuleiro[y][x] == partida->jogador) {
                for (int i = 0; i < 4; i++) {
                    int adjY = y + direcoes[i][0];
                    int adjX = x + direcoes[i][1];

                    if (dentroDoLimite(adjY, adjX)) {
                        if (partida->tabuleiro[adjY][adjX] == adversario) {
                            int distancia = abs(centroY - adjY) + abs(centroX - adjX);

                            if (distancia < melhorDistancia || 
                                (distancia == melhorDistancia && rand() % 2 == 0)) {
                                melhorDistancia = distancia;
                                *Y = adjY;
                                *X = adjX;
                            }
                        }
                    }
                }
            }
        }
    }
}

void cpuCalculaMovimento(int* origemY, int* origemX, int* destinoY, int* destinoX, jogo* partida){
     int melhorPontuacao = -10000;
    int jogadorCPU = partida->jogador;
    int adversario = (jogadorCPU == 1) ? 2 : 1;

    int direcoes[4][2] = {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };

    for (int y = 0; y < TAM_TABULEIRO; y++) {
        for (int x = 0; x < TAM_TABULEIRO; x++) {
            if (partida->tabuleiro[y][x] == jogadorCPU) {
                for (int i = 0; i < 4; i++) {
                    int novoYPos = y + direcoes[i][0];
                    int novoXPos = x + direcoes[i][1];

                    if (dentroDoLimite(novoYPos, novoXPos) && partida->tabuleiro[novoYPos][novoXPos] == 0) {
                        int pontuacao = 0;

                        if (avaliacao_silenciosa_captura(novoYPos, novoXPos, partida)){
                            pontuacao += 10;
                        }

                        pontuacao += (4 - abs(4 - novoYPos)) + (4 - abs(4 - novoXPos));

                        for (int j = 0; j < 4; j++) {
                            int perigosoY = novoYPos + direcoes[j][0];
                            int perigosoX = novoXPos + direcoes[j][1];
                            if (dentroDoLimite(perigosoY, perigosoX) && partida->tabuleiro[perigosoY][perigosoX] == adversario) {
                                pontuacao -= 5;
                            }
                        }

                        if (pontuacao > melhorPontuacao) {
                            melhorPontuacao = pontuacao;
                            *origemY = y;
                            *origemX = x;
                            *destinoY = novoYPos;
                            *destinoX = novoXPos;
                        }
                    }
                }
            }
        }
    }
}

void partidaFluxo(int Y, int X, short turnoParte, bool turnoCompleto, jogo* partida, turno* fluxo){
    char* alvo = NULL;

    if (turnoParte == 1) {
        alvo = fluxo[partida->turnoIndice].origem;
    } else if (turnoParte == 2) {
        alvo = fluxo[partida->turnoIndice].destino;
    } else if (turnoParte >= 3 && turnoParte <= 5) {
        alvo = fluxo[partida->turnoIndice].capturas[turnoParte - 3];
    }

    if (alvo) {
        alvo[0] = 'A' + Y;
        alvo[1] = '1' + X;
        alvo[2] = '\0';
    }

    if (turnoParte == 5 || turnoCompleto) {
        partida->turnoIndice++;
    }
}

void fase2(jogo* partida){
    partida->jogador = (partida->jogador == 1) ? 2 : 1;
    partida->modo = 1;
    int direcoes[4][2] = {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };

    int destaque[5][5] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };
    turno fluxo[10];

    for (int i = 0; i < 10; i++) {
        fluxo[i].jogador = 1;
        strcpy(fluxo[i].origem, "00");
        strcpy(fluxo[i].destino, "00");
        for (int j = 0; j < 3; j++) {
            strcpy(fluxo[i].capturas[j], "  ");
        }
        fluxo[i].numCapturas = 0;
        fluxo[i].turnoExtra = 0;
    }

    int Y = 2, X = 2;
    int pecaY = 0, pecaX = 0;
    bool pecaCapturada = 0;
    bool jogando = 1;
    short vencedor = 0;
    bool pecaSelecionada = 0;
    bool destinoValido = 0;
    int adversario = (partida->jogador == 1) ? 2 : 1;
    
    do {
        desenhaPosicoes(partida);
        d_Linha(3, 1, 0, 73, 97, 39, " ");
        printf("\033[1;75H\033[97;45mJogando:");
        printf("\033[2;75H\033[97;45mJogador %d", partida->jogador);
        d_Linha(23, 23, 0, 46, 39, 39, " ");

        d_Linha(9, 62, 0, 2, COR_JGDR_1, COR_JGDR_1, "█");
        printf(" = %d ", partida->pecasJogador1);
        d_Linha(11, 62, 0, 2, COR_JGDR_2, COR_JGDR_2, "█");
        printf(" = %d ", partida->pecasJogador2);
        printf("\033[0m");

        printf("\033[h\033[0m");

        pecaSelecionada = 0;
        destinoValido = 0;

        while (!pecaSelecionada){
            pecaSelecionada = 0;
            pecaCapturada = 0;
            if (avaliacao_trancado(partida)){
                do {
                    printf("\033[2;27HNão há movimentos disponíveis!");
                    printf("\033[3;25HSelecione uma peça adversária para");
                    printf("\033[4;30Hremovê-la do tabuleiro.");
                    entradaTeclado(&Y, &X, partida, 1);
                    if (partida->tabuleiro[Y][X] != partida->jogador){
                        for (int i = 0; i < 4; i++) {
                            int dy = direcoes[i][0];
                            int dx = direcoes[i][1];

                            if (dentroDoLimite(Y + dy, X + dx) &&
                                partida->tabuleiro[Y + dy][X + dx] == partida->jogador &&
                                partida->tabuleiro[Y][X] != 0){
                                    d_Linha(2, 27, 0, 30, 39, 39, " ");
                                    d_Linha(3, 25, 0, 34, 39, 39, " ");
                                    d_Linha(4, 30, 0, 23, 39, 39, " ");
                                    d_Linha(23, 29, 0, 26, 39, 39, " ");
                                    partida->tabuleiro[Y][X] = 0;
                                    if (partida->jogador == 1)
                                        partida->pecasJogador2 -= 1;
                                    else if (partida->jogador == 2)
                                        partida->pecasJogador1 -= 1;
                                    desenhaPosicao(Y, X, 30, 30);
                            }
                        }
                    }
                    if (avaliacao_trancado(partida)){
                        printf("\033[23;29HSelecione uma peça válida!");
                    }
                } while (avaliacao_trancado(partida));
            }
            selecionaPeca(&Y, &X, partida, 1);
            d_Linha(25, 31, 0, 22, 96, 39, "█");

            if (partida->tabuleiro[Y][X] == partida->jogador){
                avaliacao_destaque(&Y, &X, partida, destaque);
                } else {
                printf("\033[25;32H\033[31;106mMovimento Inválido!\033[0m");
                continue;
            }
            pecaSelecionada = 1;
            pecaY = Y;
            pecaX = X;

            selecionaPecaPosicao(&Y, &X, partida, destaque);
            if (destaque[Y][X] == 2){
                desenhaPosicao(pecaY, pecaX, 30, 30);
                partida->tabuleiro[pecaY][pecaX] = 0;
                partida->tabuleiro[Y][X] = partida->jogador;

                removePeca(Y, X, partida, &pecaCapturada);
                memset(destaque, 0, sizeof(destaque));
                pecaY = Y;
                pecaX = X;

                if (avaliacao_silenciosa(&Y, &X, partida) && pecaCapturada == 1){
                    do {
                        desenhaPosicoes(partida);
                        printf("\033[25;34H\033[32;106mMovimento Extra!\033[0m");

                        avaliacao_desenha(&Y, &X, partida);
                        avaliacao_destaque(&Y, &X, partida, destaque);
                        selecionaPecaPosicao(&Y, &X, partida, destaque);

                        if (destaque[Y][X] == 2 && avaliacao_silenciosa_captura(Y, X, partida)){
                            d_Linha(9, 62, 0, 2, COR_JGDR_1, COR_JGDR_1, "█");
                            printf(" = %d ", partida->pecasJogador1);
                            d_Linha(11, 62, 0, 2, COR_JGDR_2, COR_JGDR_2, "█");
                            printf(" = %d ", partida->pecasJogador2);
                            
                            d_Linha(25, 31, 0, 22, 96, 39, "█");
                            desenhaPosicao(pecaY, pecaX, 30, 30);
                            partida->tabuleiro[pecaY][pecaX] = 0;
                            partida->tabuleiro[Y][X] = partida->jogador;

                            removePeca(Y, X, partida, &pecaCapturada);
                            memset(destaque, 0, sizeof(destaque));

                            pecaY = Y;
                            pecaX = X;
                        } else {
                            printf("\033[25;32H\033[31;106mMovimento Inválido!\033[0m");
                            Y = pecaY;
                            X = pecaX;
                        }
                    } while (avaliacao_silenciosa(&Y, &X, partida));
                }
                desenhaPosicoes(partida);
                partida->jogador = (partida->jogador == 1) ? 2 : 1;
                adversario = (partida->jogador == 1) ? 2 : 1;
            } else {
                printf("\033[25;32H\033[31;106mMovimento Inválido!\033[0m");
            }
        }
        vencedor = verificaVitoria(partida);
    } while (!vencedor);
    d_Linha(9, 62, 0, 2, COR_JGDR_1, COR_JGDR_1, "█");
    printf(" = %d ", partida->pecasJogador1);
    d_Linha(11, 62, 0, 2, COR_JGDR_2, COR_JGDR_2, "█");
    printf(" = %d ", partida->pecasJogador2);
    fimDeJogo(1, vencedor, partida);
}

void fase2CPU(jogo* partida){
    partida->jogador = (partida->jogador == 1) ? 2 : 1;
    int direcoes[4][2] = {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };
    int destaque[5][5] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };

    int Y = 2, X = 2;
    int Y2 = 0, X2 = 0;
    int pecaY = 0, pecaX = 0;
    bool pecaCapturada = 0;
    bool jogando = 1;
    short vencedor = 0;
    bool pecaSelecionada = 0;
    bool destinoValido = 0;
    int adversario = (partida->jogador == 1) ? 2 : 1;
    partida->modo = 0;
    
    do {
        desenhaPosicoes(partida);
        d_Linha(3, 1, 0, 73, 97, 39, " ");
        printf("\033[1;75H\033[97;45mJogando:");
        printf("\033[2;75H\033[97;45mJogador %d", partida->jogador);
        d_Linha(23, 23, 0, 46, 39, 39, " ");

        d_Linha(9, 62, 0, 2, COR_JGDR_1, COR_JGDR_1, "█");
        printf(" = %d ", partida->pecasJogador1);
        d_Linha(11, 62, 0, 2, COR_JGDR_2, COR_JGDR_2, "█");
        printf(" = %d ", partida->pecasJogador2);
        printf("\033[0m");

        printf("\033[h\033[0m");

        pecaSelecionada = 0;
        destinoValido = 0;

        while (!pecaSelecionada){
            pecaSelecionada = 0;
            pecaCapturada = 0;
            if (avaliacao_trancado(partida)){
                do {
                    printf("\033[2;27HNão há movimentos disponíveis!");
                    printf("\033[3;25HSelecione uma peça adversária para");
                    printf("\033[4;30Hremovê-la do tabuleiro.");
                    if(partida->jogador == 2)
                        entradaTeclado(&Y, &X, partida, 0);
                    else
                        cpuRemovePecaPresa(&Y, &X, partida);
                    if (partida->tabuleiro[Y][X] != partida->jogador){
                        for (int i = 0; i < 4; i++) {
                            int dy = direcoes[i][0];
                            int dx = direcoes[i][1];

                            if (dentroDoLimite(Y + dy, X + dx) &&
                                partida->tabuleiro[Y + dy][X + dx] == partida->jogador &&
                                partida->tabuleiro[Y][X] != 0){
                                    d_Linha(2, 27, 0, 30, 39, 39, " ");
                                    d_Linha(3, 25, 0, 34, 39, 39, " ");
                                    d_Linha(4, 30, 0, 23, 39, 39, " ");
                                    d_Linha(23, 29, 0, 26, 39, 39, " ");
                                    partida->tabuleiro[Y][X] = 0;
                                    if (partida->jogador == 1)
                                        partida->pecasJogador2 -= 1;
                                    else if (partida->jogador == 2)
                                        partida->pecasJogador1 -= 1;
                                    desenhaPosicao(Y, X, 30, 30);
                            }
                        }
                    }
                    if (avaliacao_trancado(partida)){
                        printf("\033[23;29HSelecione uma peça válida!");
                    }
                } while (avaliacao_trancado(partida));
            }
            if(partida->jogador == 1)
                selecionaPeca(&Y, &X, partida, 0);
            else
                cpuCalculaMovimento(&Y, &X, &Y2, &X2, partida);
            
            d_Linha(25, 31, 0, 22, 96, 39, "█");

            if (partida->tabuleiro[Y][X] == partida->jogador){
                avaliacao_destaque(&Y, &X, partida, destaque);
                } else {
                printf("\033[25;32H\033[31;106mMovimento Inválido!\033[0m");
                continue;
            }
            pecaSelecionada = 1;
            pecaY = Y;
            pecaX = X;
            if(partida->jogador == 2){
                Y = Y2;
                X = X2;
            } else
                selecionaPecaPosicao(&Y, &X, partida, destaque);
            
            if (destaque[Y][X] == 2){
                desenhaPosicao(pecaY, pecaX, 30, 30);
                partida->tabuleiro[pecaY][pecaX] = 0;
                partida->tabuleiro[Y][X] = partida->jogador;

                removePeca(Y, X, partida, &pecaCapturada);
                memset(destaque, 0, sizeof(destaque));
                pecaY = Y;
                pecaX = X;

                if (avaliacao_silenciosa(&Y, &X, partida) && pecaCapturada == 1){
                    do {
                        desenhaPosicoes(partida);
                        printf("\033[25;34H\033[32;106mMovimento Extra!\033[0m");

                        avaliacao_desenha(&Y, &X, partida);
                        avaliacao_destaque(&Y, &X, partida, destaque);
                        if(partida->jogador == 1)
                            selecionaPecaPosicao(&Y, &X, partida, destaque);
                        else
                            cpuCalculaMovimento(&Y2, &X2, &Y, &X, partida);

                        if (destaque[Y][X] == 2 && avaliacao_silenciosa_captura(Y, X, partida)){
                            d_Linha(9, 62, 0, 2, COR_JGDR_1, COR_JGDR_1, "█");
                            printf(" = %d ", partida->pecasJogador1);
                            d_Linha(11, 62, 0, 2, COR_JGDR_2, COR_JGDR_2, "█");
                            printf(" = %d ", partida->pecasJogador2);
                            
                            d_Linha(25, 31, 0, 22, 96, 39, "█");
                            desenhaPosicao(pecaY, pecaX, 30, 30);
                            partida->tabuleiro[pecaY][pecaX] = 0;
                            partida->tabuleiro[Y][X] = partida->jogador;

                            removePeca(Y, X, partida, &pecaCapturada);
                            memset(destaque, 0, sizeof(destaque));

                            pecaY = Y;
                            pecaX = X;
                        } else {
                            printf("\033[25;32H\033[31;106mMovimento Inválido!\033[0m");
                            Y = pecaY;
                            X = pecaX;
                        }
                    } while (avaliacao_silenciosa(&Y, &X, partida));
                }
                desenhaPosicoes(partida);
                partida->jogador = (partida->jogador == 1) ? 2 : 1;
                adversario = (partida->jogador == 1) ? 2 : 1;
            } else {
                printf("\033[25;32H\033[31;106mMovimento Inválido!\033[0m");
            }
        }
        vencedor = verificaVitoria(partida);
    } while (!vencedor);

    d_Linha(9, 62, 0, 2, COR_JGDR_1, COR_JGDR_1, "█");
    printf(" = %d ", partida->pecasJogador1);
    d_Linha(11, 62, 0, 2, COR_JGDR_2, COR_JGDR_2, "█");
    printf(" = %d ", partida->pecasJogador2);
    fimDeJogo(0, vencedor, partida);
}

void fase1(jogo* partida){
    int Y = 2, X = 2;
    int pecasPorJogador = partida->pecasTotal/2;
    partida->jogador = rand() % 2;
    partida->modo = 3;

    printf("\033[6;62HPeças em");
    printf("\033[7;64HJogo:");
    d_Linha(9, 62, 0, 2, COR_JGDR_1, COR_JGDR_1, "█");
    printf(" = 0");
    d_Linha(11, 62, 0, 2, COR_JGDR_2, COR_JGDR_2, "█");
    printf(" = 0");
    printf("\033[0m");

    while (partida->pecasJogador1 < pecasPorJogador || partida->pecasJogador2 < pecasPorJogador) {
        printf("\033[1;75H\033[97;45mJogando:");
        printf("\033[2;75H\033[97;45mJogador %d", partida->jogador);
        d_Linha(23, 23, 0, 40, 39, 39, " ");
        for (int turn = 0; turn < 2; turn++) {
            if (partida->jogador == 1) {
                bool movimento_Valido = false;
                while (!movimento_Valido) {
                    entradaTeclado(&Y, &X, partida, 3);
                    d_Linha(25, 31, 0, 22, 96, 39, "█");
                    if (posicaoValida(Y, X, partida)) {
                        partida->tabuleiro[Y][X] = 1;
                        d_Retangulo_Preenchido(LN+1 + Y*3, COL+2 + X*6, LN+2 + Y*3, COL+5 + X*6, COR_JGDR_1, COR_JGDR_1, "█");
                        d_Linha(9, 62, 0, 2, COR_JGDR_1, COR_JGDR_1, "█");
                        printf(" = %d", partida->pecasJogador1+1);
                        partida->pecasJogador1++;
                        movimento_Valido = true;
                    } else {
                        d_Linha(25, 31, 0, 22, 96, 39, "█");
                        printf("\033[25;32H\033[31;106mMovimento Inválido!\033[0m");
                    }
                }
            } else {
                bool movimento_Valido = false;
                while (!movimento_Valido) {
                    entradaTeclado(&Y, &X, partida, 3);
                    d_Linha(25, 31, 0, 22, 96, 39, "█");
                    if (posicaoValida(Y, X, partida)) {
                        partida->tabuleiro[Y][X] = 2;
                        d_Retangulo_Preenchido(LN+1 + Y*3, COL+2 + X*6, LN+2 + Y*3, COL+5 + X*6, COR_JGDR_2, COR_JGDR_2, "█");
                        d_Linha(11, 62, 0, 2, COR_JGDR_2, COR_JGDR_2, "█");
                        printf(" = %d", partida->pecasJogador2+1);
                        partida->pecasJogador2++;
                        movimento_Valido = true;
                    } else {
                        d_Linha(25, 31, 0, 22, 96, 39, "█");
                        printf("\033[25;32H\033[31;106mMovimento Inválido!\033[0m");
                    }
                }
            }
        }
        partida->jogador = (partida->jogador == 1) ? 2 : 1;
    }
    fase2(partida); 
}

void fase1CPU(jogo* partida) {
    int Y = 2, X = 2;
    int pecasPorJogador = partida->pecasTotal/2;
    partida->jogador = rand() % 2;
    partida->modo = 3;

    printf("\033[6;62HPeças em");
    printf("\033[7;64HJogo:");
    d_Linha(9, 62, 0, 2, COR_JGDR_1, COR_JGDR_1, "█");
    printf(" = 0");
    d_Linha(11, 62, 0, 2, COR_JGDR_2, COR_JGDR_2, "█");
    printf(" = 0");
    printf("\033[0m");

    while (partida->pecasJogador1 < pecasPorJogador || partida->pecasJogador2 < pecasPorJogador) {
        printf("\033[1;75H\033[97;45mJogando:");
        printf("\033[2;75H\033[97;45mJogador %d", partida->jogador);
        d_Linha(23, 23, 0, 40, 39, 39, " ");
        for (int turn = 0; turn < 2; turn++) {
            if (partida->jogador == 1) {
                bool movimento_Valido = false;
                while (!movimento_Valido) {
                    entradaTeclado(&Y, &X, partida, 3);
                    d_Linha(25, 31, 0, 22, 96, 39, "█");
                    if (posicaoValida(Y, X, partida)) {
                        partida->tabuleiro[Y][X] = 1;
                        d_Retangulo_Preenchido(LN+1 + Y*3, COL+2 + X*6, LN+2 + Y*3, COL+5 + X*6, COR_JGDR_1, COR_JGDR_1, "█");
                        d_Linha(9, 62, 0, 2, COR_JGDR_1, COR_JGDR_1, "█");
                        printf(" = %d", partida->pecasJogador1+1);
                        partida->pecasJogador1++;
                        movimento_Valido = true;
                    } else {
                        d_Linha(25, 31, 0, 22, 96, 39, "█");
                        printf("\033[25;32H\033[31;106mMovimento Inválido!\033[0m");
                    }
                }
            } else {
                bool movimento_Valido = false;
                while (!movimento_Valido) {
                    CPUCriaPeca(&Y, &X, partida);
                    if (posicaoValida(Y, X, partida)) {
                        partida->tabuleiro[Y][X] = 2;
                        d_Retangulo_Preenchido(LN+1 + Y*3, COL+2 + X*6, LN+2 + Y*3, COL+5 + X*6, COR_JGDR_2, COR_JGDR_2, "█");
                        d_Linha(11, 62, 0, 2, COR_JGDR_2, COR_JGDR_2, "█");
                        printf(" = %d", partida->pecasJogador2+1);
                        partida->pecasJogador2++;
                        movimento_Valido = true;
                    }
                }
            }
        }
        partida->jogador = (partida->jogador == 1) ? 2 : 1;
    }
    fase2CPU(partida);
}

bool posicaoValida(int Y, int X, jogo* partida) {
    return X >= 0 && X < 5 && Y >= 0 && Y < 5 && partida->tabuleiro[Y][X] == 0 && !(Y == 2 && X == 2);
}

void novoJogoInicializa(int modo){
    jogo partida = {
        .tabuleiro = {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}
        },
        .pecasTotal = 24,
        .turnoIndice = 0,
        .pecasJogador1 = 0,
        .pecasJogador2 = 0,
        .jogador = 1,
        .tempoJogo = 0,
        .tempoInicial = time(NULL),
        .tempoDecorrido = 0.0,
        .isPausado = false
    };

    desenhaMenuLateral();
    desenhaTabuleiro(&partida);

    if(modo)
        fase1(&partida);
    else
        fase1CPU(&partida);
}

void desenhaMenuPrincipal(){
    int cor = 0;
    int x = 27;

    printf("\033[0m");
    d_Linha(1, 1, 0, 90, 96, 30, "░");
    d_Linha(25, 1, 0, 90, 96, 30, "░");
    d_Retangulo_Preenchido(2, 1, 25, 2, 96, 30, "░");
    d_Retangulo_Preenchido(2, 89, 25, 90, 96, 30, "░");
    
    for(int i = 0 ; i < 5 ; i++){  
        cor = i % 2 == 0 ? 34 : 31;
        d_Linha(3, x + (i*8), 0, 6, cor, 39, "▄");
    }
    for(int i = 0 ; i < 5 ; i++){  
        cor = i % 2 == 0 ? 31 : 34;
        d_Linha(10, x + (i*8), 0, 6, cor, 39, "▄");
    }

    d_Linha(6, 9, 0, 8, 31, 39, "░");
    d_Linha(6, 17, 0, 8, 32, 39, "░");
    d_Linha(8, 9, 0, 8, 35, 39, "░");
    d_Linha(8, 17, 0, 8, 93, 39, "░");

    d_Linha(6, 67, 0, 8, 93, 39, "░");
    d_Linha(6, 75, 0, 8, 35, 39, "░");
    d_Linha(8, 67, 0, 8, 32, 39, "░");
    d_Linha(8, 75, 0, 8, 31, 39, "░");

    for(int i = 0 ; i < 5 ; i++)
        d_Linha(5, x+(i*8), 0, 6, 97, 39, "█");
    for(int i = 0 ; i < 5 ; i++)
        d_Linha(6, x+(i*8), 0, 2, 97, 39, "█");
    d_Linha(6, 63, 0, 2, 97, 39, "█");
    for(int i = 1 ; i < 5 ; i++)
        d_Linha(7, x+(i*8), 0, 4, 97, 39, "█");
    d_Linha(7, 27, 0, 6, 97, 39, "█");
    d_Linha(7, 55, 0, 2, 97, 39, "█");
    d_Linha(7, 63, 0, 2, 97, 39, "█");

    for(int i = 1 ; i < 5 ; i++)
        d_Linha(8, x+(i*8), 0, 6, 97, 39, "█");
    d_Linha(8, 31, 0, 2, 97, 39, "█");

    for(int i = 0 ; i < 5 ; i++)
        d_Linha(9, x+(i*8), 0, 6, 97, 39, "█");

    x = 28;
    for(int i = 0 ; i < 5 ; i++)
        d_Linha(4, x+(i*8), 0, 6, 90, 39, "▄");
    for(int i = 0 ; i < 5 ; i++)
        d_Linha(9, x+5+(i*8), 0, 1, 90, 39, "▀");

    for(int i = 0 ; i < 4 ; i++)
        d_Pixel(5, x+5+(i*8), 90, 39, "▀");
    d_Linha(5, 65, 4, 0, 90, 39, "█");

    for(int i = 0 ; i < 5 ; i++)
        d_Pixel(6, x+1 + (i*8), 90, 39, "█");
    d_Pixel(6, 33, 90, 39, "▄");
    d_Linha(7, 33, 2, 0, 90, 39, "█");
    d_Linha(6, 30, 0, 3, 90, 39, "▄");
    d_Linha(8, 28, 0, 3, 90, 39, "▄");
    for(int i = 1 ; i < 3 ; i++)
        d_Linha(6, x+2+(i*8), 0, 2, 90, 39, "▄");
    d_Pixel(7, 39, 90, 39, "▀");
    d_Pixel(7, 47, 90, 39, "▀");

    for(int i = 1 ; i < 5 ; i++)
        d_Pixel(8, x+1 + (i*8), 90, 39, "█");
    d_Linha(8, 38, 0, 4, 90, 39, "▄");
    d_Linha(8, 46, 0, 4, 90, 39, "▄");
    d_Linha(6, 55, 0, 3, 90, 39, "▄");
    d_Linha(7, 57, 2, 0, 90, 39, "█");
    d_Linha(8, 62, 2, 0, 39, 39, " ");
    d_Pixel(9, 61, 90, 39, "▀");
    d_Pixel(6, 62, 90, 39, "▄");
    d_Pixel(7, 53, 90, 39, "█");
    d_Pixel(8, 54, 90, 39, "▄");
}

void desenhaHistorico(jogo* partida){
    d_Linha(1, 1, 0, 90, 96, 30, "░");
    d_Linha(25, 1, 0, 90, 96, 30, "░");
    d_Retangulo_Preenchido(2, 1, 25, 2, 96, 30, "░");
    d_Retangulo_Preenchido(2, 89, 25, 90, 96, 30, "░");

    puts("\033[3;36H\033[97;49mHistórico de Partidas");
    d_Retangulo_Preenchido(5, 27, 23, 64, 37, 37, "█");
    d_Linha(24, 28, 0, 38, 90, 39, "▀");
    d_Linha(6, 65, 18, 0, 90, 39, "█");
    d_Pixel(5, 65, 90, 39, "▄");

    puts("\033[21;9H\033[97;49mPressione [ESC]");
    puts("\033[22;9H\033[97;49mpara retornar ao");
    puts("\033[23;9H\033[97;49mmenu principal");

    d_Pixel(7, 28, 30, 37, "╔");
    d_Linha(7, 29, 0, 34, 30, 37, "═");
    d_Pixel(7, 42, 30, 37, "╤");
    d_Pixel(7, 53, 30, 37, "╤");
    d_Pixel(7, 63, 30, 37, "╗");
    d_Linha(8, 63, 15, 0, 30, 37, "║");
    d_Pixel(9, 63, 30, 37, "╢");
    d_Pixel(20, 63, 30, 37, "╢");
    d_Pixel(23, 63, 30, 37, "╝");
    d_Linha(23, 29, 0, 34, 30, 37, "═");
    d_Pixel(23, 28, 30, 37, "╚");
    d_Linha(8, 28, 15, 0, 30, 37, "║");
    d_Pixel(9, 28, 30, 37, "╟");
    d_Pixel(20, 28, 30, 37, "╟");

    d_Linha(8, 42, 12, 0, 30, 37, "│");
    d_Linha(8, 53, 12, 0, 30, 37, "│");
    d_Linha(9, 29, 0, 34, 30, 37, "─");
    d_Linha(20, 29, 0, 34, 30, 37, "─");
    d_Pixel(9, 42, 30, 37, "┼");
    d_Pixel(9, 53, 30, 37, "┼");
    d_Pixel(20, 42, 30, 37, "┴");
    d_Pixel(20, 53, 30, 37, "┴");

    puts("\033[6;30H\033[30;47mTotal:");
    puts("\033[6;48H\033[30;47mPágina   de");

    puts("\033[8;31H\033[30;47mVencedor:");
    puts("\033[8;45H\033[30;47mTempo:");
    puts("\033[8;56H\033[30;47mModo:");

    getchar();
    printf("\033[0m");
    system(CLEAR);
    fflush(stdin);
    desenhaMenuPrincipal();
    menuPrincipal(partida);
}

void desenhaAjuda(jogo* partida){
    d_Linha(1, 1, 0, 90, 96, 30, "░");
    d_Linha(25, 1, 0, 90, 96, 30, "░");
    d_Retangulo_Preenchido(2, 1, 25, 2, 96, 30, "░");
    d_Retangulo_Preenchido(2, 89, 25, 90, 96, 30, "░");

    puts("\033[3;39H\033[97;49mRegras e Ajuda");
    d_Retangulo_Preenchido(5, 6, 21, 85, 37, 37, "█");
    d_Linha(22, 7, 0, 80, 90, 39, "▀");
    d_Linha(6, 86, 16, 0, 90, 39, "█");
    d_Pixel(5, 86, 90, 39, "▄");
    puts("\033[23;24H\033[97;49mPressione [ESC] para retornar ao Menu Principal");

    d_Pixel(5, 7, 30, 37, "╔");
    d_Linha(5, 8, 0, 76, 30, 37, "═");
    d_Pixel(5, 84, 30, 37, "╗");
    d_Linha(6, 84, 15, 0, 30, 37, "║");
    d_Pixel(21, 84, 30, 37, "╝");
    d_Linha(21, 8, 0, 76, 30, 37, "═");
    d_Pixel(21, 7, 30, 37, "╚");
    d_Linha(6, 7, 15, 0, 30, 37, "║");

    d_Linha(19, 30, 0, 12, 90, 39, "█");
    d_Linha(20, 31, 0, 12, 30, 37, "▀");
    d_Pixel(19, 42, 30, 37, "▄");
    d_Linha(19, 47, 0, 12, 90, 39, "█");
    d_Linha(20, 48, 0, 11, 30, 37, "▀");
    d_Pixel(19, 58, 30, 37, "▄");
    puts("\033[19;31H\033[97;100m← Anterior");
    puts("\033[19;48H\033[97;100mPróximo →");

    puts("\033[19;31H\033[97;100m← Anterior");

    getchar();
    printf("\033[0m");
    system(CLEAR);
    fflush(stdin);
    desenhaMenuPrincipal();
    menuPrincipal(partida);
}

void menuPrincipal(jogo* partida) {
    const char* opcoes[] = {
        "Novo Jogo", 
        "Carregar Jogo", 
        "Histórico", 
        "Ajuda", 
        "Sair"
    };
    const int numOpcoes = 5;
    const int Ln = 14;
    const int Col = 41;
    int posicao = 0;
    d_Linha(12, 27, 0, 38, 39, 39, " ");

    while (1){

        for (int i = 0; i < numOpcoes; i++) {
            if (i == posicao) {
                printf("\033[%d;%df\033[7m%s\033[0m", 
                       Ln + i, Col, opcoes[i]);
            } else {
                printf("\033[%d;%df\033[0m%s", Ln + i, Col, opcoes[i]);
            }
        }

        int tecla = getch();
        if (tecla == 224) {
            int seta = getch();
            if (seta == 72){
                posicao = (posicao - 1 + numOpcoes) % numOpcoes;
            } else if (seta == 80) {
                posicao = (posicao + 1) % numOpcoes;
            }
        } else if (tecla == 13) {
            break;
        }
    }
    switch (posicao){
    case 0:
        d_Retangulo_Preenchido(13, 39, 20, 49, 39, 39, " ");
        menuNovoJogo(partida);
        break;
    case 1:
        d_Retangulo_Preenchido(13, 39, 20, 49, 39, 39, " ");
        menuCarregar(partida);
        break;
    case 2:
        system(CLEAR);
        desenhaHistorico(partida);
        break;
    case 3:
        system(CLEAR);
        desenhaAjuda(partida);
        break;
    case 4:
        system(CLEAR);
        printf("\033[?25h\033[h\033[0m");
        printf("Obrigado por jogar!");
        exit(0);
        break;
    }
}

void menuNovoJogo(jogo* partida){
    const char *opcoes[] = {
        "Jogador Vs Jogador", 
        "Jogador Vs Computador",
        "Voltar"
    };
    const int numOpcoes = 3;
    const int Ln = 14;
    const int Col = 37;
    int posicao = 0;
    while (1){

        for (int i = 0; i < numOpcoes; i++) {
            if (i == posicao) {
                printf("\033[%d;%df\033[7m%s\033[0m", 
                       Ln + i, Col, opcoes[i]);
            } else {
                printf("\033[%d;%df\033[0m%s", Ln + i, Col, opcoes[i]);
            }
        }


        int tecla = getch();
        if (tecla == 224) {
            int seta = getch();
            if (seta == 72){
                posicao = (posicao - 1 + numOpcoes) % numOpcoes;
            } else if (seta == 80) {
                posicao = (posicao + 1) % numOpcoes;
            }
        } else if (tecla == 13) {
            break;
        }
    }
    switch (posicao){
    case 0:
        system(CLEAR);
        novoJogoInicializa(1);
        break;
    case 1:
        system(CLEAR);
        novoJogoInicializa(0);
        break;
    case 2:
        d_Retangulo_Preenchido(13, 36, 17, 61, 39, 39, " ");
        menuPrincipal(partida);
        break;
    }
}

void menuCarregar(jogo* partida){
    const char *opcoes[] = {
        "Jogador Vs Jogador", 
        "Jogador Vs Computador",
        "Voltar"
    };
    const int numOpcoes = 3;
    const int Ln = 14;
    const int Col = 37;
    int posicao = 0;
    FILE *arquivo;

    while (1){
        for (int i = 0; i < numOpcoes; i++) {
            if (i == posicao) {
                printf("\033[%d;%df\033[7m%s\033[0m", 
                       Ln + i, Col, opcoes[i]);
            } else {
                printf("\033[%d;%df\033[0m%s", Ln + i, Col, opcoes[i]);
            }
        }
        int tecla = getch();
        if (tecla == 224) {
            int seta = getch();
            if (seta == 72){
                posicao = (posicao - 1 + numOpcoes) % numOpcoes;
            } else if (seta == 80) {
                posicao = (posicao + 1) % numOpcoes;
            }
        } else if (tecla == 13) {
            break;
        }
    }
    switch (posicao){
    case 0:
        partida->modo = 1;
        const char *arquivoModoPvP = partida->modo ? "savePvP.bin" : "saveCPU.bin";
        arquivo = fopen(arquivoModoPvP, "rb");
            if (!arquivo) {
                printf("\033[12;30HNão há um jogo salvo neste modo!");
                menuCarregar(partida);
            } else {
                system(CLEAR);
                desenhaMenuLateral();
                desenhaTabuleiro(partida);
                carregarJogo(partida, 1);
            }
        break;
    case 1:
        partida->modo = 0;
        const char *arquivoModoCPU = partida->modo ? "savePvP.bin" : "saveCPU.bin";
        arquivo = fopen(arquivoModoCPU, "rb");
            if (!arquivo) {
                printf("\033[12;30HNão há um jogo salvo neste modo!");
                menuCarregar(partida);
            } else {
                system(CLEAR);
                desenhaMenuLateral();
                desenhaTabuleiro(partida);
                carregarJogo(partida, 0);
            }
        break;
    case 2:
        d_Retangulo_Preenchido(13, 36, 17, 61, 39, 39, " ");
        menuPrincipal(partida);
        break;
    }
}


int main(){
    srand(time(NULL));
    setlocale(LC_ALL, "pt_br.UTF-8");
    printf("\033[?25l");
    jogo partida;

    desenhaMenuPrincipal();
    menuPrincipal(&partida);
    
    system(CLEAR);
    return 0;
}

void d_Retangulo(int y1, int x1, int y2, int x2, int cor, int corFundo, char* caractere){
    int R_LARGURA = abs(x2 - x1) + 1;
    int R_ALTURA = abs(y2 - y1) + 1;

    char* buffer = malloc(R_LARGURA * strlen(caractere) + 1);

    for(int i = 0 ; i < R_LARGURA ; i++)
        strncpy(buffer + i * strlen(caractere), caractere, strlen(caractere));
    buffer[R_LARGURA * strlen(caractere)] = '\0';

    printf("\033[%d;%dm", cor, corFundo + 10);
    printf("\033[%d;%dH%s", y1, x1, buffer);
    for (int H = y1 + 1; H < y2; H++) {
        printf("\033[%d;%dH%s", H, x1, caractere);
        printf("\033[%d;%dH%s", H, x2, caractere);
    }

    printf("\033[%d;%dH%s", y2, x1, buffer);

    printf("\033[0m");
    free(buffer);
}

void d_Retangulo_Preenchido(int y1, int x1, int y2, int x2, int cor, int corFundo, char* caractere){
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

void d_preenche_Tela(int cor, int corFundo, char* caractere){
    char* buffer = malloc(LARGURA * strlen(caractere));

    for(int i = 0 ; i < LARGURA ; i++)
        strncpy(buffer + i * strlen(caractere), caractere, strlen(caractere));

    printf("\033[%d;%dm", cor, corFundo + 10);
    for(int H = 1 ; H <= ALTURA ; H++)
        printf("\033[%d;%dH%s", H, 1, buffer);

    printf("\033[0m");
    free(buffer);
}

void d_Linha(int y, int x, int ySize, int xSize, int cor, int corFundo, char* caractere){
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

void d_Pixel(int y, int x, int cor, int corFundo, char* caractere){
    printf("\033[%d;%dm", cor, corFundo + 10);
    printf("\033[%d;%dH%s", y, x, caractere);
    printf("\033[0m");
}