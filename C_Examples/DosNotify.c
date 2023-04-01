/*
 *      DOSNotify.c
 *
 *          © LFSoft 1995
 *
 *      Test les notifications du DOS. Dans cette exemple, la tâche est notifiée
 *  par un CTRL-F
 *
 *  Historique:
 *      08/08/1995  : Premiere version
 */

#include <LF.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <stdio.h>
#include <stdlib.h>

struct NotifyRequest req;

void main( int ac, char **av ){
    if(ac != 2){
        puts("Systaxe:\n\tDosNotify [rep]");
        exit(5);
    }
    req.nr_Name = av[1];
    req.nr_Flags = NRF_SEND_SIGNAL;
    req.nr_stuff.nr_Signal.nr_Task = FindTask(NULL);
    req.nr_stuff.nr_Signal.nr_SignalNum = SIGBREAKB_CTRL_F;

    if(!StartNotify(&req)){
        puts("Erreur de StartNotify()");
        exit(10);
    }
    puts("CTRL-C pour sortir");

    FOREVER{
        ULONG sig = Wait(SIGBREAKF_CTRL_C|SIGBREAKF_CTRL_F);

        if(sig & SIGBREAKF_CTRL_F)
            puts("Notification");

        if(sig & SIGBREAKF_CTRL_C)
            break;
    }

    puts("C'est fini");
    EndNotify(&req);

    exit(0);
}
