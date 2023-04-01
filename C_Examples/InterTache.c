/******************************************************************************

    Fichier : InterTache.c
    Auteur : L.Faillie
    Version : 0.1
    Date :  27-Juin-1993

    But: Tester la communication entre tache pour le projet LFCom

*******************************************************************************/

#include <exec/types.h>
#include <exec/ports.h>
#include <exec/nodes.h>
#include <exec/memory.h>
#include <clib/exec_protos.h>
#include <intuition/intuition.h>
#include <lf.h>

#define PORTNAME "ITT.rendez-vous"
#define VERSION 01

// Quelques definitions pour facilit� la programation

#define ITT struct _ITT

typedef struct MsgPort  PORT;
typedef struct Process  PROC;
typedef struct Message  MSG;

ITT {
MSG     SysMsg;
short   Version;
char   *Commande;
};

PORT    *msgp=NULL;
ITT     *msg;

/*=======================< Tout est perdu >==================================*/
void Fatal(char *msg)
{
    char buff[100];

    buff[0]=0;buff[1]=50;buff[2]=17;
    strcpy(&buff[3],msg);
    buff[4+strlen(msg)]=0;
    DisplayAlert(RECOVERY_ALERT,buff,30);
    exit(15);
}

/*========< Fonctions de liberation de resources >===========================*/

void delport(){
    DeletePort(msgp);
}

void liberemem(){
    FreeMem(msg,sizeof(ITT));
}

/*========< Essais de communication entre taches >===========================*/

void main(){
    msgp=FindPort(PORTNAME);    // Le port �xiste-t-il deja ?

    if(!msgp){
        char fin=0;

#ifdef DEBUG
        puts("*D* Le port n'�xiste pas");
#endif
        if(!(msgp=(PORT *) CreatePort(PORTNAME,0))) // On le cr�e.
            Fatal("Impossible de cr�e le port");
        atexit(delport);

        until(fin) {
#ifdef DEBUG
        printf("*D* J'attend des messages ...");
        fflush(stdout);
#endif
            WaitPort(msgp);     // On attend des m�ssages.
#ifdef DEBUG
        puts("OK");
#endif
            while(msg=(ITT *)GetMsg(msgp)){ // On les affiche
                printf("Msg lue :'%s'\n",msg->Commande);
                if(!strcmp(msg->Commande,"fini"))
                    fin=1;
                ReplyMsg(msg);
            }
        }
    } else {
        char txt[80];
        char fin=0;
        PORT *reply;

#ifdef DEBUG
        puts("*D* Le port �xiste");
#endif
        if(!(msg=(ITT *)AllocMem(sizeof(ITT),MEMF_PUBLIC|MEMF_CLEAR)))
            Fatal("Impossible d'allouer la m�moire");
        atexit(liberemem);

        if(!(reply=(PORT *)CreatePort(NULL,0)))
            Fatal("Impossible de cr�e un nouveau port");
#ifdef DEBUG
        puts("*D* Le port de reponse est cr��");
#endif

            // Init du m�ssage
        msg->SysMsg.mn_Node.ln_Type = NT_MESSAGE;
        msg->SysMsg.mn_ReplyPort = reply;
        msg->SysMsg.mn_Length = sizeof( ITT );
        msg->Commande = txt;

        puts("Entrez les commandes. 'fini' pour quitter les 2 programmes.");

        until(fin){
            scanf("%79s",txt);
            PutMsg(msgp,msg);
#ifdef DEBUG
        printf("*D* Message envoy�, j'attend une r�ponse ...");
        fflush(stdout);
#endif
            WaitPort(reply);
#ifdef DEBUG
        puts("OK");
#endif
            while(GetMsg(reply));
            if(!strcmp(txt,"fini"))
                fin=1;
        }
        DeletePort(reply);
    }
}
