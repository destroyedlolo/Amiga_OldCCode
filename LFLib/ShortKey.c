/***************************************************************************

                        ShortKey.c

                        L.Faillie

                    Gestion des ShortKeys.
     Une liste est créée contenant toutes les touches utilisée. Ces fonctions
    permettent de la gérée.

****************************************************************************/

#define LFAddKey    fonction_bidon
#define LFFindKey   fonction_bidon

#include <LF.h>
#include <proto/exec.h>
#include <exec/memory.h>
#include <ctype.h>

#undef LFAddKey
#undef LFFindKey

#define IGNORE_CASE 1

static __regargs struct Node *_findkey(struct List *lst,char key,BOOL icase){
    struct Node *nd = GetHead(lst);

    while(nd){
        if(icase || (nd->ln_Type & IGNORE_CASE )){ // insensible au majuscule
            if(toupper(nd->ln_Pri) == toupper(key))
                return nd;
        } else
            if(nd->ln_Pri == key)
                return nd;
        nd = GetSucc(nd);
    }
    return(NULL); // Pas trouvé
}

__stkargs int LFAddKey( struct List *lst, char key, Tag tags){
    struct Node *nd;
    Tag data = (Tag) -1,
        mode = (Tag) LFKM_MODIFY,
        icase = (Tag) -2;
    struct TagItem *taglist = (struct TagItem *)&tags;
    int i=0;

    // Lecture des tags
    while(taglist[i].ti_Tag != TAG_DONE) {
        switch(taglist[i].ti_Tag){
        case LFK_DATA:
            data = taglist[i].ti_Data; break;
        case LFK_MODE:
            mode = taglist[i].ti_Data; break;
        case LFK_IGNORE_CASE:
            icase = taglist[i].ti_Data; break;
        }
        i++;
    }

#ifdef DEBUG
    printf("*D* data = %8X,mode = %d,icase = %d\n",data,mode,icase);
#endif

    if(!lst || !key)
        return LFAK_DIV;

    if(nd = _findkey(lst,key,icase)){   // La touche existe déjà
        #ifdef DEBUG
            printf("'%c' existe déjà\n",key);
        #endif
        if(mode == LFKM_OCREATE)
            return LFAK_NAE;

        if(icase != -2)
            nd->ln_Type = (icase)? IGNORE_CASE:0;
        if(data != -1)
            nd->ln_Name = (char *)data;
    } else {
        #ifdef DEBUG
            printf("'%c' n'existe pas encore\n",key);
        #endif

        if(icase == -2)
            icase = 0;

        if(data == -1)
            return LFAK_DIV;

        if(!(nd = AllocVec(sizeof(struct Node),MEMF_ANY)))
            return LFAK_PAM;

        nd->ln_Type = (icase)? IGNORE_CASE:0;
        nd->ln_Pri = (BYTE) key;
        nd->ln_Name = (char *)data;
        AddTail(lst,nd);
    }

    return LFAK_OK;
}

__stkargs void *LFFindKey( struct List *lst, char key, Tag tags){
/* Renvoie le champ data du node si il est trouvé. Sinon, NULL ou une valeur
 * par défaut fourni par le tag LFK_DATA. Toujours NULL en cas d'erreur.
 */
    struct Node *nd;
    Tag data = (Tag) NULL,
        icase = (Tag) 0;
    struct TagItem *taglist = (struct TagItem *)&tags;
    int i=0;

    // Lecture des tags
    while(taglist[i].ti_Tag != TAG_DONE) {
        switch(taglist[i].ti_Tag){
        case LFK_DATA:
            data = taglist[i].ti_Data; break;
        case LFK_IGNORE_CASE:
            icase = taglist[i].ti_Data; break;
        }
        i++;
    }

#ifdef DEBUG
    printf("*D* data = %8X,icase = %d\n",data,icase);
#endif

    if(!lst || !key)
        return NULL;

    if(nd = _findkey(lst,key,icase))
        data = nd->ln_Name;

    return data;
}

__regargs void LFFreeKey( struct List *lst, char key){
    struct Node *nd;

    if(nd = _findkey(lst,key,FALSE)){
        Remove(nd);
        FreeVec(nd);
    }
}

__regargs void LFFreeKeyList( struct List *lst ){
    struct Node *nd = GetHead(lst),*nnd;

    while(nd){
        nnd=GetSucc(nd);
        FreeVec(nd);
        nd = nnd;
    }
};
