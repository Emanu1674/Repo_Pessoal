#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define S_DECK 52

typedef struct{
    int valor; // 0-12 (Ás -> Rei)
    int naipe; // 0-3 (Paus, ouros, copas e espadas)
} Carta;

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
void imprimirDeckComoNumeros(Carta *deck) {
    for (int i = 0; i < S_DECK; i++) {
        int cartaNumero = deck[i].valor + 1 + (deck[i].naipe * 13); // Converte valor/naipe para 1-52
        printf("%02d ", cartaNumero);
        if ((i + 1) % 13 == 0) printf("\n");
    }
}

// Inicializa o deck de cartas, com todas as cartas e naipes em ordem de A até Rei
void inicializaDeck(Carta* deck){
    for(int i = 0 ; i < S_DECK ; i++){
        deck[i].valor = i % 13; // 0 a 12 (Ás -> Rei)
        deck[i].naipe = i / 13; // 0 a 3 (Paus, ouros, copas e espadas)
    }
}

int main(){

    Carta deck[S_DECK];

    inicializaDeck(deck);

    printf("\nDeck original:\n");
    imprimirDeck(deck);
    printf("\nDeck original (Números):\n");
    imprimirDeckComoNumeros(deck);

    // Embaralhar deck
    embaralhaCartas(deck, S_DECK);

    printf("\nDeck Embaralhado:\n");
    imprimirDeck(deck);
    printf("\nDeck embaralhado (Números):\n");
    imprimirDeckComoNumeros(deck);

    printf("\nCarta na posição 5:\n");
    imprimirCartaComoNumero(deck, 5);

    //printf("%c%s", baralhoNomes[11], baralhoNaipes[3]);
    //printf("\n🂡");

    return 0;
}