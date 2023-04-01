/*
 *              TstOutDatRX.c
 *              © LFSoft 1994
 *
 *  Tentative pour utiliser F2 & F3 du port joystick en sortie
 */

#include <LF.h>
#include <hardware/custom.h>

struct Custom *custom = 0xdff000;

void main( void ){
    FOREVER{
        puts("1");
        custom->potgo = 0x3500; // F2 = 1
        Delay(25);
        puts("0");
        custom->potgo = 0x2500; // F2 = 0
        Delay(25);
    }
}