/*****************************************************************************
                        Tst_DBuff.c

                       © LFSoft 1993

     Test du Double Buffering de la LF.lib.

*****************************************************************************/

#include <LF.h>
#include <stdlib.h>
#include <graphics/gfx.h>
#include <proto/graphics.h>

#if LF_H < 304
#include <proto/Intuition.h>
__regargs struct Window *LFDB_OpenScreen( struct NewScreen * );
__regargs void LFDB_SwapBuffers( struct Window * );
__regargs void LFDB_CloseScreen( struct Window * );
#endif

struct NewScreen NewScreenStructure = {
        0,0,
        640,256,
        1,
        0,1,
        HIRES,
        CUSTOMSCREEN,
        NULL,
        " Tst DB",
        NULL,
        NULL
};

void main(){
    struct Window *w;
    int x,y=128,rx=20,ry=10;
    char r;

    if(!(w=LFDB_OpenScreen(&NewScreenStructure))){
        LFatal("Impossible d'ouvrir l'écran");
        exit(20);
    }

    for(x=0;x<639;x++){
        r=(char)rand();
        if(r & 1)
            y=(y<255)?y+1:255;
        else
            y=y?y-1:0;
        if(r & 2)
            rx+=2;
        else
            rx=(rx>10)?rx-2:10;
        if(r & 4)
            ry++;
        else
            ry=(ry>10)?ry-1:10;

    // Gfx
        SetRast(w->RPort,0);
        DrawEllipse( w->RPort, x, y, rx, ry);

        LFDB_SwapBuffers(w);
    }
    LFDB_CloseScreen(w);
}
