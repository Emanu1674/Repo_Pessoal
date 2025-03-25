#include <stdio.h>
#include <stdlib.h>
#include "f_arqs.h"

typedef struct {
    // Nó da árvore huffman
    char caractere;
    int frequencia;
    int codigo[8];
} HuffmanNode;

// Conta quantos de cada caractere tem em texto.txt
I_ContaCaracteres() {

    FILE* lista = F_AbrirArquivo("lista.txt", "r");
    FILE* arq = F_AbrirArquivo("texto.txt", "r");

    for(int i = 0 ; );
    return 0;
}