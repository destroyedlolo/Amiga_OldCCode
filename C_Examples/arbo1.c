/*
 Ce programme cree une arboressance a partir d'une expression.

    Par L.Faillie

*/

#include <LF.h>
#include <ctype.h>

// noeud de l'arbre

struct noeud {
    char type;
    struct noeud *opp1; // Opperandes
    struct noeud *opp2;
};


// pour stocker les variables

struct {
    char type;
    char nvar;  // Les variables n'ont qu'un seul caractere pour leur nom (a,b,c,...)
} var;


// pour stocker les constantes

struct {
    char type;
    int nbre;
} nbre;

enum {CST,VAR,PLUS,MOINS,MULT,DIV};
// Ces constantes definiront le type d'un noeud. Si ce type > a PLUS, il s'agit
// alors d'un opperateur ( a 2 opperandes ).

enum {NON=1,OUI,ERREUR};
// Pour les variables servant a sortir des boucles

char exp[256]; // Cette chaine contiendra l'expression


void passe(int *i){
// passe les paires de ( ).
#ifdef DEBUG
    printf("\nPasse ...");
#endif
    while(exp[++(*i)]){
#ifdef DEBUG
    printf("%c",exp[*i]);
    fflush(stdout);
#endif
        switch(exp[*i]){
        case '(':
            passe(i);
            break;
        case ')':
            *i++;
#ifdef DEBUG
    puts(" ... passé");
#endif
            return;
        }
    }
    puts("Erreur : '(' sans ')'");
    exit(0);
}

int reflechis(i,nd)
int i; // Indice a partir duquel il faut reflechir
struct noeud *nd; // noeud où doit etre stocké les infos
{
    int j,bidon=0;

    j=i;

// On lit d'abors l'opperateur s'il existe
    do {
#ifdef DEBUG
    printf("%c",exp[j]);
    fflush(stdout);
#endif
        switch(exp[j]){
        case '(':
            passe(&j);
            break;
        case 0:
        case ')':   // On était dans une sous expression
            bidon=NON;
            break;
        case '+':
        case '-':
        case '*':
        case '/':
            bidon=OUI;
            break;
        }
    j++;
    } while(!bidon);

    if(bidon==OUI)
        switch(exp[j])
}

void main(){
    struct noeud racine;

    puts("Quelle est l'expression ?");
    scanf("%255s",exp);

    reflechis(0,&racine);
}
