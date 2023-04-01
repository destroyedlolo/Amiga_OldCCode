/*
 *              TstPotGo.c
 *              © LFSoft 1994
 *
 *  Tentative pour utiliser F2 du port joystick en sortie
 */

#include <exec/types.h>
#include <resources/potgo.h>
#include <clib/potgo_protos.h>
#include <clib/exec_protos.h>

#include <stdio.h>
#include <stdlib.h>

struct Library *PotgoBase = NULL;
UWORD bits;

void libbits( void ){
/* Libère les bits alloués */
    WritePotgo(0x2000,bits); // F2 = 0
    FreePotBits(bits);
}

void main( void ){
    if(!(PotgoBase = OpenResource(POTGONAME))){
        puts("Impossible d'ouvrir la resource");
        exit(20);
    }

    if((bits = AllocPotBits(0x3000)) != 0x3000){
        puts("Impossible d'allouer DatRX !");
        exit(20);
    }
    atexit(libbits);

    for(;;){
        puts("1");
        WritePotgo(0x3000, bits); // F2 = 1
        Delay(25); // attent 1/2 seconde
        puts("0");
        WritePotgo(0x2000,bits); // F2 = 0
        Delay(25);
    }
}