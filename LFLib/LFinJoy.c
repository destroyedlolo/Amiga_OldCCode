/***************************************************************************

                        LFInJoy.c

                        L.Faillie

    Cette fonction teste l'état d'un joystick. Basé sur l'ACM 3.0

    12-11-1993 : premiere version
    26-03-1994 : Ajout des boutons F2 & F3 du port 1 ( Joystick )

****************************************************************************/
#include <lf.h>
#include <proto/exec.h>
#include <exec/types.h>
#include <hardware/custom.h>
#include <hardware/cia.h>

    // On defini des pointeurs au lieu d'utiliser directement les
    // extern struct CIA ciaa; car cette definition n'éxiste pas dans l'
    // amigaxxsr.lib
static struct Custom *custom = 0xdff000 ;
static struct CIA *ciaa = 0xbfe001 ;

__regargs UBYTE LFInJoy( UBYTE port )
{
  UBYTE data = 0;
  UWORD joy;

  custom->potgo = 0xf500;
  custom->potgo = 0x0500;

  if( port == LFJ_MOUSE )
  {
    /* PORT 1 ("MOUSE PORT") */
    joy = custom->joy0dat;
    data |= (!( ciaa->ciapra & 0x0040 ) ? LFJ_FIRE : 0) |\
        (!(custom->potinp & 0x0400) ? LFJ_FIRE2 : 0 ) |\
        (!(custom->potinp & 0x0100) ? LFJ_FIRE3 : 0 );
  }
  else
  {
    /* PORT 2 ("JOYSTICK PORT") */
    UWORD val_potinp;
    joy = custom->joy1dat;

    Disable();  // Interdit les interruptions
    custom->potgo = 0xf500; // On met les signaux en sortie à 1
    custom->potgo = 0x0500; // Remise en entrée des signaux.
    val_potinp = custom->potinp ; // On stocke le résultat
    Enable();   // Autorise les interruptions

    data |= (!( ciaa->ciapra & 0x0080 ) ? LFJ_FIRE : 0) |\
        (!(val_potinp & 0x4000) ? LFJ_FIRE2 : 0) |\
        (!(val_potinp & 0x1000) ? LFJ_FIRE3 : 0);
  }

  data += joy & 0x0002 ? LFJ_RIGHT : 0;
  data += joy & 0x0200 ? LFJ_LEFT : 0;
  data += (joy >> 1 ^ joy) & 0x0001 ? LFJ_DOWN : 0;
  data += (joy >> 1 ^ joy) & 0x0100 ? LFJ_UP : 0;

  return( data );
}

/*
void main(){
    UBYTE old=0;

    until( LFInJoy(LFJ_MOUSE) & LFJ_FIRE){
        if(LFInJoy(LFJ_JOY) != old){
            old=LFInJoy(LFJ_JOY);
            if(old & LFJ_FIRE)
                printf("Feu1 (*) ");
            if(old & LFJ_FIRE2)
                printf("Feu2 (*) ");
            if(old & LFJ_FIRE3)
                printf("Feu3 (*) ");
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
*/