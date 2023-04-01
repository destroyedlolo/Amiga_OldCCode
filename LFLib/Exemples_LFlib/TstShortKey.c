/****************************************************************************

                        TstShortKey.c

                        © LFSoft 1994

            Teste des fonctions de la LF.lib v3.07+ .
    Quelques manips. simples sur l'utilisation des fonctions.

****************************************************************************/

#include <LF.h>
struct List list;

void main( void ){
    struct Node *nd;

    NewList(&list);

        // Crée 2 nodes.
    printf("resultat LFAddKey( create 'a' ) = %d\n",
    LFAddKey( &list, 'a', LFK_MODE,LFKM_OCREATE,LFK_DATA,6,TAG_DONE)
    );
    printf("resultat LFAddKey( create 'z' ) = %d\n",
    LFAddKey( &list, 'z', LFK_MODE,LFKM_OCREATE,LFK_DATA,50,TAG_DONE)
    );
    printf("resultat LFAddKey( create 'b' ) = %d\n",
    LFAddKey( &list, 'b', LFK_MODE,LFKM_OCREATE,LFK_DATA,8,TAG_DONE)
    );

        // Tentative d'écraser un node
    printf("resultat LFAddKey( create 'a' ) -> erreur = %d\n",
    LFAddKey( &list, 'a', LFK_MODE,LFKM_OCREATE,LFK_DATA,0,TAG_DONE)
    );

        // Modif du node 'a'
    printf("resultat LFAddKey( mod 'a' ) = %d\n",
    LFAddKey( &list, 'a', LFK_MODE,LFKM_MODIFY,LFK_IGNORE_CASE, TRUE, TAG_DONE)
    );

        // recherche du node 'A'. -> 6 car 'a' à le bit LFK_IGNORE_CASE activé
    printf("\nresultat LFFindKey( 'A' ) = %08X\n",
    LFFindKey( &list, 'A', TAG_DONE)
    );
        // Recherche du node 'B' ( ignore les majuscules )
    printf("resultat LFFindKey( 'B' IC ) = %08X\n",
    LFFindKey( &list, 'B', LFK_IGNORE_CASE, TRUE, TAG_DONE)
    );

        // Idem mais sensible au majuscules -> Erreur
    printf("resultat LFFindKey( 'B' ) = %08X\n",
    LFFindKey( &list, 'B', TAG_DONE)
    );

        // Idem mais sensible au majuscules -> 'B001'
    printf("resultat LFFindKey( 'B' def 'B001')= %08X\n",
    LFFindKey( &list, 'B', LFK_DATA,0xb001, TAG_DONE)
    );

    nd = GetHead(&list);
    puts("\nContenu de la liste\n");
    while(nd){
        printf("node %08X, flags = %02x, key = %02x, data= %08X\n",
        nd,nd->ln_Type,nd->ln_Pri,nd->ln_Name);

        nd=GetSucc(nd);
    }

    puts("Suppression de 'z'");
    LFFreeKey(&list,'z');

    nd = GetHead(&list);
    puts("\nContenu de la liste\n");
    while(nd){
        printf("node %08X, flags = %02x, key = %02x, data= %08X\n",
        nd,nd->ln_Type,nd->ln_Pri,nd->ln_Name);

        nd=GetSucc(nd);
    }

    LFFreeKeyList(&list);
}
