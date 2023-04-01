/*
 *  BNF
 *
 *      Test pour l'évaluation d'une expression BNF
 *
 *  exp ::= exp + term | exp - term | term
 *  term ::= term * facteur | term / facteur | facteur
 *  facteur ::= {0 | ... | 9}
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

int fact( char *f ){ // Lecture d'un nombre
    char *x;
    int resultat;

    DEB(printf("\t\tfact(%s)\n",f));

    for(x=f,resultat=0;*x;x++){
        if(*x==' ')
            continue; // On saute les espaces...
        else if(*x>='0' && *x<='9') // C'est bien un chiffre
            resultat = resultat*10 + *x -'0';
        else {
            printf("Erreur: Les arabes n'avaient jamais penssé à mettre '%c' comme chiffre...\n",*x);
            exit(10);
        }
    }

    return resultat;
}

int term( char *t ){ // Evaluation d'un terme
    char *fin,*x;

    DEB(printf("\tterm(%s)\n",t));

        /*
         * Pour séparer le premier membre du second, on recherche
         * le dernier '+' ou '-' significatif
         */

    for(x=t,fin=0 ;*x;x++){
        if(*x=='*' || *x=='/')
            fin = x;
    }

    if(fin){ // Un signe a été trouvé
        char opp = *fin; // Mémorisation de l'opération
        *fin++=0;   // On sépare les 2 éléments et fin pointe sur le second

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
        }
    } else // Il n'y a qu'un facteur
        return fact(t);
}

int exp( char *e ){ // Evaluation d'une expression
    char *fin,*x;

    DEB(printf("exp(%s)\n",e));

        /*
         * Pour séparer le premier membre du second, on recherche
         * le dernier '+' ou '-' significatif
         */

    for(x=e,fin=0 ;*x;x++){
        if(*x=='+' || *x=='-')
            fin = x;
    }

    if(fin){ // Un signe a été trouvé
        char opp = *fin; // Mémorisation de l'opération
        *fin++=0;   // On sépare les 2 éléments et fin pointe sur le second

        switch(opp){
        case '+':
            return exp(e)+term(fin);
        case '-':
            return exp(e)-term(fin);
        }
    } else // Il n'y a qu'un term
        return term(e);
}

void main(int ac, char **av){
    char *buf;
    if(ac<2){
        puts("Syntaxe:\n\tBFN expr");
        exit(10);
    }

    if(!(buf = strdup(av[1]))){ // Copie de travail de la chaîne
        puts("Manque de mémoire...");
        exit(10);
    }

    printf("Le résultat de %s est :%d\n",av[1],exp(buf));

    free(buf);
    exit(0);
}