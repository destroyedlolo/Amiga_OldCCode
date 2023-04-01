/* Test de rapidité.
 *  Créé par G.SOULET pour le comparatif des compilateur C pour
 *  l'AmigaNews n°48 (Juillet-Aout 1992).
 */

#include <stdio.h>

unsigned int bidon(unsigned long a, unsigned long b){
    int c,d,e,f,i;
    char *ptr;
    struct _tst {   /* Petite structure de test */
        int     v1;
        short   v2;
        int     v3;
    }toto, tata;

    /* Calculs complexes */
    c=4*a+(3*b+b);  /* Utilisation de décalage ?*/
    d=18*a-b/8;     /* Idem mais plus compliqué */
    e=(d-a)*(d-a);  /* Mise au carré: (d-a) ne doit être calculé qu'1 fois */

    f=a;    /* Affectation inutile */
    f=8;

    /* Elimination de calcul redondant */
    if(e==4)
        e=(d+a+f)/3;
    else
        d=(d+a+f)/3;

    /* Code jamais atteint */
    if(0) puts("Je suis un compilo NUL!");

    /* Boucle: Simplification dans les calculs d'indexes */
    for(i=0;i<10;i++){
        e=10*i;
        f=(int)(*(ptr+e));
        d=b; /* Devrait être sortie de la boucle */
    }

    /* Affectations successive sur des structures */
    toto.v1=0x1992;
    toto.v2=0x1993;
    toto.v3=0x1994;

    /* Copie de structure */
    tata=toto;

    toto.v2=(short)a;

    return (c*d*f+(tata.v1*tata.v2*tata.v3));
}

void main(void){
    unsigned long p1=4,res;

    res = bidon(p1,5);

    printf("Resultat =%d\n",res);
}