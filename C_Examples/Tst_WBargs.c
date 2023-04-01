/*****************************************************************************

                    Tst_WBargs.c

                     L.Faillie

    Lecture des arguments provenant de wbmain().

-> Le principe est identique que pour la fonction main standard :
    - Le nbre d'argument est contenu dans sm_NumArgs,
    - Les arguments se trouvent dans un tableau de structure WBArg sm_ArgList,
        sm_ArgList[0] contient l'outil ( l'éxécutable ),
    Dans ces structures, le wa_Lock contient un ... lock sur le repertoire du
    fichier ( utiliser CurrentDir() pour acceder au repertoire ).

******************************************************************************/

#include <LF.h>
#include <workbench/startup.h>
#include <stdio.h>

wbmain (struct WBStartup *wbarg){
    FILE *s;
    int i;

    if(!(s=fopen("CON:////Sortie de tst_wbarg","w"))){
        LFatal("Erreur durant l'ouverture de la fenetre ...");
        exit(20);
    }

    fprintf(s,"Nbre d'argument :%d\n",wbarg->sm_NumArgs);
    for(i=0;i<wbarg->sm_NumArgs;i++)
        fprintf(s,"%d : '%s'\n",i,wbarg->sm_ArgList[i].wa_Name);
    fflush(s);
    sleep(10);
    exit(0);
}

void main(){
    puts("Ce programme affiche les arguments provenant du WB.\n"
         "il est donc plus judicieu de le lancer du WB pour avoir un resultat !!\n");
}
