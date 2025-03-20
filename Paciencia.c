#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define S_DECK 52

typedef struct{
    int valor; // 0-12 (Ás -> Rei)
    int naipe; // 0-3 (Paus, ouros, copas e espadas)
    bool vazio; // identifica se a carta existe ou se é uma posição vazia. 1 = vazio, 0 = ocupada
    bool virada;
} Carta;

typedef struct{
    Carta pilha[7][13];
    Carta pilhaEstoque[24];
    Carta pilhaDescarte[24];
    Carta pilhaSaida[4][13];
    Carta gabarito[4][13];
} Mesa;

void inicializaDeck(Carta* deck);
void embaralhaCartas(Carta* deck, int size);
void jogoLoop(Mesa* jogo);
void imprimeEstoque(Mesa* jogo);
void imprimeJogo(Mesa* jogo);

void imprimirCarta(Carta deck);
void imprimirDeck(Carta* deck);
void imprimirCartaComoNumero(Carta* deck, int Indice);
void imprimirDeckComoNumeros(Carta* deck);

// Imprime uma carta específica
void imprimirCarta(Carta deck){
    // Vetores usados apenas para imprimir as cartas na tela
    char* deckNomes[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    char* deckNaipes[4] = {u8"♣", u8"♦", u8"♥", u8"♠"}; // Paus, ouros, copas e espadas

    printf("[%s%s] ", deckNomes[deck.valor], deckNaipes[deck.naipe]);
}

// Imprime uma carta específica, mas como um número
void imprimirCartaComoNumero(Carta* deck, int Indice){
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
void imprimirDeckComoNumeros(Carta* deck){
    for (int i = 0; i < S_DECK; i++) {
        int cartaNumero = deck[i].valor + 1 + (deck[i].naipe * 13); // Converte valor/naipe para 1-52
        printf("%02d ", cartaNumero);
        if ((i + 1) % 13 == 0) printf("\n");
    }
}

// Inicializa o deck de cartas, com todas as cartas e naipes em ordem de A até Rei
void inicializaDeck(Carta* deck){
    for(int i = 0 ; i < S_DECK ; i++){
        deck[i].valor = i % 13; // 0 - 13 (Ás -> Rei)
        deck[i].naipe = i / 13; // 0 - 3 (Paus, ouros, copas e espadas)
        deck[i].vazio = 0;       // posição é uma carta válida
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

void gerarFilas(Carta* deck, Carta* deckNovo){
    // Struct com as pilhas de jogo
    static Mesa jogo;
    int deckIndice = 0;

    // Inicializa pilhas como vazias primeiro
    for (int i = 0; i < 7; i++){
        for (int j = 0; j < 13; j++){
            jogo.pilha[i][j].vazio = 1; // Posição vazia
        }
    }

    // Inicializa pilha de descarte como vazia
    for (int i = 0; i < 24; i++){
        jogo.pilhaDescarte[i].vazio = 1; // Posição vazia
    }

    // Inicializa o gabarito do jogo como cheio
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 13; j++){
            jogo.gabarito[i][j].vazio = 0; // Posição ocupada
        }
    }

    // Prepara o gabarito do jogo
    int k = 0;
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 13; j++){
            jogo.gabarito[i][j].valor = deckNovo[k].valor; // Copia valor de deckNovo
            jogo.gabarito[i][j].naipe = deckNovo[k].naipe; // Copia naipe de deckNovo
            k++;
        }
    }

    // Distribui cartas uma por uma
    int cartasColocadas = 0;
    int volta = 0;
    while (cartasColocadas < 28){  // Primeiras 28 cartas são distribuidas
        for (int i = volta; i < 7 && cartasColocadas < 28; i++) {  
            jogo.pilha[i][volta] = deck[deckIndice++];
            jogo.pilha[i][volta].vazio = 0; // Essa posição tem uma carta
            cartasColocadas++;
        }
        volta++;
    }

    // Coloca as 24 cartas que sobram na pilha de estoque
    for (int i = 0; i < 24; i++) {
        jogo.pilhaEstoque[i] = deck[deckIndice++];
        jogo.pilhaEstoque[i].vazio = 0; // Pilha de estoque cheia no começo
    }

    // Inicia o Loop do jogo
    jogoLoop(&jogo);
}

void jogoLoop(Mesa* jogo){


// Imprime o jogo
    imprimeJogo(jogo);
    
// Imprime pilha de Estoque
    imprimeEstoque(jogo);
    printf("\n");

// [DEBUG] Imprime gabarito
    for (int i = 0; i < 4; i++){
        printf("\n");
        for (int j = 0; j < 13; j++){
            imprimirCarta(jogo->gabarito[i][j]);
        }
    }

    // TODO: imprimir pilha de descarte
    // CODE
    
    // TODO: imprimir pilha de saída
    // CODE

    // TODO: Lógica de jogo
    // CODE
    // {REPETIR ATÉ O JOGO ACABAR OU SEM MOVIMENTOS POSSÍVEIS}

    // TODO: Imprimir lista de movimentos
    // imprimir "FIM" na tela

    // Fim do jogo
}

void imprimeEstoque(Mesa* jogo) {
    printf("\nPilha de Estoque: ");

    // Encontra a última carta na lista
    int lastIndex = -1;
    for (int i = 23; i >= 0; i--) {
        if (!jogo->pilhaEstoque[i].vazio) {
            lastIndex = i;
            break;
        }
    }

    // Se não tem mais cartas, imprime uma pilha vazia
    if (lastIndex == -1) {
        printf("[Vazio]\n");
        return;
    }

    // Imprime "..." se tem mais de 4 cartas
    if (lastIndex > 3) {
        printf("... ");
    }

    // Imprime as últimas 4 cartas (se faltam 4 cartas ou menos)
    int start = lastIndex - 3 < 0 ? 0 : lastIndex - 3;
    for (int i = start; i <= lastIndex; i++) {
        imprimirCarta(jogo->pilhaEstoque[i]);
    }

    printf("\n");
}

void imprimeJogo(Mesa* jogo){
    printf("\033[s"); // Salva posição do cursor (código escape ANSI)

    // Imprime as 7 pilhas

    for (int i = 0; i < 7; i++){
        printf("Pilha de jogo %d: ", i + 1);
        for (int j = 0; j <= i; j++) { // Só imprime o número correto de cartas por pilha
            if (!jogo->pilha[i][j].vazio) { 
                imprimirCarta(jogo->pilha[i][j]);
            }
        }
        printf("\033[1E"); // Move cursor 1 linha para baixo (ANSI escape)
    }
}


int main(){
// Structs com as cartas do jogo
    static Carta deckNovo[S_DECK];
    Carta deck[S_DECK];

// Inicializa deck com 1-52
    inicializaDeck(deck);
    inicializaDeck(deckNovo);

// [DEBUG] Imprimir deck na tela
    //printf("\nDeck original:\n");
    //imprimirDeck(deck);
    //printf("\nDeck original (Números):\n");
    //imprimirDeckComoNumeros(deck);

// Embaralhar deck
    embaralhaCartas(deck, S_DECK);

// [DEBUG] Imprimir deck na tela
    //printf("\nDeck Embaralhado:\n");
    //imprimirDeck(deck);
    //printf("\n");
    //printf("\nDeck embaralhado (Números):\n");
    //imprimirDeckComoNumeros(deck);
    //printf("\nCarta na posição 6:\n");
    //imprimirCartaComoNumero(deck, 5);

    gerarFilas(deck, deckNovo);

    return 0;
}