/****************************************************************************

                        LFTstTimer.c

                       © LF Soft 1994

                   Gestion du Timer_B du CIA_B

            Genere plusieur fréquence sur F1 du port 1

******************************************************************************/

// Cette défine permet de sortir des carré sur F1 du port 1
#define PORT1_F1

#define SysBase_DECLARED
#include <LF.h>
#include <exec/execbase.h>
extern struct ExecBase *SysBase;
#include <proto/exec.h>
#include <exec/tasks.h>
#include <exec/interrupts.h>
#include <proto/dos.h>
#include <resources/cia.h>
#include <hardware/cia.h>
#include <hardware/intbits.h>

extern void OS2_0( void );

extern __far struct CIA ciaa,ciab;

ULONG   timersigmask= 0L;    // Signal du timer
BYTE    sig = -1;
struct Library *CIAResource = NULL;
struct Task *tachemaitre;

/* Determination de la periode d'échantillonage :
 * Le timer est décrémenté avec la frequence d'horloge E (709379hz en PAL),
 * la période d'échantillonage est donc de :
 *          1/f = 1/E * vtim.
 * ( f= frq d'échantillonage, vtim = valeur mise dans le timer )
 *          => vtim = E/F
 * Comme vtim est un USHORT, la frequence mini est de 11 Hz ( E/65535 )
 */


int changetimer(USHORT vtim){
/* Change la valeur du timer */

    ciab.ciacrb &= ~CIACRBF_START; // stop le timer

    ciab.ciatblo = vtim & 0xff;
    ciab.ciatbhi = vtim >> 8;

    ciab.ciacrb |= CIACRBF_LOAD; // Charge la valeur
    ciab.ciacrb |= CIACRBF_START; // demart le timer
}

int inittimer(USHORT vtim){
 /* Initialise le timer
  *  -> vtim: Valeur d'échantillonage
  *  <- !=0 Erreur
  *
  *  Remarque: Le timer n'est pas démaré.
  */

        /********* Initialisation diverses ***********/
    if(ciab.ciacrb & CIACRBF_START) // Déja occupé
        return 1;

    if(!(tachemaitre = FindTask(NULL))) // Impossible de trouvé cette tache !!
        return 2;

    if((sig = AllocSignal(-1L))== -1)   // Impossible d'allouer un signal
        return 3;
    else
        timersigmask = 1L << sig;

    if(!(CIAResource = OpenResource(CIABNAME))) // Impossible d'ouvrir la resource
        return 4;

    /**************** Initialisation du compteur *****************/

    ciab.ciacrb &= ~CIACRBF_RUNMODE; // Redémarage automatique du compteur
    ciab.ciacrb &= ~CIACRBF_PBON;    // PB7 n'est pas touché
    ciab.ciacrb &= ~(CIACRBF_INMODE0|CIACRBF_INMODE1);  // La source est E

    ciab.ciatblo = vtim & 0xff;
    ciab.ciatbhi = vtim >> 8;

    ciab.ciacrb |= CIACRBF_LOAD; // Charge la valeur
    /********* Autorise les interruptions du timer B *************/

    ciab.ciaicr = CIAICRF_SETCLR|CIAICRF_TB;

    return 0;
}

void liberetimer(){
    ciab.ciacrb &= ~CIACRBF_START; // stop le timer
    ciab.ciaicr = CIAICRF_TB; // Interdit les interuptions

    if(sig) FreeSignal(sig);
}

/******************** Serveur d'interruption **************************/

__geta4 inter( void ){
    ciaa.ciapra ^= 128; // Inverse la valeur du signal F1 du port 1
    return 0;
    // Lorsque l'on sort, le flag Z doit etre mis pour que les autres
    // serveur soit actif.Ce return le fait car les instructions qui suivent
    // le MOVEQ #0,D0, ( MoveM ...,(A7)+ ou AddA A7) ne touche pas ce flags.
}

struct Interrupt sinter;

void ajtserver( void ){
    sinter.is_Node.ln_Type = NT_INTERRUPT;
    sinter.is_Node.ln_Pri  = 0;
    sinter.is_Node.ln_Name = "TestTimer";
    sinter.is_Code = inter;
    sinter.is_Data = NULL;

    AddIntServer(INTB_EXTER,&sinter);
}

void suppserver( void ){
    RemIntServer(INTB_EXTER,&sinter);
}

void main(){
    USHORT f;
    ULONG vt;

    OS2_0(); // car ex_EClockFrequency n'éxiste que depuis le 2.0

    ciaa.ciaddra |= 128;    // F1 du port 1 est en sortie

    ajtserver();
    atexit(suppserver);

    atexit(liberetimer);
    inittimer(0);

    FOREVER{
        for(f=100;f<1000;f+=10){
            vt= (ULONG)(SysBase->ex_EClockFrequency)/f;
            changetimer(vt);
            printf("%d hz\n",f);
            Delay(50);
        }
        for(f=1000;f>110;f-=10){
            vt= (ULONG)(SysBase->ex_EClockFrequency)/f;
            changetimer(vt);
            printf("%d hz\n",f);
            Delay(50);
        }
    }
    ciaa.ciaddra &= 0x7f;    // F1 du port 1 est remis en entrée

    exit(0);
}
