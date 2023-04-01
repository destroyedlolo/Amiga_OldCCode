/****************************************************************************

                           Test_propInt.c
                           © LFSoft 1994

        Teste de l'utilisation d'un joystick proportionnel avec interruption.

*****************************************************************************/

#include <LF.h>
#include <hardware/custom.h>
#include <exec/interrupts.h>
#include <hardware/intbits.h>
#include <stdio.h>

extern __far struct Custom custom;

UWORD pot;

__geta4 inter( void ){
    pot = custom.pot1dat; // Lecture de la valeur du compteur
    custom.potgo = 1;     // Début d'une nouvelle mesure
    custom.potgo = 0xf00; // Non ! le menu n'est pas demandé
    return 0;
}

struct Interrupt sinter;

void ajtserver( void ){
    sinter.is_Node.ln_Type = NT_INTERRUPT;
    sinter.is_Node.ln_Pri  = 0;
    sinter.is_Node.ln_Name = "TstOutDat";
    sinter.is_Code = inter;
    sinter.is_Data = NULL;

    AddIntServer(INTB_VERTB,&sinter);
}

int suppserver( void ){
    RemIntServer(INTB_VERTB,&sinter);
}

void main( void ){
    ajtserver();
    atexit(suppserver);

    FOREVER {
        Delay(12); // Attend 1/4 second
        printf("Valeur = %04x\n", pot);
    }
}
