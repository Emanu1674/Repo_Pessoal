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
#define COL 18
#define LN 6

typedef struct{
    int hora, min, seg;
} horario;

typedef struct{
    int tabuleiro[5][5];
    short pecas;
    int jogador;
    long tempoJogo;
} jogo;

void d_preenche_Tela(int _FGColor, int _corFundo, char* _Char);
void d_Retangulo(int _xPos1, int _yPos1, int _xPos2, int _yPos2, int _FGColor, int _corFundo, char* _Char);
void d_Retangulo_Preenchido(int _xPos1, int _yPos1, int _xPos2, int _yPos2, int _FGColor, int _corFundo, char* _Char);
void d_Linha(int _xPos, int _yPos, int _xSize, int _ySize, int _FGColor, int _corFundo, char* _Char);
void d_Pixel(int _xPos, int _yPos, int _FGColor, int _corFundo, char* _Char);

void desenhaMenuPrincipal();
void desenhaHistorico();
void desenhaMenu();
void desenhaTabuleiro(jogo *_partida);

void MenuPrincipal();
void menuNovoJogo();
void jogoLoop(int modo);
int moverCursor(int* _rX, int* _rY, jogo *_partida);
bool posicao_Valida(int X, int Y, jogo* partida);

void desenhaMenu(){
d_Retangulo_Preenchido(62, 1, 79, 2, 35, 35, "█");
    d_Retangulo_Preenchido(62, 3, 79, 24, 94, 94, "█");
    d_Linha(80, 1, 0, 2, 35, 30, "░");
    d_Linha(62, 25, 19, 0, 94, 34, "░");
    d_Linha(80, 3, 0, 22, 94, 34, "░");
}

void desenhaTabuleiro(jogo *partida){
    d_Retangulo_Preenchido(16, 5, 47, 20, 96, 30, "░");
    for(int i = 0 ; i <= 4 ; i++){
        for(int j = 0 ; j <= 4 ; j++){
            if(partida->tabuleiro[j][i] == 0)
                d_Retangulo(COL + (i*6), LN + (j*3), COL+3 + (i*6), LN+1 + (j*3), 30, 30, "█");
            else if(partida->tabuleiro[j][i] == 1)
                d_Retangulo(COL + (i*6), LN + (j*3), COL+3 + (i*6), LN+1 + (j*3), 34, 30, "█");
            else if(partida->tabuleiro[j][i] == 2)
                d_Retangulo(COL + (i*6), LN + (j*3), COL+3 + (i*6), LN+1 + (j*3), 31, 30, "█");
        }
    }
}

int moverCursor(int* rX, int* rY, jogo *partida){
    int tabuleiro[5][5];
    int Y = 2;
    int X = 2;
    int cor = 0;
    int tecla = 0;
    while(tecla != 13){     // 13 = Enter

        if(partida->tabuleiro[X][Y] == 0)
            cor = 30;
        else if(partida->tabuleiro[X][Y] == 2)
            cor = 34;
        else if(partida->tabuleiro[X][Y] == 1)
            cor = 31;

        d_Retangulo_Preenchido(COL + X*6, LN + Y*3, COL+3 + X*6, LN+1 + Y*3, cor+60, cor+60, "█");
        tecla = getch();
        if(tecla == 0 || tecla == 224) {
            tecla = getch();
            switch (tecla) {
                case 72:
                    d_Retangulo_Preenchido(COL + X*6, LN + Y*3, COL+3 + X*6, LN+1 + Y*3, cor, cor, "█");
                    Y -= 1;
                    if(Y < 0)
                        Y = 4;
                break;
                case 80:
                    d_Retangulo_Preenchido(COL + X*6, LN + Y*3, COL+3 + X*6, LN+1 + Y*3, cor, cor, "█");
                    Y += 1;
                    if(Y > 4)
                        Y = 0;
                break;
                case 75:
                    d_Retangulo_Preenchido(COL + X*6, LN + Y*3, COL+3 + X*6, LN+1 + Y*3, cor, cor, "█");
                    X -= 1;
                    if(X < 0)
                        X = 4;
                break;
                case 77:
                    d_Retangulo_Preenchido(COL + X*6, LN + Y*3, COL+3 + X*6, LN+1 + Y*3, cor, cor, "█");
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

void CPUCriaPeca(int* pX, int* pY, jogo* partida) {
    int X, Y;
    int attempts = 0;  // Total de tentativas feitas
    const int maxAttempts = 25;  // Máximo de tentativas falhas

    // Seleção aleatória com fallback em caso de loop infinito
    while (attempts < maxAttempts) {
        X = (rand() % 5);  // Coordenada X Aleatória (0-4)
        Y = (rand() % 5);  // Coordenada Y Aleatória (0-4)

        if (partida->tabuleiro[X][Y] == 0 && !(X == 2 && Y == 2)) {
            *pX = X;
            *pY = Y;
            return;  // Posição válida encontrada
        }
        attempts++;
    }

    // Pesquisa de força bruta caso a função fique presa
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (partida->tabuleiro[i][j] == 0) {
                *pX = i;
                *pY = j;
                return;  // Posição válida encontrada
            }
        }
    }
}

void fase2(jogo* partida){
    desenhaTabuleiro(partida);
    gets("\033[3;1H\033[2K");
    printf("\033[1;65H\033[97;45HJogando:");
    printf("\033[2;65H\033[97;45HJogador %d", partida->jogador);
    getchar();
}

void fase1(jogo* partida) {
    int pecasJogador1 = 1, pecasJogador2 = 1;
    int X, Y;
    
    printf("\033[6;52HPeças em");
    printf("\033[7;54HJogo:");
    d_Linha(52, 9, 2, 0, 31, 31, "█");
    printf(" = 0");
    d_Linha(52, 11, 2, 0, 34, 34, "█");
    printf(" = 0");
    printf("\033[0m");

    for (int etapa = 0; etapa < 6; etapa++) {
        for (int jogador = 1; jogador < 3; jogador++) {
            for (int peca = 0; peca < 3; peca++) {
                printf("\033[3;19HJogador %d, adicione 2 peças", jogador);
                moverCursor(&X, &Y, partida);
                
                if (X == 2 && Y == 2) {  // Centro é proibido
                    printf("\033[25;24HPeça proibida!");
                    continue;
                }

                if (partida->tabuleiro[X][Y] == 0) {  // Se Posição vazia
                    // Atualizar tabuleiro
                    partida->tabuleiro[X][Y] = jogador;  // Jogador 1 ou 2
                    int cor = (jogador == 1) ? 31 : 34;  // Cor para jogador 1 ou 2
                    d_Retangulo_Preenchido(COL + X*6, LN + Y*3, COL + 3 + X*6, LN + 1 + Y*3, cor, cor, "█");

                    // Atualizar contagem de peças
                    if (jogador == 1) {
                        d_Linha(52, 9, 2, 0, 31, 31, "█");
                        printf(" = %d", pecasJogador1++);
                    } else {
                        d_Linha(52, 11, 2, 0, 34, 34, "█");
                        printf(" = %d", pecasJogador2++);
                    }
                    peca++;
                }
            }
        }
    }
    fase2(partida);
}

void fase1CPU(jogo* partida) {
    int X, Y; // Variáveis de posição
    int jogador = 1; // Jogador 1 começa
    int pecasPorJogador = 12; // Cada jogador começa com 12 peças
    int pecasJogador1 = 0, pecasComputador = 0; // Contador de peças jogadas

    printf("\033[6;52HPeças em");
    printf("\033[7;54HJogo:");
    d_Linha(52, 9, 2, 0, 31, 31, "█");
    printf(" = 0");
    d_Linha(52, 11, 2, 0, 34, 34, "█");
    printf(" = 0");
    printf("\033[0m");

    while (pecasJogador1 < pecasPorJogador || pecasComputador < pecasPorJogador) {
        for (int turn = 0; turn < 2; turn++) { // Cada jogador tem dois turnos por round
            if (jogador == 1) { // Turno do Jogador 1
                bool movimento_Valido = false;
                while (!movimento_Valido) {
                    moverCursor(&X, &Y, partida); // Entrada de input do Jogador
                    if (posicao_Valida(X, Y, partida)) { // Verifica se posição é válida
                        partida->tabuleiro[X][Y] = 1; // Atualiza o tabuleiro
                        d_Retangulo_Preenchido(COL + X * 6, LN + Y * 3, COL + 3 + X * 6, LN + 1 + Y * 3, 31, 31, "█"); // Desenha quadrado vermelho
                        d_Linha(52, 9, 2, 0, 31, 31, "█");
                        printf(" = %d", pecasJogador1+1);
                        pecasJogador1++;
                        movimento_Valido = true;
                    } else {
                        printf("\033[25;24HMovimento Inválido!.");
                    }
                }
            } else { // Vez da CPU
                bool movimento_Valido = false;
                while (!movimento_Valido) {
                    CPUCriaPeca(&X, &Y, partida); // CPU seleciona posição
                    if (posicao_Valida(X, Y, partida)) { // Valida a escolha da CPU
                        partida->tabuleiro[X][Y] = 2; // Atualiza o tabuleiro
                        d_Retangulo_Preenchido(COL + X * 6, LN + Y * 3, COL + 3 + X * 6, LN + 1 + Y * 3, 34, 34, "█"); // Desenha quadrado azul
                        d_Linha(52, 11, 2, 0, 34, 34, "█");
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

    printf("\033[10;10HBoard setup complete. Starting the next phase...");
    getchar();
}

bool posicao_Valida(int X, int Y, jogo* partida) {
    return X >= 0 && X < 5 && Y >= 0 && Y < 5 && partida->tabuleiro[X][Y] == 0 && !(X == 2 && Y == 2);
}

void jogoLoop(int modo){
    int pecasTotal = 24;
    int pecasColocadas = 0;
    int jogador = 1;

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
        .jogador = 0,
        .tempoJogo = 0
    };

    // Desenha o Menu da partida
    desenhaMenu();

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
    d_Linha(1, 1, 80, 0, 96, 30, "░");
    d_Linha(1, 25, 80, 0, 96, 30, "░");
    d_Retangulo_Preenchido(1, 2, 2, 25, 96, 30, "░");
    d_Retangulo_Preenchido(79, 2, 80, 25, 96, 30, "░");
    
    // Desenhos em volta da logo
    for(int i = 0 ; i < 5 ; i++){  
        cor = i % 2 == 0 ? 34 : 31;
        d_Linha(x + (i*8), 3, 6, 0, cor, 39, "▄");
    }
    for(int i = 0 ; i < 5 ; i++){  
        cor = i % 2 == 0 ? 31 : 34;
        d_Linha(x + (i*8), 10, 6, 0, cor, 39, "▄");
    }

    d_Linha(4, 6, 8, 0, 31, 39, "░");
    d_Linha(12, 6, 8, 0, 32, 39, "░");
    d_Linha(4, 8, 8, 0, 35, 39, "░");
    d_Linha(12, 8, 8, 0, 93, 39, "░");

    d_Linha(62, 6, 8, 0, 93, 39, "░");
    d_Linha(70, 6, 8, 0, 35, 39, "░");
    d_Linha(62, 8, 8, 0, 32, 39, "░");
    d_Linha(70, 8, 8, 0, 31, 39, "░");

    // Logo SEEGA
    for(int i = 0 ; i < 5 ; i++)
        d_Linha(x+(i*8), 5, 6, 0, 97, 39, "█");
    for(int i = 0 ; i < 5 ; i++)
        d_Linha(x+(i*8), 6, 2, 0, 97, 39, "█");
    d_Linha(58, 6, 2, 0, 97, 39, "█");
    for(int i = 1 ; i < 5 ; i++)
        d_Linha(x+(i*8), 7, 4, 0, 97, 39, "█");
    d_Linha(22, 7, 6, 0, 97, 39, "█");
    d_Linha(50, 7, 2, 0, 97, 39, "█");
    d_Linha(58, 7, 2, 0, 97, 39, "█");

    for(int i = 1 ; i < 5 ; i++)
        d_Linha(x+(i*8), 8, 6, 0, 97, 39, "█");
    d_Linha(26, 8, 2, 0, 97, 39, "█");

    for(int i = 0 ; i < 5 ; i++)
        d_Linha(x+(i*8), 9, 6, 0, 97, 39, "█");

    // Logo SEEGA Sombra
    x = 23;
    for(int i = 0 ; i < 5 ; i++)
        d_Linha(x+(i*8), 4, 6, 0, 90, 39, "▄");
    for(int i = 0 ; i < 5 ; i++)
        d_Linha(x+5+(i*8), 9, 1, 0, 90, 39, "▀");

    for(int i = 0 ; i < 4 ; i++)
        d_Pixel(x+5+(i*8), 5, 90, 39, "▀");
    d_Linha(60, 5, 0, 4, 90, 39, "█");

    for(int i = 0 ; i < 5 ; i++)
        d_Pixel(x+1 + (i*8), 6, 90, 39, "█");
    d_Pixel(28, 6, 90, 39, "▄");
    d_Linha(28, 7, 0, 2, 90, 39, "█");
    d_Linha(25, 6, 3, 0, 90, 39, "▄");
    d_Linha(23, 8, 3, 0, 90, 39, "▄");
    for(int i = 1 ; i < 3 ; i++)
        d_Linha(x+2+(i*8), 6, 2, 0, 90, 39, "▄");
    d_Pixel(34, 7, 90, 39, "▀");
    d_Pixel(42, 7, 90, 39, "▀");

    for(int i = 1 ; i < 5 ; i++)
        d_Pixel(x+1 + (i*8), 8, 90, 39, "█");
    d_Linha(33, 8, 4, 0, 90, 39, "▄");
    d_Linha(41, 8, 4, 0, 90, 39, "▄");
    d_Linha(50, 6, 3, 0, 90, 39, "▄");
    d_Linha(52, 7, 0, 2, 90, 39, "█");
    d_Linha(57, 8, 0, 2, 39, 39, " ");
    d_Pixel(56, 9, 90, 39, "▀");
    d_Pixel(57, 6, 90, 39, "▄");
    d_Pixel(48, 7, 90, 39, "█");
    d_Pixel(49, 8, 90, 39, "▄");
}

void desenhaHistorico(){
    // Desenha borda da tela
    d_Linha(1, 1, 80, 0, 96, 30, "░");
    d_Linha(1, 25, 80, 0, 96, 30, "░");
    d_Retangulo_Preenchido(1, 2, 2, 25, 96, 30, "░");
    d_Retangulo_Preenchido(79, 2, 80, 25, 96, 30, "░");

    puts("\033[3;31H\033[97;49mHistórico de Partidas");
    d_Retangulo_Preenchido(22, 5, 59, 23, 37, 37, "█");
    d_Linha(23, 24, 38, 0, 90, 39, "▀");
    d_Linha(60, 6, 0, 18, 90, 39, "█");
    d_Pixel(60, 5, 90, 39, "▄");

    puts("\033[21;4H\033[97;49mPressione [ESC]");
    puts("\033[22;4H\033[97;49mpara retornar ao");
    puts("\033[23;4H\033[97;49mmenu principal");

    d_Pixel(23, 7, 30, 37, "╔");
    d_Linha(24, 7, 34, 0, 30, 37, "═");
    d_Pixel(37, 7, 30, 37, "╤");
    d_Pixel(48, 7, 30, 37, "╤");
    d_Pixel(58, 7, 30, 37, "╗");
    d_Linha(58, 8, 0, 15, 30, 37, "║");
    d_Pixel(58, 9, 30, 37, "╢");
    d_Pixel(58, 20, 30, 37, "╢");
    d_Pixel(58, 23, 30, 37, "╝");
    d_Linha(24, 23, 34, 0, 30, 37, "═");
    d_Pixel(23, 23, 30, 37, "╚");
    d_Linha(23, 8, 0, 15, 30, 37, "║");
    d_Pixel(23, 9, 30, 37, "╟");
    d_Pixel(23, 20, 30, 37, "╟");

    d_Linha(37, 8, 0, 12, 30, 37, "│");
    d_Linha(48, 8, 0, 12, 30, 37, "│");
    d_Linha(24, 9, 34, 0, 30, 37, "─");
    d_Linha(24, 20, 34, 0, 30, 37, "─");
    d_Pixel(37, 9, 30, 37, "┼");
    d_Pixel(48, 9, 30, 37, "┼");
    d_Pixel(37, 20, 30, 37, "┴");
    d_Pixel(48, 20, 30, 37, "┴");

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
    MenuPrincipal();
}

void MenuPrincipal() {
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
        d_Retangulo_Preenchido(34, 13, 49, 20, 39, 39, " ");
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
        d_Retangulo_Preenchido(31, 13, 56, 17, 39, 39, " ");
        MenuPrincipal();
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
    MenuPrincipal();

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