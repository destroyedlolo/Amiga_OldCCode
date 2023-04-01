#include <lf.h>
#include <string.h>

__regargs char *LFDecryptB(const char *msg,const char *pw,char *buff,ULONG len){
// Routine de decryptage. Renvoie la chaine decryptée.
// buff est le buffer où ce trouvera le resultat.

    char mask=0,*p,*ppw=pw;

    ULONG i;

    if(!*pw||!len)        // Le Mots de passe ou la longueur est vide
        return(NULL);

    memcpy(p=buff,msg,len+1);

    for(i=0;i<len;i++){
        mask ^=*ppw++;
        if(!*ppw)
            ppw=pw;
        *p++^=mask;
    }

    return(buff);
}
