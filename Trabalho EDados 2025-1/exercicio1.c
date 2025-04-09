/*  Exercício 1:
 *  Matriz Diagonal
 *
 */

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif
 
#include <stdio.h>

struct diagonal {
    
    int ordem;
    int* v;
};
typedef struct diagonal Diagonal;

Diagonal* DS_MatCria( int ord ) {

    Diagonal* Matriz;
    Matriz->ordem = ord;
    Matriz->v[ord];
    return Matriz;
}

int main() {

    Diagonal* Matriz = DS_MatCria(3);
    return 0;
}