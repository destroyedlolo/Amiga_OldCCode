/* How to create an ARP request */

#include <exec/types.h>
#include <string.h>
#include <arpBase.h>

APTR ArpBase=NULL;

char fname[80];
char dirname[120];
char nom_complet[200];
LONG reponse;

struct FileRequester Freq=
{"              Essais ",fname,dirname,NULL,NULL,NULL,NULL,NULL};

/* La suite n'est qu'un exemple */
void main()
{
ArpBase=(APTR)OpenLibrary("arp.library",0);
if (ArpBase==NULL)
        exit(FALSE);
reponse=FileRequest(&Freq);
CloseLibrary(ArpBase);
if(reponse)
        {
        printf("\n Chemin '%s'",dirname);
        printf("\n Nom du fichier '%s'",fname);
        }
exit(0);
}

