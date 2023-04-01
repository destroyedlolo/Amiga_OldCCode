/*
 *          TstAppWin.c
 *                  LFSoft 1994
 *
 *   Test des AppWindows du WB 2.0+
 *
 *  Cf AmigaNews de Juillet-Aout 1994
 *  voir aussi Tst_WBargs.c qui contient un exemple d'utilisation des WBArgs.
 *
 *  Note: Pour une utilisation complete, il ne faut pas oublier tester si toutes
 *  listes de message sont vides avant d'appeler Wait().
 */

#include <LF.h>
#include <workbench/workbench.h>
#include <workbench/startup.h>
#include <proto/wb.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/exec.h>

    /* Quelques défines utiles */
#define SIGWIN  (1L << wnd->UserPort->mp_SigBit)
#define SIGAPP  (1L << aport->mp_SigBit)

struct Window *wnd;
struct AppWindow *awnd;
struct MsgPort *aport;

    /********************** Libération des resources ***********************/
void ferme_wnd( void ){
    CloseWindow(wnd);
}

void lib_port( void ){
    struct Message *msg;

    while(msg = GetMsg(aport))
        ReplyMsg(msg);

    DeleteMsgPort(aport);
}

void supp_appwnd( void ){
    RemoveAppWindow(awnd);
}

    /*************************** Boucle principale *************************/

void main( void ){
    ULONG sig;

    OS2_0();

        /* Ouverture de la fenetre */
    if(!(wnd = OpenWindowTags(NULL,WA_Width,300,WA_Height,50,
        WA_IDCMP,CLOSEWINDOW, WA_Flags, WINDOWCLOSE|WINDOWDRAG,
        WA_Title,"Lacher un fichier",TAG_DONE))){
            LFatal("Impossible d'ouvrir la fenêtre");
            exit(20);
    }
    atexit(ferme_wnd);

        /* Ajout de l'AppWindow */
    if(!(aport = CreateMsgPort())){
        LFatal("Impossible de créer le port");
        exit(20);
    }
    atexit(lib_port);
    if(!(awnd=AddAppWindow(0,NULL,wnd,aport,NULL))){
        LFatal("Impossible de définir l'AppWindow");
        exit(20);
    }
    atexit(supp_appwnd);

        /* Gestion des messages */
    FOREVER {
        sig = Wait( SIGWIN | SIGAPP );

        if(sig & SIGWIN){   // Signal de la fenêtre
            struct Message *msg;

             while(msg = GetMsg(wnd->UserPort))
                    ReplyMsg(msg);
                        // Auccun test car on n'attend que la fermeture
            exit(0);
        }

        if(sig & SIGAPP){   // Signal de l'AppWindow
            struct AppMessage *msg;

            while(msg = GetMsg(aport)){
                int i;
                printf("%d argument%c :\n",msg->am_NumArgs,
                                        (msg->am_NumArgs>1)?'s':0);
                for(i=0; i<msg->am_NumArgs; i++)
                    puts(msg->am_ArgList[i].wa_Name);

                ReplyMsg((struct Message *)msg);
            }
        }
    }
}
