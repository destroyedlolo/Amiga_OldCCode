//
// Fichier LF.h
//
//      par L.Faillie
//
// Quelques #defines utiles, prototype des fonctions de LF.lib
// & utilisation de bibliotheques partageables.
//
//   Historique
//  ------------
//  V1.x    ??????????  L.Faillie   Disparue lors du crache de mon HD.
//  V2.0    08/10/1992  L.Faillie   Recr�ation complette de ce fichier
//                                  Utilisation de Req, Arp & PP
//  V2.01   19/10/1992  L.Faillie   Ajout de l'Iff.library
//  V2.02   09/11/1992  L.Faillie   Ajout de LFDecrypt()
//  V2.03   18/04/1993  L.Faillie   Modif de LFDecrypt() & cr�ation de LFCrypt()
//  V2.04   18/06/1993  L.Faillie   Ajout de LFDecryptB() & LFCryptB()
//  V2.05   09/08/1993  L.Faillie   Support des GadToolsBox, NoFrags,
//                                  modif pour support de Dice V2.07.54R
//  V3.00   14/08/1993  L.Faillie   modif de la lib pour supporter les pragmas,
//                                  Ajout de LFatal().
//  V3.01   17/08/1993  L.Faillie   Support de la ReqTools.
//  V3.02   02/10/1993  L.Faillie   Support des librairies debug & ddebug
//  V3.03   12/11/1993  L.Faillie   Ajout de LFicos(), LFisin() & LFInJoy()
//  V3.04   15/11/1993  L.Faillie   Ajout des fonctions du Double-Buffering.
//          20/11/1993  L.Faillie   Passage de LFicos() en long
//  V3.05   04/02/1994  L.Faillie   Tous les boutons des joysticks peuvent etre
//                                  test�, supprime un bug caus� car LFInJoy().
//  v3.06   04/03/1994  L.Faillie   Support de l'AmigaGuide.library
//  v3.07   26/03/1994  L.Faillie   Ajout de la gestion des ShortKey. Supprime
//                                  un bug dans LFStrDos()
//

#ifndef LF_H
#define LF_H 307

#include <exec/types.h>
#include <stdlib.h>
#include <stdio.h>

extern void OS2_0( void ); // Cette fonction n'est d�finie nul part !!

// Pour l'utilisation des bibliotheques externes

#ifndef __DICE_INLINE
    #define NO_PRAGMAS
#endif

#ifdef U_ARP
// Appel des fonctions de l'ARP.library a la place des fonctions standards
    #ifdef Fast_LF
        #define puts    Puts
        #define printf  LFPrintf
        #define sprintf LFSPrintf
    #endif
    // fonctions cr��es dans l'LF.lib.
extern __stkargs int LFPrintf(const char *,...);
                // Remplacement pour printf, il utilise le printf
                // d'Arp.library. Attention, les entiers sont par defaut
                // sur 16 bits, les flottants ne sont pas acc�cible.

extern __stkargs int LFSPrintf(char *,const char *,...);
                // Remplacement de sprintf. M�me condition que LFPrintf

    #define C_Args  __stkargs
    #include <libraries/ArpBase.h>
#endif

#ifdef U_REQ
    // Faire un #define __stdargs pour supprimer les warnings ( sauf options -mr+ )
    #ifndef __stdargs
        #define __stdargs __stkargs
    #endif
    #include <libraries/ReqBase.h>
    #include <clib/reqproto.h>
#endif

#ifdef U_PP
    #include <dos/dos.h>
    #include <libraries/PPBase.h>
    #include <clib/powerpacker_protos.h>
#endif

#ifdef U_IFF
    #include <libraries/Iff.h>
#endif

#ifdef U_GTX
    #include <GadToolsBox/forms.h>
    #include <GadToolsBox/gtxbase.h>
    #include <GadToolsBox/gui.h>
    #include <GadToolsBox/hotkey.h>
    #include <GadToolsBox/prefs.h>
    #include <GadToolsBox/textclass.h>
    #include <clib/Gtx_protos.h>
#endif

#ifdef U_NOFRAG
    #include <libraries/nofrag.h>
    #include <clib/nofrag_protos.h>
#endif

#ifdef U_REQTOOLS
    #include <libraries/reqtools.h>
    #include <proto/reqtools.h>
#endif

#ifdef U_AMIGAGUIDE
    #include <libraries/amigaguide.h>
    #include <proto/amigaguide.h>
#endif

    // Support de la (d)debug.lib
#ifdef DEBUG
    char DGetChar( void );
    int DGetNum( void );
    int DMayGetChar( void );
    char DPutChar( char );
    int KCmpStr(char *, char *);
    int dputchar( char );
    int dputs( char * );
    int dprintf( char *,...);
    char KGetChar( void );
    int KGetNum( void );
    int KMayGetChar( void );
    char KPutChar( char );
    int kputchar( char );
    int kputs( char * );
    int kprintf( char *,...);
#endif

// Fonctions propres � l'LF.lib

#define until(x)    while(!(x))
#define taille(t)   (sizeof(t)/sizeof(t[1]))
#ifndef FOREVER
#define FOREVER     for(;;)
#endif

extern __stkargs char *cheure( void ); // retourne un ptr sur une chaine contenant l'heure & la date courante en fran�ais
extern char heure[];   // Chaine contenant l'heure ( retourn� par cheure() );
extern const char *mois[], *jours[];
                // Cha�ne contenant les mois & les jours

extern __regargs char exist(char *);
                // Teste l'existence d'un fichier,
                // renvoit 1 si oui, 0 si non.

extern __regargs char *LFgets(FILE *,char *,size_t);
                // Lit une cha�ne & efface un �ventuel \n

extern __regargs char *LFStrDos(const char *,const char *);
                // Concatene la 1er cha�ne ( le path ) avec la seconde
                // ( Le fichier ) en intercallant un '/' si n�cessaire.

extern __regargs char *LFCrypt(char *,char *,ULONG *);
                // Crypte la premiere cha�ne avec le mot de passe contenue dans
                // la seconde. Renvoie le pointeur sur la cha�ne crypt�e. La
                // variable ULONG contiendra la longueur de la cha�ne.
                // La cha�ne renvoy� est allou�e dynamiquement donc la m�moire
                // doit etre lib�r�e par free() ( exit() le fait automatiquement ).
                //  ^   Attention : Les cha�nes doivent IMPERATIVEMENT ce finir par
                // /!\ un 0 sinon de la m�moire innocente vas etre corrompue !!
                // �T� Le 0 est lui aussi crypter donc une copy doit se faire avec
                // memcopy(dst,src,len+1).

extern __regargs char *LFDecrypt(const char *,const char *,ULONG len);
                // Decrypt la 1er cha�ne avec le mot de passe pass� dans la seconde.
                // Len contient la longeur ( sans le 0 ).

extern __regargs char *LFCryptB(char *msg,char *pw,char *buff,ULONG len);
/* Revoie des donn�es crypt�es. Contrairemant a LFCrypt() cette fonction peut */
/* aussi traiter les donn�es binaire. */
// len contient la taille des donn�es. /!\ different de LFCrypt() & len contient
// le 0 en cas de cha�ne (len =strlen()+1).
// buffer contiendra le resultat. Evidement, il doit etre asses grand pour contenir
// les donn�es.

extern __regargs char *LFDecryptB(const char *msg,const char *pw,char *buff,ULONG len);
// Routine de decryptage. Renvoie les donn�es decrypt�es.
// buff est le buffer o� ce trouvera le resultat.
// /!\ len contient aussi le 0 en cas de cha�ne. On peut l'optenir grace a
// un sizeof() sur le tableau de donn�e.

extern __regargs void LFatal(char *msg);
// Affiche une alerte.

extern __regargs long LFicos( short ); // Fonction cos() cod�e d'apres un
                                        // d'entiers tableau
                                        // /!\ angle en degr�e
#define LFisin(x) LFicos((x)-90)
#define LFicosMAX   0x7fff

    // Gestion des joysticks
extern __regargs UBYTE LFInJoy( UBYTE );

#define LFJ_FIRE   1
#define LFJ_RIGHT  2
#define LFJ_LEFT   4
#define LFJ_DOWN   8
#define LFJ_UP    16
#define LFJ_FIRE2 32
#define LFJ_FIRE3 64
/*  Pour que F2 & F3 soient test�s correctement, il faut d'abors initialiser les
 * bits DATR? de potgo avec AllocPotBits().
 */
#define LFJ_MOUSE 1
#define LFJ_JOY   2

    // Gestion du Double buffering
/*   ^
 *  /!\ Ne marche pas sous 3.0.
 *  �T�
 */
#include <intuition/intuition.h>

extern __regargs struct Window *LFDB_OpenScreen( struct NewScreen * );
/*    Ouvre un �cran comme le ferait OpenScreen(). Sur cette �cran est ouverte
      une fenetre de type 'backdrop' & 'borderless' dont le pointeur est retourn�.
      C'est sur cette fenetre que doivent ce faire les Gfx.
      Retourne NULL en cas d'erreur.
*/

extern __regargs void LFDB_SwapBuffers( struct Window * );
/*    Echange les buffers ( celui affich� & celui on l'on trace ).
      /!\ Auccun test n'est fait pour savoir si la fen�tre a �t� ouverte avec
      LFDB_OpenScreen().
*/

extern __regargs void LFDB_CloseScreen( struct Window * );
/*    Ferme l'�cran ouvert par LFDB_OpenScreen() et libere les resources.
*/

    // Gestion des ShortKeys
/* Ces routines ont �t� concues pour la gestions des ShortKeys des gadgets d'
 * une fenetre, mais peuvent �tre utiles a d'autres fins.
 */
#ifndef UTILITY_TAGITEM_H
#include <utility/tagitem.h>
#endif  /* UTILITY_TAGITEM_H */

#ifndef CLIB_ALIB_PROTOS_H
#include <clib/alib_protos.h>
#endif

#ifndef LISTS_H
#include <lists.h>
#endif

extern __stkargs int LFAddKey( struct List *lst, char key, Tag tags,...);
/* Ajoute une touche dans la liste */
extern __stkargs void *LFFindKey( struct List *lst, char key, Tag tags,...);
/* Cherche une touche dans la liste */
extern __regargs void LFFreeKey( struct List *lst, char key);
/* Supprime une touche */
extern __regargs void LFFreeKeyList( struct List *lst );
/* Libere une liste cr�er par des LFAddKey() */

// Liste des tags
#define LF_Base   TAG_USER
#define LFK_DATA    (LF_Base + 1)
    // ti_data : void *
    // LFAddKey(): D�fini les donn�es renvoy� par LFFindKey()
    // LFFindKey(): Donn�es renvoy�es par d�faut.
#define LFK_MODE    (LF_Base + 2)
    // pour LFAddKey() seulement.
    // ti_data : contient une des d�finitions suivantes:
    #define LFKM_MODIFY     (0L)
        // remplace la d�finition actuelle, en cr�e une entr�e dans la liste si
        //la touche n'est pas encore d�finie
    #define LFKM_OCREATE    (1L)
        // Seulement si la touche n'est pas encore d�fini
#define LFK_IGNORE_CASE     (2L)
    // ti_data : BOOL
    // si TRUE, ignore les majuscules/ minuscules
    // LFAddKey(): Attention, si ce flag est omis, la recherche se fait d'abors
    // en mode ignore avant d'ajouter.

// Erreurs retourn�s par LFAddKey()
enum { LFAK_OK=0,   // pas d'erreur
       LFAK_PAM ,   // pas assez de m�moire
       LFAK_NAE ,   // Le node existe d�j�
       LFAK_DIV     // Donn�e invalide ( LFK_DATA Absant ou contenant -1, lst ou key null ).
};

#endif

