/* Launch a shell on AUX: */

#include <exec/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <libraries/dos.h>
#include <libraries/dosextens.h>

struct FileHandle *f,*Open(),*Input();

void fin()
{
    Close(f);
}

void main(c,v)
int c;
char **v;
{
    if(!(f=Open("AUX:",MODE_READWRITE))){
        puts("Impossible d'ouvrir le fichier temporaire:");
        exit(0);
    }

    atexit(fin);

    if(!Execute("",f,0)){
        puts("Erreur d'execution");
        exit(20);
    }

//    Delay(500);

    exit(0);
}
