/****************************************************************************

                        Tst_LFInJoy.c

                        © LFSoft 1993

    Teste de la fonction LFInJoy() de la LF.lib v3.03 .


*****************************************************************************/

#include <LF.h>
#include <stdlib.h>
#include <proto/dos.h>

void main(){

    UBYTE old=0;

    until( LFInJoy(LFJ_MOUSE) & LFJ_FIRE){
        if(LFInJoy(LFJ_JOY) != old){
            old=LFInJoy(LFJ_JOY);
            if(old & LFJ_FIRE)
                printf("Feu (*) ");
            if(old & LFJ_LEFT)
                printf("Gauche (<-) ");
            if(old & LFJ_RIGHT)
                printf("Droite (->) ");
            if(old & LFJ_DOWN)
                printf("Bas (V) ");
            if(old & LFJ_UP)
                printf("Haut (^) ");
            if(!old)
                printf("Rien");
            puts("");
        }
        Delay(10);
    }
}