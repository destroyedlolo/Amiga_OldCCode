/*****************************************************************************

                                  TstMem.c
                                © LFSoft 1993

         Ce programme cherche tout les chunks de mémoire libre.

******************************************************************************/

#include <exec/execbase.h>
#include <exec/memory.h>
#include <lists.h>

extern struct ExecBase *SysBase;

void main(){
    struct MemHeader *mh;
    struct MemChunk *mc;

    printf("SysBase = %x\n",SysBase);

    mh=GetHead(&SysBase->MemList);

    do {
        printf(" mh= %08x\n",mh);
        printf("Lower= %08x Upper %08x\n",mh->mh_Lower,mh->mh_Upper);
        printf("1er MemChunk = %08x\n",mh->mh_First);
/* Les chunks sont linké les uns à la suite des autres ( pointeur = mc_Next ) */
    } while(mh=GetSucc(mh));
}
