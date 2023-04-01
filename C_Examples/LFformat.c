/***************************************************************************

                             LFformat.c

                            © LFSoft 1994

    Routine de formatage à la printf()
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

__stkargs char *LFformat(const char *fmt, const char *cmd, char *premier,...){
/* cmd contient les commandes de formatage dans l'ordre .
 * exemple : si cmd = "abc"
 *              -> %a est remplacer par le premier argument,
 *              -> %b par le second,
 *              -> %c par le 3em
 */

    char *c, *p, *cp;
    int len = strlen(fmt) + 1;
    char **arg;

    arg = (char **)&premier;

    if(!(p=fmt) || !cmd)    // Rien a formater
        return NULL;

    if(!(cp=c=malloc(len)))
        puts("No memories");
    else while(*p){
        if(*p == '%'){
            unsigned short idx;
            char *x;

            *cp = 0; p++;
            if(!(x=strchr(cmd,*p))){
                printf("Erreur : %%%c n'est pas un token\n",*p);
                free(c);
                return NULL;
            } else {
                idx = x-cmd;
                printf("*D* %%%c ->arg[%d]\n    =>%s\n",*p,idx,arg[idx]);
                if(!arg[idx]){
                    printf("Erreur : la chaine pour %%%c est NULL\n",*p);
                    return NULL;
                }
                if(!(c = realloc(c, len += strlen(arg[idx]) - 2))){
                    puts("No memories");
                    return NULL;
                }
                strcat(c,arg[idx]);
                cp=c; while(*cp) cp++;
                *cp = 0; p++;
            }
        } else
            *cp++ = *p++;
    }
    *cp= 0;

    return c;
}

void main(){
    char *res = LFformat("Ceci %b %a %c", "abc","un","est","test");
    puts(res);
}