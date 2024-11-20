#include <stdio.h>
#include <unistd.h>
#include <time.h>

typedef unsigned char u8;
typedef struct{
    u8 hora, min, seg;
} horario;

typedef struct{
    int tabuleiro[5][5];
    short pecas;
    long tempoJogo;
} jogo;

void carregarJogo(jogo *carrega){
    FILE *salvo = fopen("save.bin", "rb");
    fread(carrega, sizeof(jogo), 1, salvo);
    fclose(salvo);

    for(int i = 0 ; i < 5 ; i++){
        printf("\n");
        for(int j = 0 ; j < 5 ; j++){
            printf("%d, ", carrega->tabuleiro[i][j]);
        }
    }
    printf("\nTempo de Jogo: %d segundos", carrega->tempoJogo);
}

void converteTempo(horario *duracao, long tempoJogo){
    duracao->hora = tempoJogo / (60 * 60);
    tempoJogo %= (60 * 60);
    duracao->min = tempoJogo / (60);
    tempoJogo %= (60);
    duracao->seg = tempoJogo;

    printf("\nTempo de jogo: %.2d:%.2d:%.2d", duracao->hora, duracao->min, duracao->seg);
}

int main(){
    short memoria = 0;

    // Armazenar valores nos bytes
    horario duracao = {0,0,0};
    time_t t_inicio = time(NULL);
    jogo carrega = {
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

    jogo partida = {
        .tabuleiro = {
            {1, 0, 2, 0, 1},
            {0, 1, 0, 2, 0},
            {0, 0, 1, 0, 0},
            {2, 1, 0, 1, 0},
            {2, 0, 0, 2, 1}
        },
        .pecas = 0,
        .tempoJogo = 0
    };

    sleep(62);
    partida.tempoJogo = time(NULL) - t_inicio;
    //printf("%d\n%d\n", time(NULL), t_inicio);

    FILE *salvo = fopen("save.bin", "wb");
    if (!salvo) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }
    puts("\nSalvando...\n");
    fwrite(&partida, sizeof(jogo), 1, salvo);
    fclose(salvo);

    
    converteTempo(&duracao, partida.tempoJogo);

    carregarJogo(&carrega);
    return 0;
}

    /*
    partida.pecas = (partida.pecas & 0x00FF) | (79 << 8);   // Armazena no byte mais significativo
    partida.pecas = (partida.pecas & 0xFF00) | (211);       // Armazena no byte menos significativo

    u8 a = (partida.pecas >> 8) & 0xFF;               // Lê o byte mais significativo
    u8 b = partida.pecas & 0xFF;                      // Lê o byte menos significativo
    // Escreve 71 na tela
    printf("%d - %d", a, b);
    */