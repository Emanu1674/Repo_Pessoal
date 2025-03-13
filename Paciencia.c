#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void embaralhar(int* bar, size_t n){

}

void imprimirCartas(){
    
}

int main(){
    srand(time(NULL));

    int baralho[52];
    char baralhoNomes[14] = {'1','2','3','4','5','6','7','8','9','A','J','Q','K'};
    char* baralhoNaipes[4] = {u8"♣", u8"♦", u8"♥", u8"♠"}; // Paus, ouros, copas e espadas

    for(int i = 0 ; i <= 52 ; i++){
        baralho[i] = i+1;
    }

    //embaralhar(baralho, 52);

    for(int i = 0 ; i < 52 ; i++){
        printf("%d ", baralho[i]);
    }

    //printf("%c%s", baralhoNomes[11], baralhoNaipes[3]);
    //printf("\n🂡");

    return 0;
}