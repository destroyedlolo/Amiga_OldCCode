/*****************************************************************************

                           LFDBuff.c
                        © LF Soft 1993

     Ces routines permettent d'obtenir un double-buffering d'une maniere
    simple. Inspiré des fonctions de PCQ.

struct Window *LFDB_OpenScreen( struct NewScreen * )
    Ouvre un écran comme le ferait OpenScreen(). Sur cette écran est ouverte
    une fenetre de type 'backdrop' & 'borderless' dont le pointeur est retourné.
    C'est sur cette fenetre que doivent ce faire les Gfx.
    Retourne NULL en cas d'erreur.

void LFDB_SwapBuffers( struct Window * )
    Echange les buffers ( celui affiché & celui on l'on trace ).
    /!\ Auccun test n'est fait pour savoir si la fentre a été ouverte avec
    LFDB_OpenScreen().

void LFDB_CloseScreen( struct Window * )
    Ferme l'écran ouvert par LFDB_OpenScreen() et libere les resources.

    14-11-1993: Création.

*****************************************************************************/

#include <LF.h>
#include <proto/intuition.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <Intuition/Intuition.h>
// #include <Intuition/Screens.h>
#include <exec/memory.h>

__regargs struct Window *LFDB_OpenScreen( struct NewScreen *ns ){
    struct Screen *s;
    struct Window *w;
    struct NewWindow nw;
    struct BitMap *bm;
    struct RastPort *rp;
    int i,j;

    if(!(s = OpenScreen(ns))) // Ouvre l'écran
        return(NULL);

    ShowTitle(s,FALSE); // La ligne de titre n'est pas affichée

        // Affectation des parametres de la fenetre
    nw.LeftEdge  = s->LeftEdge;
    nw.TopEdge   = s->TopEdge;
    nw.Width     = s->Width;
    nw.Height    = s->Height;
    nw.DetailPen = nw.BlockPen = -1;
    nw.IDCMPFlags= 0;
    nw.Flags     = WFLG_BACKDROP | WFLG_BORDERLESS | WFLG_ACTIVATE;
    nw.FirstGadget = nw.CheckMark = NULL;
    nw.Title     = "";
    nw.Screen    = s;
    nw.BitMap    = NULL;
    nw.Type      = CUSTOMSCREEN;

    if(!(w = OpenWindow ( &nw ))){  // Ouverture de la fenetre
        CloseScreen(s);
        return(NULL);
    }

        // Allocation de la nouvelle BitMap
    if(!(bm = AllocMem( sizeof( struct BitMap ), MEMF_PUBLIC ))){
        CloseWindow(w);
        CloseScreen(s);
        return(NULL);
    }

    *bm = s->BitMap;    // Copy des valeurs de la BM de l'écran

        // Initialisation des BitPlane
    for( i=0; i<bm->Depth; i++){
        if(!(bm->Planes[i] = AllocRaster(s->Width,s->Height))){
            if(i) for( j=0; i<i; i++)   // Liberation des rasters.
                FreeRaster(bm->Planes[j],s->Width,s->Height);
            CloseWindow(w);
            CloseScreen(s);
            return(NULL);
        }
    }
    rp = w->RPort;
    rp->BitMap = bm;

    return(w);
}

__regargs void LFDB_SwapBuffers( struct Window *w ){
    struct Screen *s;
    struct BitMap *bm1, *bm2;
    struct RastPort *rp;
    struct Plane *tmp[8];

    s = w->WScreen;
    rp = w->RPort;
    bm1 = rp->BitMap;
    bm2 = &(s->BitMap);

                //Echange des BitPlanes
    CopyMem(bm2->Planes,tmp        ,sizeof(tmp)); // 2  -> tmp
    CopyMem(bm1->Planes,bm2->Planes,sizeof(tmp)); // 1  -> 2
    CopyMem(tmp        ,bm1->Planes,sizeof(tmp)); // tmp-> 1

    ScrollVPort(&(s->ViewPort)); // Actualisation de l'écran
}

__regargs void LFDB_CloseScreen( struct Window *w ){
    struct Screen *s;
    struct BitMap *bm;
    int i;
    struct RastPort *rp;

    s = w->WScreen;
    rp = w->RPort;
    bm = rp->BitMap;
    rp->BitMap = &(s->BitMap);

    for(i=0; i< bm->Depth; i++)
        FreeRaster(bm->Planes[i],s->Width,s->Height);

    FreeMem(bm,sizeof(struct BitMap));
    CloseWindow(w);
    CloseScreen(s);
}
