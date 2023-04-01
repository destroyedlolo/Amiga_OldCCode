// Test l'utilisation du requester Arp
// Test l'auto ouverture de l'Arp

// © LF Soft 5-Sep-1992

#include <exec/types.h>
#include <string.h>
#include <libraries/arpBase.h>

char fname[80];
char dirname[120];
char nom_complet[200];
LONG reponse;

struct FileRequester Freq=
{"              Essais ",fname,dirname,NULL,NULL,NULL,NULL,NULL};

/* La suite n'est qu'un exemple */
void main()
{
reponse=FileRequest(&Freq);
if(reponse)
        {
        printf("\n Chemin '%s'",dirname);
        printf("\n Nom du fichier '%s'\n",fname);
        }
exit(0);
}

