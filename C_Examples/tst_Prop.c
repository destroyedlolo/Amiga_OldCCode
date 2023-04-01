/****************************************************************************

                            Test_prop.c
                           © LFSoft 1994

        Teste de l'utilisation d'un joystick proportionnel.

*****************************************************************************/

#include <LF.h>
#include <hardware/cia.h>
#include <stdio.h>

extern __far struct CIA ciaa;
extern __far struct Custom custom;

void main(){
    puts("\33[0 p");
    FOREVER { // On stop le programme par un break
        custom.potgo = 1; // Début de la mesure
        Delay(20);  // On attent 300 ms que la mesure soit faite.
        printf("Joy0=%04x joy1=%04x\r",custom.pot0dat,custom.pot1dat);
        fflush(stdout);
    }
}
