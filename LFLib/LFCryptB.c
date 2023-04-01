#include <LF.h>
#include <string.h>
#include <stdlib.h>

__regargs char *LFCryptB(char *msg,char *pw,char *buff,ULONG len)
/* Revoie des donn�es crypt�es. Contrairemant a LFCrypt() cette fonction peut */
/* aussi traiter les donn�es binaire. */
// len contient la taille des donn�es. /!\ different de LFCrypt().
// buffer contiendra le resultat. Evidement, il doit etre asses grand pour contenir
// les donn�es.
{
    char mask=0,*p,*ppw=pw;
    ULONG i;

    if(!*pw)
        return NULL;    // Le mot de passe est vide.

    if(!len)
        return NULL; // Longeure null !?!?!

    memcpy(p=buff,msg,len);

    for(i=0;i<len;i++){
        mask ^=*ppw++;
        if(!*ppw)
            ppw=pw;
        *p++^=mask;
    }

    return(buff);
}
