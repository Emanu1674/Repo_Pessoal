#include <stdio.h>

void COLOR_F(int c){
    switch(c){
        case 0: printf("\033[30m"); break;  // Preto
        case 1: printf("\033[34m"); break;  // Azul
        case 2: printf("\033[32m"); break;  // Verde
        case 3: printf("\033[36m"); break;  // Ciano
        case 4: printf("\033[31m"); break;  // Vermelho
        case 5: printf("\033[35m"); break;  // Magenta
        case 6: printf("\033[33m"); break;  // Marrom
        case 7: printf("\033[37m"); break;  // Cinza
        case 8: printf("\033[1;30m"); break;    // Cinza escuro
        case 9: printf("\033[1;34m"); break;    // Azul claro
        case 10: printf("\033[1;32m"); break;   // Verde claro
        case 11: printf("\033[1;36m"); break;   // Ciano claro
        case 12: printf("\033[1;31m"); break;   // Vermelho claro
        case 13: printf("\033[1;35m"); break;   // Magenta claro
        case 14: printf("\033[1;33m"); break;   // Amarelo
        case 15: printf("\033[1;37m"); break;   // Branco
    }
}