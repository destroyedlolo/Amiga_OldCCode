#include <stdio.h>
#include <string.h>

__regargs char *LFgets(FILE *f,char *l,size_t max)
/* Lit une chaine & efface un eventuel \n */
{
    size_t i;

    fgets(l,max,f);
    if (l[(i=strlen(l)-1)]=='\n') l[i]=0;
    return(l);
}
