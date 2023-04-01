/****************************************************************************

                            icos.c

                      L.Faillie 12-11-1993

     Fonction cos & sin codées d'apres un tableau d'entiers.
    Resultats lors du calcul des boucles des cos de -180 a +180, avec
    calcul en simple précision & interface identique (Argument d'entrée SHORT,
    et FLOAT en sortie) :
        Erreur total relative = 0,0025 %
        temps pris par f2cos() = 45,38 % } Fonctions d'interface pour avoir des
        temps pris par i2cos() = 20,27 % } arguments identiques.
        temps pris par fcos()  = ????? % ( dprof ne l'affiche pas !! ).
        temps pris par icos()  = 05.77 %

20/11/1993 Passage en long

  ****************************************************************************/
#include <exec/types.h>
#include <stdlib.h>

    // tableau des cosinus de 0 a 90
    // 7fff = maximum ( 1 )
const WORD _tabcos[]={
0x7fff,0x7ffa,0x7feb,0x7fd2,0x7faf,0x7f82,0x7f4b,0x7f0a,0x7ec0,0x7e6b,0x7e0d,
0x7da4,0x7d32,0x7cb7,0x7c31,0x7ba2,0x7b09,0x7a67,0x79bb,0x7905,0x7846,0x777e,
0x76ad,0x75d2,0x74ee,0x7400,0x730a,0x720b,0x7103,0x6ff2,0x6ed9,0x6db6,0x6c8b,
0x6b58,0x6a1d,0x68d9,0x678d,0x6638,0x64dc,0x6378,0x620c,0x6099,0x5f1e,0x5d9c,
0x5c12,0x5a81,0x58e9,0x574b,0x55a5,0x53f9,0x5246,0x508c,0x4ecd,0x4d07,0x4b3b,
0x496a,0x4793,0x45b6,0x43d3,0x41ec,0x3fff,0x3e0d,0x3c17,0x3a1b,0x381c,0x3617,
0x340f,0x3203,0x2ff2,0x2dde,0x2bc6,0x29ab,0x278d,0x256c,0x2347,0x2120,0x1ef7,
0x1cca,0x1a9c,0x186c,0x1639,0x1405,0x11d0,0x0f99,0x0d61,0x0b27,0x08ed,0x06b2,
0x0477,0x023b,0x0000};

__regargs long LFicos( short x ){

    x = abs(x) % 360;
        // car cos(x) = cos (-x) & cos() est prériodique sur 360

    if(x>180)   // Normalisation sur 180
        x = 360-x;

    if(x>90)    // 90<x<180
        return(-(long)_tabcos[180-x]);
    else
        return((long)_tabcos[x]);
}
