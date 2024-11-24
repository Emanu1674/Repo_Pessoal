#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

#define LARGURA 80
#define ALTURA 25
#define COR_JGDR_1 31
#define COR_JGDR_2 34
#define COL 18
#define LN 6

typedef struct{
    int hora, min, seg;
} horario;

typedef struct{
    int tabuleiro[5][5];
    short pecasTotal;
    short pecasJogador1;
    short pecasJogador2;
    int jogador;
    long tempoJogo;
} jogo;

void d_preenche_Tela(int _FGColor, int _corFundo, char* _Char);
void d_Retangulo(int _yPos1, int _xPos1, int _yPos2, int _xPos2, int _FGColor, int _corFundo, char* _Char);
void d_Retangulo_Preenchido(int _yPos1, int _xPos1, int _yPos2, int _xPos2, int _FGColor, int _corFundo, char* _Char);
void d_Linha(int _yPos, int _xPos, int _ySize, int _xSize, int _FGColor, int _corFundo, char* _Char);
void d_Pixel(int _yPos, int _xPos, int _FGColor, int _corFundo, char* _Char);

void desenhaMenuPrincipal();
void desenhaMenuLateral();
void desenhaHistorico();
void desenhaTabuleiro(jogo *_partida);

void menuPrincipal();
void menuNovoJogo();
void jogoLoop(int _modo);
int entradaTeclado(int* _rY, int* _rX, jogo *_partida);
bool posicao_Valida(int _Y, int _X, jogo* _partida);

void desenhaMenuLateral(){
d_Retangulo_Preenchido(1, 64, 2, 79, 35, 35, "█");
    d_Retangulo_Preenchido(3, 64, 24, 79, 94, 94, "█");
    d_Linha(1, 80, 2, 0, 35, 30, "░");
    d_Linha(25, 64, 0, 17, 94, 34, "░");
    d_Linha(3, 80, 22, 0, 94, 34, "░");
    d_Linha(25, 1, 0, 63, 90, 39, "░");
    d_Linha(25, 21, 0, 22, 96, 39, "█");

    printf("\033[4;65H\033[30;104mN\033[97;104movo Jogo");
    printf("\033[5;65H\033[30;104mS\033[97;104malvar Jogo");
    printf("\033[6;65H\033[30;104mC\033[97;104marregar Jogo");
    printf("\033[7;65H\033[30;104mP\033[97;104mausar Jogo");
    printf("\033[8;65H\033[30;104mA\033[97;104mjuda");
    printf("\033[10;65HVoltar ao \033[30;104mM\033[97;104menu");
    d_Linha(11, 65, 0, 14, 97, 94, "─");
    printf("\033[12;65H\033[97;104mMostrar");
    printf("\033[13;65H\033[30;104mJ\033[97;104mogadas");

    printf("\033[0m");
}

void desenhaTabuleiro(jogo *partida){
    d_Pixel(6, 15, 97, 39, "A");
    d_Pixel(9, 15, 97, 39, "B");
    d_Pixel(12, 15, 97, 39, "C"); 
    d_Pixel(15, 15, 97, 39, "D");
    d_Pixel(18, 15, 97, 39, "E");
    d_Retangulo_Preenchido(5, 16, 20, 47, 96, 30, "░");
    for(int i = 0 ; i <= 4 ; i++){
        for(int j = 0 ; j <= 4 ; j++){
            if(partida->tabuleiro[i][j] == 0)
                d_Retangulo(LN + (i*3), COL + (j*6), LN+1 + (i*3), COL+3 + (j*6), 30, 30, "█");
            else if(partida->tabuleiro[i][j] == 1)
                d_Retangulo(LN + (i*3), COL + (j*6), LN+1 + (i*3), COL+3 + (j*6), COR_JGDR_1, 30, "█");
            else if(partida->tabuleiro[i][j] == 2)
                d_Retangulo(LN + (i*3), COL + (j*6), LN+1 + (i*3), COL+3 + (j*6), COR_JGDR_2, 30, "█");
        }
    }
    d_Pixel(4, 19, 97, 39, "1");
    d_Pixel(4, 25, 97, 39, "2");
    d_Pixel(4, 31, 97, 39, "3");
    d_Pixel(4, 37, 97, 39, "4");
    d_Pixel(4, 43, 97, 39, "5");
}

int entradaTeclado(int* rY, int* rX, jogo *partida){
    int tabuleiro[5][5];
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

        d_Retangulo_Preenchido(LN + Y*3, COL + X*6, LN+1 + Y*3, COL+3 + X*6, cor+60, cor+60, "█");

        tecla = getch();        // Código de tecla especial
        if(tecla == 0 || tecla == 224) {
            tecla = getch();    // Código das teclas de setas
            switch (tecla) {
                case 72:    // Cima
                    d_Retangulo_Preenchido(LN + Y*3, COL + X*6, LN+1 + Y*3, COL+3 + X*6, cor, cor, "█");
                    Y -= 1;
                    if(Y < 0)
                        Y = 4;
                    break;
                case 80:    // Baixo
                    d_Retangulo_Preenchido(LN + Y*3, COL + X*6, LN+1 + Y*3, COL+3 + X*6, cor, cor, "█");
                    Y += 1;
                    if(Y > 4)
                        Y = 0;
                    break;
                case 75:    // Esquerda
                    d_Retangulo_Preenchido(LN + Y*3, COL + X*6, LN+1 + Y*3, COL+3 + X*6, cor, cor, "█");
                    X -= 1;
                    if(X < 0)
                        X = 4;
                    break;
                case 77:    // Direita
                    d_Retangulo_Preenchido(LN + Y*3, COL + X*6, LN+1 + Y*3, COL+3 + X*6, cor, cor, "█");
                    X += 1;
                    if(X > 4)
                        X = 0;    
                    break;
                default:
                    continue;
            }
        } else {
            // Outras teclas
            switch (tecla)
            {
            case 78:    // N
            case 110:
                // Código para iniciar novo jogo
                break;
            case 83:    // S
            case 115:
                // Código para salvar o jogo
                break;
            case 67:    // C
            case 99:
                // Código para carregar jogo salvo
                break;
            case 80:    // P
            case 112:
                // Código para pausar o jogo
                break;
            case 65:    // A
            case 97:
                // Código para abrir ajuda
                break;
            case 77:    // M
            case 109:
                // Código para voltar ao menu
                break;
            case 74:    // J
            case 106:
                // Código para mostrar os movimentos
                break;
            default:
                continue;
            }
        }
    }

    d_Retangulo_Preenchido(LN + Y * 3, COL + X * 6, LN + 1 + Y * 3, COL + 3 + X * 6, cor, cor, "█");

    *rY = Y;
    *rX = X;
}

void CPUCriaPeca(int* pY, int* pX, jogo* partida) {
    int Y, X;
    int attempts = 0;  // Total de tentativas feitas
    const int maxAttempts = 25;  // Máximo de tentativas falhas

    // Seleção aleatória com fallback em caso de loop infinito
    while (attempts < maxAttempts) {
        Y = (rand() % 5);  // Coordenada Y Aleatória (0-4)
        X = (rand() % 5);  // Coordenada X Aleatória (0-4)

        if (partida->tabuleiro[Y][X] == 0 && !(Y == 2 && X == 2)) {
            *pY = Y;
            *pX = X;
            return;  // Posição válida encontrada
        }
        attempts++;
    }

    // Pesquisa de força bruta caso a função fique presa
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (partida->tabuleiro[i][j] == 0) {
                *pY = i;
                *pX = j;
                return;  // Posição válida encontrada
            }
        }
    }
}

void fase2(jogo* partida){
    desenhaTabuleiro(partida);
    d_Linha(3, 1, 0, 63, 97, 39, " ");
    printf("\033[1;65H\033[97;45mJogando:");
    printf("\033[2;65H\033[97;45mJogador %d", partida->jogador);
    printf("\033[h\033[0m");
    getchar();
}

void fase2CPU(jogo* partida){
    desenhaTabuleiro(partida);
    d_Linha(3, 1, 0, 63, 97, 39, " ");
    printf("\033[1;65H\033[97;45mJogando:");
    printf("\033[2;65H\033[97;45mJogador %d", partida->jogador);
    printf("\033[h\033[0m");
    getchar();
}

void fase1(jogo* partida) {
    int pecasJogador1 = 1, pecasJogador2 = 1;
    int Y = 2, X = 2;
    
    printf("\033[6;52HPeças em");
    printf("\033[7;54HJogo:");
    d_Linha(9, 52, 0, 2, COR_JGDR_1, COR_JGDR_1, "█");
    printf(" = 0");
    d_Linha(11, 52, 0, 2, COR_JGDR_2, COR_JGDR_2, "█");
    printf(" = 0");
    printf("\033[0m");

    for (int etapa = 0; etapa < 6; etapa++) {
        for (int jogador = 1; jogador < 3; jogador++) {
            int peca = 0; // Reseta para cada jogador
            while (peca < 2) { // While ao invés de for pra controle mais restrito
                printf("\033[3;18HJogador %d, adicione 2 peças", jogador);
                entradaTeclado(&Y, &X, partida);
                d_Linha(25, 21, 0, 22, 96, 39, "█");
                if (Y == 2 && X == 2) { // Centro é proibido
                    d_Linha(25, 21, 0, 22, 96, 39, "█");
                    printf("\033[25;22H\033[31;106mMovimento Inválido!\033[0m");
                    continue; // Pula iteração
                }

                if (partida->tabuleiro[Y][X] == 0) { // Se posição = vazia
                    // Atualiza tabuleiro
                    partida->tabuleiro[Y][X] = jogador;
                    int cor = (jogador == 1) ? COR_JGDR_1 : COR_JGDR_2;
                    d_Retangulo_Preenchido(LN + Y*3, COL + X*6, LN + 1 + Y*3, COL + 3 + X*6, cor, cor, "█");
                    
                    // Atualiza contagem de peças
                    if (jogador == 1) {
                        d_Linha(9, 52, 0, 2, COR_JGDR_1, COR_JGDR_1, "█");
                        printf(" = %d", pecasJogador1++);
                    } else {
                        d_Linha(11, 52, 0, 2, COR_JGDR_2, COR_JGDR_2, "█");
                        printf(" = %d", pecasJogador2++);
                    }

                    peca++; // Increment apenas após jogada válida
                } else {
                    d_Linha(25, 21, 0, 22, 96, 39, "█");
                    printf("\033[25;24H\033[31;106mPosição ocupada!\033[0m");
                }
            }
        }
    }
    fase2(partida);
}

void fase1CPU(jogo* partida) {
    int Y = 2, X = 2; // Variáveis de posição
    int jogador = 1; // Jogador 1 começa
    int pecasPorJogador = 12; // Cada jogador começa com 12 peças
    int pecasJogador1 = 0, pecasComputador = 0; // Contador de peças jogadas

    printf("\033[6;52HPeças em");
    printf("\033[7;54HJogo:");
    d_Linha(9, 52, 0, 2, COR_JGDR_1, COR_JGDR_1, "█");
    printf(" = 0");
    d_Linha(11, 52, 0, 2, COR_JGDR_2, COR_JGDR_2, "█");
    printf(" = 0");
    printf("\033[0m");

    while (pecasJogador1 < pecasPorJogador || pecasComputador < pecasPorJogador) {
        for (int turn = 0; turn < 2; turn++) { // Cada jogador tem dois turnos por round
            if (jogador == 1) { // Turno do Jogador 1
                bool movimento_Valido = false;
                while (!movimento_Valido) {
                    entradaTeclado(&Y, &X, partida); // Entrada de input do Jogador
                    d_Linha(25, 21, 0, 22, 96, 39, "█");
                    if (posicao_Valida(Y, X, partida)) { // Verifica se posição é válida
                        partida->tabuleiro[Y][X] = 1; // Atualiza o tabuleiro
                        d_Retangulo_Preenchido(LN + Y * 3, COL + X * 6, LN + 1 + Y * 3, COL + 3 + X * 6, COR_JGDR_1, COR_JGDR_1, "█"); // Desenha quadrado vermelho
                        d_Linha(9, 52, 0, 2, COR_JGDR_1, COR_JGDR_1, "█");
                        printf(" = %d", pecasJogador1+1);
                        pecasJogador1++;
                        movimento_Valido = true;
                    } else {
                        d_Linha(25, 21, 0, 22, 96, 39, "█");
                        printf("\033[25;22H\033[31;106mMovimento Inválido!\033[0m");
                    }
                }
            } else { // Vez da CPU
                bool movimento_Valido = false;
                while (!movimento_Valido) {
                    CPUCriaPeca(&Y, &X, partida); // CPU seleciona posição
                    if (posicao_Valida(Y, X, partida)) { // Valida a escolha da CPU
                        partida->tabuleiro[Y][X] = 2; // Atualiza o tabuleiro
                        d_Retangulo_Preenchido(LN + Y * 3, COL + X * 6, LN + 1 + Y * 3, COL + 3 + X * 6, COR_JGDR_2, COR_JGDR_2, "█"); // Desenha quadrado azul
                        d_Linha(11, 52, 0, 2, COR_JGDR_2, COR_JGDR_2, "█");
                        printf(" = %d", pecasComputador+1);
                        pecasComputador++;
                        movimento_Valido = true;
                    }
                }
            }
        }
        // Alterna entre Jogador 1 e CPU
        jogador = (jogador == 1) ? 2 : 1;
    }
    fase2CPU(partida);
}

bool posicao_Valida(int Y, int X, jogo* partida) {
    return X >= 0 && X < 5 && Y >= 0 && Y < 5 && partida->tabuleiro[Y][X] == 0 && !(Y == 2 && X == 2);
}

void jogoLoop(int modo){
    //int pecasTotal = 24;
    //int pecasColocadas = 0;
    //int jogador = 1;

    // Inicializa Struct
    jogo partida = {
        .tabuleiro = {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}
        },
        .pecasTotal = 0,
        .pecasJogador1 = 0,
        .pecasJogador2 = 0,
        .jogador = 1,
        .tempoJogo = 0
    };

    // Desenha o Menu da partida
    desenhaMenuLateral();

    // Desenha Tabuleiro
    desenhaTabuleiro(&partida);

    if(modo)
        fase1(&partida);
    else
        fase1CPU(&partida);
}

void desenhaMenuPrincipal(){
    int cor = 0;
    int x = 22;

    printf("\033[0m");
    // Desenha borda da tela
    d_Linha(1, 1, 0, 80, 96, 30, "░");
    d_Linha(25, 1, 0, 80, 96, 30, "░");
    d_Retangulo_Preenchido(2, 1, 25, 2, 96, 30, "░");
    d_Retangulo_Preenchido(2, 79, 25, 80, 96, 30, "░");
    
    // Desenhos em volta da logo
    for(int i = 0 ; i < 5 ; i++){  
        cor = i % 2 == 0 ? 34 : 31;
        d_Linha(3, x + (i*8), 0, 6, cor, 39, "▄");
    }
    for(int i = 0 ; i < 5 ; i++){  
        cor = i % 2 == 0 ? 31 : 34;
        d_Linha(10, x + (i*8), 0, 6, cor, 39, "▄");
    }

    d_Linha(6, 4, 0, 8, 31, 39, "░");
    d_Linha(6, 12, 0, 8, 32, 39, "░");
    d_Linha(8, 4, 0, 8, 35, 39, "░");
    d_Linha(8, 12, 0, 8, 93, 39, "░");

    d_Linha(6, 62, 0, 8, 93, 39, "░");
    d_Linha(6, 70, 0, 8, 35, 39, "░");
    d_Linha(8, 62, 0, 8, 32, 39, "░");
    d_Linha(8, 70, 0, 8, 31, 39, "░");

    // Logo SEEGA
    for(int i = 0 ; i < 5 ; i++)
        d_Linha(5, x+(i*8), 0, 6, 97, 39, "█");
    for(int i = 0 ; i < 5 ; i++)
        d_Linha(6, x+(i*8), 0, 2, 97, 39, "█");
    d_Linha(6, 58, 0, 2, 97, 39, "█");
    for(int i = 1 ; i < 5 ; i++)
        d_Linha(7, x+(i*8), 0, 4, 97, 39, "█");
    d_Linha(7, 22, 0, 6, 97, 39, "█");
    d_Linha(7, 50, 0, 2, 97, 39, "█");
    d_Linha(7, 58, 0, 2, 97, 39, "█");

    for(int i = 1 ; i < 5 ; i++)
        d_Linha(8, x+(i*8), 0, 6, 97, 39, "█");
    d_Linha(8, 26, 0, 2, 97, 39, "█");

    for(int i = 0 ; i < 5 ; i++)
        d_Linha(9, x+(i*8), 0, 6, 97, 39, "█");

    // Logo SEEGA Sombra
    x = 23;
    for(int i = 0 ; i < 5 ; i++)
        d_Linha(4, x+(i*8), 0, 6, 90, 39, "▄");
    for(int i = 0 ; i < 5 ; i++)
        d_Linha(9, x+5+(i*8), 0, 1, 90, 39, "▀");

    for(int i = 0 ; i < 4 ; i++)
        d_Pixel(5, x+5+(i*8), 90, 39, "▀");
    d_Linha(5, 60, 4, 0, 90, 39, "█");

    for(int i = 0 ; i < 5 ; i++)
        d_Pixel(6, x+1 + (i*8), 90, 39, "█");
    d_Pixel(6, 28, 90, 39, "▄");
    d_Linha(7, 28, 2, 0, 90, 39, "█");
    d_Linha(6, 25, 0, 3, 90, 39, "▄");
    d_Linha(8, 23, 0, 3, 90, 39, "▄");
    for(int i = 1 ; i < 3 ; i++)
        d_Linha(6, x+2+(i*8), 0, 2, 90, 39, "▄");
    d_Pixel(7, 34, 90, 39, "▀");
    d_Pixel(7, 42, 90, 39, "▀");

    for(int i = 1 ; i < 5 ; i++)
        d_Pixel(8, x+1 + (i*8), 90, 39, "█");
    d_Linha(8, 33, 0, 4, 90, 39, "▄");
    d_Linha(8, 41, 0, 4, 90, 39, "▄");
    d_Linha(6, 50, 0, 3, 90, 39, "▄");
    d_Linha(7, 52, 2, 0, 90, 39, "█");
    d_Linha(8, 57, 2, 0, 39, 39, " ");
    d_Pixel(9, 56, 90, 39, "▀");
    d_Pixel(6, 57, 90, 39, "▄");
    d_Pixel(7, 48, 90, 39, "█");
    d_Pixel(8, 49, 90, 39, "▄");
}

void desenhaHistorico(){
    // Desenha borda da tela
    d_Linha(1, 1, 0, 80, 96, 30, "░");
    d_Linha(25, 1, 0, 80, 96, 30, "░");
    d_Retangulo_Preenchido(2, 1, 25, 2, 96, 30, "░");
    d_Retangulo_Preenchido(2, 79, 25, 80, 96, 30, "░");

    puts("\033[3;31H\033[97;49mHistórico de Partidas");
    d_Retangulo_Preenchido(5, 22, 23, 59, 37, 37, "█");
    d_Linha(24, 23, 0, 38, 90, 39, "▀");
    d_Linha(6, 60, 18, 0, 90, 39, "█");
    d_Pixel(5, 60, 90, 39, "▄");

    puts("\033[21;4H\033[97;49mPressione [ESC]");
    puts("\033[22;4H\033[97;49mpara retornar ao");
    puts("\033[23;4H\033[97;49mmenu principal");

    d_Pixel(7, 23, 30, 37, "╔");
    d_Linha(7, 24, 0, 34, 30, 37, "═");
    d_Pixel(7, 37, 30, 37, "╤");
    d_Pixel(7, 48, 30, 37, "╤");
    d_Pixel(7, 58, 30, 37, "╗");
    d_Linha(8, 58, 15, 0, 30, 37, "║");
    d_Pixel(9, 58, 30, 37, "╢");
    d_Pixel(20, 58, 30, 37, "╢");
    d_Pixel(23, 58, 30, 37, "╝");
    d_Linha(23, 24, 0, 34, 30, 37, "═");
    d_Pixel(23, 23, 30, 37, "╚");
    d_Linha(8, 23, 15, 0, 30, 37, "║");
    d_Pixel(9, 23, 30, 37, "╟");
    d_Pixel(20, 23, 30, 37, "╟");

    d_Linha(8, 37, 12, 0, 30, 37, "│");
    d_Linha(8, 48, 12, 0, 30, 37, "│");
    d_Linha(9, 24, 0, 34, 30, 37, "─");
    d_Linha(20, 24, 0, 34, 30, 37, "─");
    d_Pixel(9, 37, 30, 37, "┼");
    d_Pixel(9, 48, 30, 37, "┼");
    d_Pixel(20, 37, 30, 37, "┴");
    d_Pixel(20, 48, 30, 37, "┴");

    puts("\033[6;25H\033[30;47mTotal:");
    puts("\033[6;43H\033[30;47mPágina   de");

    puts("\033[8;26H\033[30;47mVencedor:");
    puts("\033[8;40H\033[30;47mTempo:");
    puts("\033[8;51H\033[30;47mModo:");

    getchar();
    printf("\033[0m");
    system(CLEAR);
    fflush(stdin);
    desenhaMenuPrincipal();
    menuPrincipal();
}

void menuPrincipal() {
    const char* opcoes[] = {
        "Novo Jogo", 
        "Carregar Jogo", 
        "Histórico", 
        "Ajuda", 
        "Sair"
    };
    const int numOpcoes = 5; // Número de itens no menu
    const int Ln = 14; // Linha de base para o texto do menu
    const int Col = 36; // Coluna base para o texto do menu
    int posicao = 0; // Opção selecionada

    while (1){

        // Imprime o menu
        for (int i = 0; i < numOpcoes; i++) {
            if (i == posicao) {
                // Acende a opção selecionada
                printf("\033[%d;%df\033[7m%s\033[0m", 
                       Ln + i, Col, opcoes[i]); // Ajuste para '>'
            } else {
                printf("\033[%d;%df\033[0m%s", Ln + i, Col, opcoes[i]);
            }
        }


        // Entrada do teclado
        int tecla = getch();
        if (tecla == 224) { // O código das setas vem depois de 224
            int seta = getch();
            if (seta == 72){
                // Seta pra cima
                posicao = (posicao - 1 + numOpcoes) % numOpcoes; // Loopa o menu
            } else if (seta == 80) {
                // Seta pra baixo
                posicao = (posicao + 1) % numOpcoes; // Loopa o menu
            }
        } else if (tecla == 13) {
            // Enter
            break;
        }
    }
    //printf("\033[%d;%df%s\n", Ln + numOpcoes + 2, Col - 10, opcoes[posicao]);
    switch (posicao){
    case 0:
        d_Retangulo_Preenchido(13, 34, 20, 49, 39, 39, " ");
        menuNovoJogo();
        break;
    case 1:
        break;
    case 2:
        system(CLEAR);
        desenhaHistorico();
        break;
    case 3:
        break;
    case 4:
        system(CLEAR);
        printf("\033[?25h\033[h\033[0m");
        printf("Obrigado por jogar!");
        exit(0);
        break;
    }
}

void menuNovoJogo(){
    const char *opcoes[] = {
        "Jogador Vs Jogador", 
        "Jogador Vs Computador",
        "Voltar"
    };
    const int numOpcoes = 3; // Número de itens no menu
    const int Ln = 14; // Linha de base para o texto do menu
    const int Col = 32; // Coluna base para o texto do menu
    int posicao = 0; // Opção selecionada

    while (1){

        // Imprime o menu
        for (int i = 0; i < numOpcoes; i++) {
            if (i == posicao) {
                // Acende a opção selecionada
                printf("\033[%d;%df\033[7m%s\033[0m", 
                       Ln + i, Col, opcoes[i]); // Ajuste para '>'
            } else {
                printf("\033[%d;%df\033[0m%s", Ln + i, Col, opcoes[i]);
            }
        }


        // Entrada do teclado
        int tecla = getch();
        if (tecla == 224) { // O código das setas vem depois de 224
            int seta = getch();
            if (seta == 72){
                // Seta pra cima
                posicao = (posicao - 1 + numOpcoes) % numOpcoes; // Loopa o menu
            } else if (seta == 80) {
                // Seta pra baixo
                posicao = (posicao + 1) % numOpcoes; // Loopa o menu
            }
        } else if (tecla == 13) {
            // Enter
            break;
        }
    }
    //printf("\033[%d;%df%s\n", Ln + numOpcoes + 2, Col - 10, opcoes[posicao]);
    switch (posicao){
    case 0:
        system(CLEAR);
        jogoLoop(1);
        break;
    case 1:
        system(CLEAR);
        jogoLoop(0);
        break;
    case 2:
        d_Retangulo_Preenchido(13, 31, 17, 56, 39, 39, " ");
        menuPrincipal();
        break;
    }
}
// Função Main, serve apenas para configurar o terminal e
// chamar as funções que iniciam o jogo de fato

int main(){
    srand(time(NULL));
    setlocale(LC_ALL, "pt_br.UTF-8");
    printf("\033[?25l");

    // Desenha o menu principal do jogo
    desenhaMenuPrincipal();
    menuPrincipal();

    //getchar();
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
        printf("\033[%d;%dH%s", H, x1, caractere); // Borda esquerda
        printf("\033[%d;%dH%s", H, x2, caractere); // Borda direita
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