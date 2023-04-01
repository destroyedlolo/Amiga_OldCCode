/***************************************************************************
                            TstReadArgs.c

                            © LF Soft 1994

    Teste des fonction de lecture des arguments & d'IO de la Dos.Library

 ^
/!\ Attention : Du fait de la façon dont ils sont affichés, tous les arguments
¯T¯     doivent etre des chaînes ( pas de /N )...

****************************************************************************/

#include <LF.h>
#include <proto/Dos.h>
#include <stdlib.h>

    // Nombre de mots clefs dans le 'template'
#define NBRE_MC 1
    // Le template en lui-même
#define TEMP    "TEST=-T/K"

struct RDArgs *argt;

void fini(void){
    if(argt) FreeArgs(argt);
}

void main(int ac,char **av){
    int i;
    long argr[NBRE_MC];

    atexit(fini);
    if(!(argt=ReadArgs(TEMP,argr,NULL))){
        PrintFault(IoErr(),av[0]);
        exit(20);
    }
    PutStr("Ok, Les arguments sont :\n");
    for(i=0;i<NBRE_MC;i++)
        if(argr[i])
            Printf("%ld: '%s'\n",i,(char *)argr[i]);
        else
            Printf("%ld: Absant\n",i);
}