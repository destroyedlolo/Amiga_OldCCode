/***************************************************************************

                        LFStrDos.c

                        L.Faillie

                    Concatene un path & un nom de fichier.

    10-04-1994: Supprime un bug si le path est null

****************************************************************************/

#include <stdlib.h>
#include <string.h>

__regargs char *LFStrDos(const char *p,const char *f)
{
    char *s;
    int t;

    if(!p || !f)
        return (0);

    if(!(s=(char *)malloc(strlen(p)+strlen(f)+2)))
        return(0);
    if(*p){
        strcpy(s,p);
        if(s[t=strlen(s)-1]!='/' && s[t]!=':')
            strcat(s,"/");
    } else
        *s=0;

    strcat(s,f);
    return(s);
}
