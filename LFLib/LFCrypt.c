#include <LF.h>
#include <string.h>
#include <stdlib.h>

__regargs char *LFCrypt(char *msg,char *pw,ULONG *len)
// Revoie la chaine cryptée.
// len contient la taille du message, NULL si on ne veut pas de valeur.
{
    char mask=0, *ptr,*p,*ppw=pw;
    ULONG l,i;

    l=strlen(msg);
    if(len)
        *len=l;

    if(!*pw)
        return NULL;    // Le mot de passe est vide.

    if(!(ptr=p=(char *)malloc(l+1)))
        return(NULL);   // Manque de mémoire
    strcpy(ptr,msg);

    for(i=0;i<=l;i++){
        mask ^=*ppw++;
        if(!*ppw)
            ppw=pw;
        *ptr++^=mask;
    }

    return(p);
}
