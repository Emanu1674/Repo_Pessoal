#include <stdio.h>

int F_AbrirArquivo( const char* filename, const char* modo ) {
    FILE* file = fopen(filename, modo);
    if( file == NULL ) {
        perror("ERRO: Arquivo não existe ou está corrompido!");
    }
    return file;
}