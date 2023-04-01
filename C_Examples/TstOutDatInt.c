/*
 *              TstOutDatInt.c
 *              © LF Soft 1994
 *
 *  Test pour ecrire des données par le port joystick.
 *
 *   Le probleme est que le gameport.device utilise une interruption sur le VBL
 *  qui remet le bits DATR? a 0 tout les 1/50eme de secondes. Ce programme
 *  place sa propre interruption apres celle du gameport.device pour
 *  reinitialiser ses bits.
 */

#define SysBase_DECLARED
#include <LF.h>
#include <lists.h>
#include <exec/execbase.h>
extern struct ExecBase *SysBase;
#include <proto/exec.h>
#include <exec/tasks.h>
#include <exec/interrupts.h>
#include <hardware/intbits.h>
#include <hardware/custom.h>

WORD val = 0x500;

struct Custom *custom = 0xdff000;

__geta4 inter( void ){
    custom->potgo = val;
    return 0;
    // Lorsque l'on sort, le flag Z doit etre mis pour que les autres
    // serveur soit actif.Ce return le fait car les instructions qui suivent
    // le MOVEQ #0,D0, ( MoveM ...,(A7)+ ou AddA A7) ne touche pas ce flags.
}

struct Interrupt sinter;

void ajtserver( void ){
    struct Node *nd;
    sinter.is_Node.ln_Type = NT_INTERRUPT;
    sinter.is_Node.ln_Pri  = 0;
    sinter.is_Node.ln_Name = "TstOutDat";
    sinter.is_Code = inter;
    sinter.is_Data = NULL;

    AddIntServer(INTB_VERTB,&sinter);

/* Il faut mettre notre serveur juste deriere celui du gameport */

    Disable();

    nd= &sinter.is_Node;
    while(nd= GetPred(nd)){
        if(!strcmp(nd->ln_Name,"gameport.device"))
            break;
    }

    if(nd){ // Le node a été trouvé. On place le notre juste deriere
        Remove(&sinter.is_Node);
        sinter.is_Node.ln_Succ = nd->ln_Succ;   // suivant
        sinter.is_Node.ln_Pred = nd;            // précedant
        nd->ln_Succ=&sinter;
        sinter.is_Node.ln_Succ->ln_Pred = &sinter;
    }

    Enable();
}

int suppserver( void ){
    RemIntServer(INTB_VERTB,&sinter);
}

void main( void ){

    ajtserver();
    atexit(suppserver);

    FOREVER{
        Delay(25);
        chkabort();
        val = 0x3500;
        Delay(25);
        chkabort();
        val = 0x2500;
    }
}