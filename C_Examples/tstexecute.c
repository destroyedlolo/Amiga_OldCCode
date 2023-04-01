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
    char x[80],tn[100];

    if(!(f=Open("AUX:",MODE_OLDFILE))){
        puts("Impossible d'ouvrir le fichier temporaire:");
        exit(0);
    }

    atexit(fin);

    if(!Execute("",Input(),f)){
        puts("Erreur d'execution");
        exit(20);
    }

//    Delay(500);

    exit(0);
}
