#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

void clearScreen(void){
    system(CLEAR);
}

void setColor(int x, int y, int color, int bgcolor){
    //if(color != 0);
    printf("\033[%d;%dH", y, x);
    printf("\033[%d;%dm", color, bgcolor+10);
    printf("\033[6n");
    printf("\033[0m");
}

void fillScreen(int color, int bgcolor, const char *block){
    int W, H;
    printf("\033[%d;%dm", color, bgcolor+10);
    for(H = 0 ; H <= 25 ; H++){
        for(W = 0 ; W <= 80 ; W++)
            printf("\033[%d;%dH%s", H, W, block);
    }
}

void drawPixel(int x, int y, int color, int bgcolor, const char *block){
    printf("\033[%d;%dH", y, x);
    printf("\033[%d;%dm", color, bgcolor+10);
    printf("\033[%d;%dH%s", y, x, block);
    printf("\033[0m");
}

int main(){
    setlocale(LC_ALL, "en_us.UTF-8");
    fillScreen(34, 30, u8"█");
    //drawPixel(18, 5, 33, 31, u8"▄");
    setColor(18, 5, 33, 31);
    getchar();
    clearScreen();
    return 0;
}