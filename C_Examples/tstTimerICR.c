/****************************************************************************

                        LFTstTimerICR.c

                        © LF Soft 1994

                   Gestion du Timer_B du CIA_B

    Note: Avec mon 68010, des frequences > 4000 Hz perturbe le déroulement
    normal du multitache. Blocage a 7000 Hz car cette tache ne peut plus etre
    stoppée. Plus ne nombre de tache actives de priorité >= a ce programme est
    grand, plus cette frequence est basse.
     On peut surement monté plus haut en fréquence mais il faut coupé le
    multi-tache. Une autre solution est que la tache qui gere l'interruption
    ai une priorité tres grandes (supperieur aux taches de gestion du systeme).

     Cette version utilise les fonctions de la resource de l'ACIA.

******************************************************************************/

// Cette défine permet de sortir des carré sur F1 du port 1
/*
#define PORT1_F1
*/

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

struct Library *CIAResource = NULL;

/* Determination de la periode d'échantillonage :
 * Le timer est décrémenté avec la frequence d'horloge E (709379hz en PAL),
 * la période d'échantillonage est donc de :
 *          1/f = 1/E * vtim.
 * ( f= frq d'échantillonage, vtim = valeur mise dans le timer )
 *          => vtim = E/F
 * Comme vtim est un USHORT, la frequence mini est de 11 Hz ( E/65535 )
 */

void inittimer(USHORT vtim){
 /* Initialise le timer
  *  -> vtim: Valeur d'échantillonage
  *  <- !=0 Erreur
  *
  *  Remarque: Le timer n'est pas demmaré.
  */

    /**************** Initialisation du compteur *****************/

    ciab.ciacrb &= ~CIACRBF_RUNMODE; // Redémarage automatique du compteur
    ciab.ciacrb &= ~CIACRBF_PBON;    // PB7 n'est pas touché
    ciab.ciacrb &= ~(CIACRBF_INMODE0|CIACRBF_INMODE1);  // La source est E

    ciab.ciatblo = vtim & 0xff;
    ciab.ciatbhi = vtim >> 8;

    ciab.ciacrb |= CIACRBF_LOAD; // Charge la valeur
}

/******************** Serveur d'interruption **************************/
ULONG cnt=0;

__geta4 inter( void ){
    cnt++;
    #ifdef PORT1_F1
        ciaa.ciapra ^= 128; // Inverse la valeur du signal F1 du port 1
    #endif
    return 0;
    // Lorsque l'on sort, le flag Z doit etre mis pour que les autres
    // serveur soit actif.Ce return le fait car les instructions qui suivent
    // le MOVEQ #0,D0, ( MoveM ...,(A7)+ ou AddA A7) ne touche pas ce flags.
}

struct Interrupt sinter;

int ajtserver( void ){
    sinter.is_Node.ln_Type = NT_INTERRUPT;
    sinter.is_Node.ln_Pri  = 0;
    sinter.is_Node.ln_Name = "TestTimer";
    sinter.is_Code = inter;
    sinter.is_Data = NULL;

    return(AddICRVector(CIAResource,CIAICRB_TB,&sinter));
}

int suppserver( void ){
    Disable();
    RemICRVector(CIAResource,CIAICRB_TB,&sinter);
    Enable();
    #ifdef PORT1_F1
        ciaa.ciaddra &= 0x7f;    // F1 du port 1 est remis en entrée
    #endif
    return 0;
}

void main(int ac,char **av){
    ULONG vt;

    OS2_0(); // car ex_EClockFrequency n'éxiste que depuis le 2.0

    #ifdef PORT1_F1
        ciaa.ciaddra |= 128;    // F1 du port 1 est en sortie
    #endif

    if(ac!=2 || !atoi(av[1])){
        puts("TstTimer frq_d'échantillonage");
        exit(1);
    }

    vt= (ULONG)(SysBase->ex_EClockFrequency)/atoi(av[1]);
    printf("Vt = %04x\n",vt);

    if(vt>0xffff){
        puts("Frequence or limite");
        exit(0);
    } else
        printf("Frequence reel %d\n", (ULONG)(SysBase->ex_EClockFrequency)/vt);

    if(!(CIAResource = OpenResource(CIABNAME))){ // Impossible d'ouvrir la resource
        LFatal("Impossible d'ouvrir la resource");
        exit(20);
    }

    Disable();
    if(ajtserver()){
        Enable();
        LFatal("Impossible d'ajouter le serveur");
        exit(20);
    }
    atexit(suppserver);

    inittimer(vt);
    Enable();

    ciab.ciacrb |= CIACRBF_START;   // demart le timer
    FOREVER{
        cnt=0;
        Delay(50);
        printf("%d \r",cnt); fflush(stdout);
    }
}

