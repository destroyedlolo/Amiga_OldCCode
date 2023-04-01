/*
 *  BNF2
 *
 *      Test pour l'�valuation d'une expression BNF
 *
 *  exp ::= exp + term | exp - term | term
 *  term ::= term * facteur | term / facteur | facteur
 *  facteur ::= ( exp ) | nombre
 *  nombre ::= {0 | ... | 9}
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Uniquement pour strdup()

#define DEBUG

#ifdef DEBUG
    #define DEB(x) {x;}
#else
    #define DEB(x)
#endif

int exp( char * );

char *rch( char *ch, const char *sep ){
/*  Recherche dans ch le dernier caract�re significatif de sep.
 */
    char *fin, *x, *y;
    int ignore=0;   // Nombre de parentaises ouvrantes.

    for( x=ch, fin=0; *x; x++ ){
        if(!ignore){
            for(y=sep;*y;y++) //Recherche si un des s�parteurs est trouv�
                if(*x == *y) fin = x;
        } else {
            if( *x == ')' ){ // Est-ce une fermeture attendue ?
                ignore--;
                continue;
            }
        }
        if( *x == '(' )
            ignore++;
        else if( *x == ')' ){ // Fermeture en trops.
            fin = x;
            break;  // Donc on sort
        }
    }

    return fin;
}

int nombre( char *n ){
/* Evaluation d'un nombre
 *  nombre ::= {0 | ... | 9}
 */
    char *x;
    int resultat;

    DEB(printf("\t\t\tnombre(%s)\n",n));

    for(x=n,resultat=0;*x;x++){
        if(*x==' ')
            continue; // On saute les espaces...
        else if(*x>='0' && *x<='9') // C'est bien un chiffre
            resultat = resultat*10 + *x -'0';
        else {
            printf("Erreur: Les arabes n'avaient jamais penss� � mettre '%c' comme chiffre...\n",*x);
            exit(10);
        }
    }

    return resultat;
}

int fact( char *f ){
/* Evaluation d'un facteur
 *  facteur ::= ( exp ) | nombre
 */
    char *debut,*fin;

    DEB(printf("\t\tfact(%s)\n",f));

    for(debut=f; *debut; debut++)   // Ellimination des espaces
        if(*debut!=' ' && *debut!='\t') break;

    if(*debut=='('){ // C'est une expression
        fin = rch(++debut,")");
        if(*fin == ')'){
            *fin = 0;
            if(*++fin){ // Il reste quelque chose apres le ')'
                puts("Erreur de syntaxe!");
                exit(5);
            }
            return exp(debut);
        } else {
            printf("Caract�re non attendu :%c\n",*fin);
            exit(5);
        }
    } else
        return nombre(debut);
}

int term( char *t ){
/* Evaluation d'un term
 *  term ::= term * facteur | term / facteur | facteur
 */
    char *fin;

    DEB(printf("\tterm(%s)\n",t));

        // Pour s�parer 'term', recherche du dernier '+' ou '-' significatif
    fin=rch(t,"*/");

    if(fin){ // Un signe a �t� trouv�
        char opp = *fin; // M�morisation de l'op�ration
        *fin++=0;   // On s�pare les 2 �l�ments et fin pointe sur le second

        switch(opp){
        case '*':
            return term(t)*fact(fin);
        case '/':{
                int bidon=fact(fin); // On est pas chez micro-sucker!
                if(!bidon){
                    puts("Ha ha!! Division par 0...");
                    exit(10);
                }
                return term(t)/bidon;
            }
        default:
            printf("Caract�re non attendu :%c\n",opp);
            exit(5);
        }
    } else // Il n'y a qu'un facteur
        return fact(t);
}

int exp( char *e ){
/* Evaluation d'une expression
 *   exp ::= exp + term | exp - term | term
 */
    char *fin;

    DEB(printf("exp(%s)\n",e));

        // Pour s�parer 'term', recherche du dernier '+' ou '-' significatif
    fin=rch(e,"+-");

    if(fin){
        char opp = *fin; // M�morisation de l'op�ration
        *fin++=0; // S�paration des 2 �l�ments

        switch(opp){
        case '+':
            return exp(e)+term(fin);
        case '-':
            return exp(e)-term(fin);
        default:
            printf("Caract�re non attendu :%c\n",opp);
            exit(5);
        }
    } else // Il n'y a qu'un term
        return term(e);
}

void main(int ac, char **av){
    char *buf;
    if(ac<2){
        puts("Syntaxe:\n\tBFN2 expr");
        exit(10);
    }

    if(!(buf = strdup(av[1]))){ // Copie de travail de la cha�ne
        puts("Manque de m�moire...");
        exit(10);
    }

    printf("Le r�sultat de %s est :%d\n",av[1],exp(buf));

    free(buf);
    exit(0);
}
