#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define S_DECK 52

typedef struct{
    int valor; // 0-13 (Ás -> Rei)
    int naipe; // 0-3 (Paus, ouros, copas e espadas)
} Carta;

typedef struct{
    Carta pilha1[12];
    Carta pilha2[12];
    Carta pilha3[12];
    Carta pilha4[12];
    Carta pilha5[12];
    Carta pilha6[12];
    Carta pilha7[12];
} Mesa;

void inicializaDeck(Carta* deck);
void embaralhaCartas(Carta* deck, int size);
void jogoLoop(Carta* deck, Mesa* jogo);
void imprimeJogo(Carta* deck, Mesa* jogo);

void imprimirCarta(Carta deck);
void imprimirDeck(Carta* deck);
void imprimirCartaComoNumero(Carta* deck, int Indice);
void imprimirDeckComoNumeros(Carta* deck);

// Imprime uma carta específica
void imprimirCarta(Carta deck) {
    // Vetores usados apenas para imprimir as cartas na tela
    char* deckNomes[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    char* deckNaipes[4] = {u8"♣", u8"♦", u8"♥", u8"♠"}; // Paus, ouros, copas e espadas

    printf("[%s%s] ", deckNomes[deck.valor], deckNaipes[deck.naipe]);
}

// Imprime uma carta específica, mas como um número
void imprimirCartaComoNumero(Carta* deck, int Indice) {
    int cartaNumero = deck[Indice].valor + 1 + (deck[Indice].naipe * 13); // Converte valor/naipe para 1-52
    printf("%02d ", cartaNumero);
    if ((Indice + 1) % 13 == 0) printf("\n");
}

// Imprime o deck inteiro, em 4 colunas separadas por naipe
void imprimirDeck(Carta* deck){
    for(int i = 0 ; i < S_DECK ; i++){
        imprimirCarta(deck[i]);
        if((i + 1) % 13 == 0) printf("\n");
    }
}

// Imprime o deck inteiro, mas como números
void imprimirDeckComoNumeros(Carta* deck) {
    for (int i = 0; i < S_DECK; i++) {
        int cartaNumero = deck[i].valor + 1 + (deck[i].naipe * 13); // Converte valor/naipe para 1-52
        printf("%02d ", cartaNumero);
        if ((i + 1) % 13 == 0) printf("\n");
    }
}

// Inicializa o deck de cartas, com todas as cartas e naipes em ordem de A até Rei
void inicializaDeck(Carta* deck){
    for(int i = 0 ; i < S_DECK ; i++){
        deck[i].valor = i % 13; // 0 a 13 (Ás -> Rei)
        deck[i].naipe = i / 13; // 0 a 3 (Paus, ouros, copas e espadas)
    }
}

// Embaralha o deck de cartas
void embaralhaCartas(Carta* deck, int size){
    // Seed para RNG
    srand(time(NULL));

    // Algoritmo de Fisher-Yates
    for(int i = size - 1 ; i > 0 ; i--){
        // índice aleatório de 0 a i
        int j = rand() % (i + 1);

        // Permuta deck[i] e deck[j]
        Carta temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

void gerarFilas(Carta* deck){
    // Struct com as pilhas de jogo
    static Mesa jogo;

    jogo.pilha1[0] = deck[0];
    jogo.pilha2[0] = deck[1];
    jogo.pilha3[0] = deck[2];
    jogo.pilha4[0] = deck[3];
    jogo.pilha5[0] = deck[4];
    jogo.pilha6[0] = deck[5];
    jogo.pilha7[0] = deck[6];

    jogo.pilha2[1] = deck[7];
    jogo.pilha3[1] = deck[8];
    jogo.pilha4[1] = deck[9];
    jogo.pilha5[1] = deck[10];
    jogo.pilha6[1] = deck[11];
    jogo.pilha7[1] = deck[12];

    jogo.pilha3[2] = deck[13];
    jogo.pilha4[2] = deck[14];
    jogo.pilha5[2] = deck[15];
    jogo.pilha6[2] = deck[16];
    jogo.pilha7[2] = deck[17];

    jogo.pilha4[3] = deck[18];
    jogo.pilha5[3] = deck[19];
    jogo.pilha6[3] = deck[20];
    jogo.pilha7[3] = deck[21];

    jogo.pilha5[4] = deck[22];
    jogo.pilha6[4] = deck[23];
    jogo.pilha7[4] = deck[24];

    jogo.pilha6[5] = deck[25];
    jogo.pilha7[5] = deck[26];

    jogo.pilha7[6] = deck[27];

    jogoLoop(deck, &jogo);
}

void jogoLoop(Carta* deck, Mesa* jogo){
    imprimeJogo(deck, jogo);
}

void imprimeJogo(Carta* deck, Mesa* jogo){
    printf("\033[s");
    printf("Pilha de jogo 1: ");
    for(int i = 0 ; i < 1 ; i++){
        imprimirCarta(jogo->pilha1[i]);
    }
    printf("\033[1E");
    printf("Pilha de jogo 2: ");
    for(int i = 0 ; i < 2 ; i++){
        imprimirCarta(jogo->pilha2[i]);
    }
    printf("\033[1E");
    printf("Pilha de jogo 3: ");
    for(int i = 0 ; i < 3 ; i++){
        imprimirCarta(jogo->pilha3[i]);
    }
    printf("\033[1E");
    printf("Pilha de jogo 4: ");
    for(int i = 0 ; i < 4 ; i++){
        imprimirCarta(jogo->pilha4[i]);
    }
    printf("\033[1E");
    printf("Pilha de jogo 5: ");
    for(int i = 0 ; i < 5 ; i++){
        imprimirCarta(jogo->pilha5[i]);
    }
    printf("\033[1E");
    printf("Pilha de jogo 6: ");
    for(int i = 0 ; i < 6 ; i++){
        imprimirCarta(jogo->pilha6[i]);
    }
    printf("\033[1E");
    printf("Pilha de jogo 7: ");
    for(int i = 0 ; i < 7 ; i++){
        imprimirCarta(jogo->pilha7[i]);
    }
}


int main(){
    // Struct com as cartas do jogo
    static Carta deck[S_DECK];

    // Inicializa deck com 1-52
    inicializaDeck(deck);

    // Imprimir deck na tela (debug)
    //printf("\nDeck original:\n");
    //imprimirDeck(deck);
    //printf("\nDeck original (Números):\n");
    //imprimirDeckComoNumeros(deck);

    // Embaralhar deck
    embaralhaCartas(deck, S_DECK);

    // Imprimir deck na tela (debug)
    //printf("\nDeck Embaralhado:\n");
    //imprimirDeck(deck);
    //printf("\n");
    //printf("\nDeck embaralhado (Números):\n");
    //imprimirDeckComoNumeros(deck);
    //printf("\nCarta na posição 6:\n");
    //imprimirCartaComoNumero(deck, 5);

    gerarFilas(deck);

    return 0;
}