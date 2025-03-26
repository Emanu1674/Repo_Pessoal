
/*  d_ = Data (assets, tables, static data) / d_sprite.c
 * cl_ = Client-side logic                  / cl_main.c
 *  r_ = Renderer / visual output           / r_main.c
 *  g_ = Game logic
 *  c_ = Console / Commands
 *  f_ = File handling
 *  i_ = Initialize / Interface
 *  e_ = Error handling
 *  m_ = Miscellaneous / Memory / Math
 *  p_ = Physics / Parsing
 *  s_ = Sound / System
 * 
 *  Local Var       →   camelCase
 *  Global Var      →   snake_case / g_ prefix
 *  Functions       →   PascalCase
 *  Constants       →   SCREAMING_SNAKE_CASE
 *  Macros          →   SCREAMING_SNAKE_CASE
 *  Structs/Enums   →   PascalCase
*/



// Random boilerplate for example purposes,
//  100% certified complete nonsense
#ifdef _WIN32
#define CLEAR "cls"
#include <windows.h>
#include <conio.h>
#else
#define CLEAR "clear"
#endif

#ifndef ALWAYS_INLINE
#define ALWAYS_INLINE inline __attribute__((always_inline))
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>
#include <inttypes.h>
#include <stdbool.h>

#include <time.h>

#pragma once
//#include "include/r_ansi.h"

void I_Print( char* str ) {

    printf( "%s", str );
}

void I_DrawRectFill( int16_t y1, int16_t x1, int16_t y2, int16_t x2, int8_t color, int8_t colorBg, char* block ) {

    int16_t R_Width = abs( x2 - x1 ) + 1;
    char* buffer = malloc( R_Width * strlen( block ) + 1 );

    for ( int16_t i = 0 ; i < R_Width ; i++ )
        strncpy( buffer + i * strlen(block), block, strlen(block) );
    buffer[ R_Width * strlen(block) ] = '\0';

    printf( "\033[%d;%dm", color, colorBg + 10 );
    for ( int16_t H = y1 ; H <= y2 ; H++ )
        printf( "\033[%d;%dH%s", H, x1, buffer );

    printf( "\033[0m" );
    free( buffer );
}

void I_Pixel( int16_t y, int16_t x, int16_t color, int16_t colorBg, char* block ) {

    printf( "\033[%d;%dm", color, colorBg + 10 );
    printf( "\033[%d;%dH%s", y, x, block );
    printf( "\033[0m" );
}

static ALWAYS_INLINE float I_AbsF( float x ) {

    float ret;
    __asm__ ( "abs.s %0, %1"
            : "=f"( ret )
            :  "f"( x ) );
    return ret;
}

uint8_t E_MallocOutOfMemory( void ) {

    while (1) {
        char key = '';
        printf( "\nMemory allocation failed\n" );
        printf( "Abort, Retry, Fail?" );

        fflush(stdin);
        key = getch();
        if( key == 'A' || key == 'a' ) {
            exit(EXIT_FAILURE);
        } else
        if( key == 'R' || key == 'r' ) {
            return 11;
        } else
        if( key == 'F' || key == 'f' ) {
            return 20;
        }
    }
}

uint8_t E_BugCheck( uint8_t errCode ) {

    switch ( errCode ) {
        case 0x00:
            return E_MallocOutOfMemory();
            break;
        
        default:
            E_UnhandledException();
            break;
    }
}

void E_UnhandledException() {

    system( CLEAR );
    puts( "The system ran into a problem it can't recover from and\n"
          "has been shut down to prevent damage to your computer.\n\n" );

    puts( "The machine will restart automatically in 10 seconds, and any unsaved\n"
          "information will be lost. For more information, please visit\n"
          "https://www.genericcompany.com/errcode");

    puts( "ERROR 0x4C: FATAL_UNHANDLED\n" );
    puts( "         UnhandledException" );
    exit( EXIT_FAILURE );
}

void I_RambusAlloc( int8_t KB ) {

    // 64KB vroom vroom
    uint64_t* mem_rambus = malloc( 1024 * KB * sizeof( uint64_t ) );
    if ( mem_rambus == NULL ) { 
        uint8_t err = E_BugCheck( 0x00 );
        if( err == 11 ){
            I_RambusAlloc( KB );
        } else
        if( err == 20  ){
            puts( "\nMemory allocation failed.\n" );
            return;
        }
    }
}

int main( void ) {

    setlocale( LC_ALL, "en_us.UTF-8" );
    unsigned char*      CPU;
    unsigned char*      RCP;

    I_RambusAlloc(64);

    uint_fast64_t       fast[64];
    uint16_t*           answer[42];
    uint16_t            TMM;
    int8_t              i[16];
    int8_t              j;

    // Prints R_Graph  to the screen
    //  [FAST_SIZE] times per second
    for ( int i = 0 ; i <= 64 ; i++ ) {
        TMM = R_GraphTimeOut( fast[i] );
    }

    I_Print( "Hello, World!" );
    return 0;
}