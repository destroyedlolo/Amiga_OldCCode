/****************************************************************************

                        LFTstTimer.c

                       � LF Soft 1994

                   Gestion du Timer_B du CIA_B

    Note: Avec mon 68010, des frequences > 4000 Hz perturbe le d�roulement
    normal du multitache. Blocage a 7000 Hz car cette tache ne peut plus etre
    stopp�e. Plus le nombre de tache actives de priorit� >= a ce programme est
    grand, plus cette frequence est basse.
     On peut surement mont� plus haut en fr�quence mais il faut coup� le
    multi-tache. Une autre solution est que la tache qui gere l'interruption
    ai une priorit� tres grandes (supperieur aux taches de gestion du systeme).

******************************************************************************/

// Cette d�fine permet de sortir des carr� sur F1 du port 1
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

/* Determination de la periode d'�chantillonage :
 * Le timer est d�cr�ment� avec la frequence d'horloge E (709379hz en PAL),
 * la p�riode d'�chantillonage est donc de :
 *          1/f = 1/E * vtim.
 * ( f= frq d'�chantillonage, vtim = valeur mise dans le timer )
 *          => vtim = E/F
 * Comme vtim est un USHORT, la frequence mini est de 11 Hz ( E/65535 )
 */

int inittimer(USHORT vtim){
 /* Initialise le timer
  *  -> vtim: Valeur d'�chantillonage
  *  <- !=0 Erreur
  *
  *  Remarque: Le timer n'est pas d�mar�.
  */

        /********* Initialisation diverses ***********/
    if(ciab.ciacrb & CIACRBF_START) // D�ja occup�
        return 1;

    if(!(tachemaitre = FindTask(NULL))) // Impossible de trouv� cette tache !!
        return 2;

    if((sig = AllocSignal(-1L))== -1)   // Impossible d'allouer un signal
        return 3;
    else
        timersigmask = 1L << sig;

    if(!(CIAResource = OpenResource(CIABNAME))) // Impossible d'ouvrir la resource
        return 4;

    /**************** Initialisation du compteur *****************/

    ciab.ciacrb &= ~CIACRBF_RUNMODE; // Red�marage automatique du compteur
    ciab.ciacrb &= ~CIACRBF_PBON;    // PB7 n'est pas touch�
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

void main(int ac,char **av){
    ULONG vt;

    OS2_0(); // car ex_EClockFrequency n'�xiste que depuis le 2.0

    #ifdef PORT1_F1
        ciaa.ciaddra |= 128;    // F1 du port 1 est en sortie
    #endif

    if(ac!=2 || !atoi(av[1])){
        puts("TstTimer frq_d'�chantillonage");
        exit(1);
    }

    vt= (ULONG)(SysBase->ex_EClockFrequency)/atoi(av[1]);
    printf("Vt = %04x\n",vt);

    if(vt>0xffff){
        puts("Frequence or limite");
        exit(0);
    }

    ajtserver();
    atexit(suppserver);

    atexit(liberetimer);
    inittimer(vt);

    ciab.ciacrb |= CIACRBF_START;   // demart le timer
    FOREVER{
        cnt=0;
        Delay(50);
        printf("%d\n",cnt);
    }
    #ifdef PORT1_F1
        ciaa.ciaddra &= 0x7f;    // F1 du port 1 est remis en entr�e
    #endif

    exit(0);
}
