/*****************************************************************************

                            Tst_OutJoy.c

                           © LF Soft 1994

            Utilisation de F1 du port 1 (joystick) en sortie

    void TstOutDat*.c pour F3. LA GESTION EST FAUSSE DANS CETTE EXEMPLE

******************************************************************************/

#include <LF.h>
#include <hardware/cia.h>
#include <hardware/custom.h>

extern __far struct CIA ciaa;
extern __far struct Custom custom;

// Cette défine fait aussi changé F3 du port0
/*
#define F3P0
*/

#ifdef F3P0
    #define VAL 0x5100
#else
    #define VAL 0x5000
#endif

void main(){
    BOOL f=0;

    printf("ciaddra = %x\n",ciaa.ciaddra);
    ciaa.ciaddra |= 128;
#ifdef F3P0
    custom.potgo = custom.potinp | 0xa200; // F2 & F3 port1 & F3 port0 en sortie
#else
    custom.potgo = custom.potinp | 0xa000; // F2 & F3 port1 en sortie
#endif

    FOREVER { // On stop le programme avec 1 break
        ciaa.ciapra ^= 128;
        puts(ciaa.ciapra & 128 ? "F1 On": "F1 Off");
        Forbid();// Plus de multitache
        if(f){
            f=FALSE;
            custom.potgo = (custom.potinp & ~(VAL)) | 1;
            puts("F2 F3 Off");
        } else {
            f=TRUE;
            custom.potgo = custom.potinp | VAL | 1 ;
            puts("F2 F3 On");
        }
        Permit();// retour du multi-tache
        Delay(5);
    }
}