#include <lf.h>
#include <string.h>

__regargs char *LFDecrypt(const char *msg,const char *pw,ULONG len){
// Routine de decryptage. Renvoie la chaine decryptée.
    char mask=0, *ptr,*p,*ppw=pw;

    ULONG i;

    if(!*pw||!len)        // Le Mots de passe ou la longueur est vide
        return(NULL);

    if(!(ptr=p=(char *)malloc(len+1)))
        return(NULL);   // Manque de mémoire
    memcpy(ptr,msg,len+1);

    for(i=0;i<=len;i++){
        mask ^=*ppw++;
        if(!*ppw)
            ppw=pw;
        *ptr++^=mask;
    }

    return(p);
}
