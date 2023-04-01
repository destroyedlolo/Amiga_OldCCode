#include <lf.h> // Juste pour les kputs()
#include <exec/types.h>
#include <intuition/intuition.h>
#include <intuition/classes.h>
#include <intuition/classusr.h>
#include <intuition/imageclass.h>
#include <intuition/gadgetclass.h>
#include <intuition/cghooks.h>
#include <intuition/icclass.h>
#include <utility/tagitem.h>
#include <utility/hooks.h>

#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/utility.h>
#include <clib/alib_protos.h>

extern ULONG DoSuperMethodA( struct IClass *, Object *, Msg);
extern ULONG DoMethod(Object *, ULONG, ...);

ULONG RenderTSTBut(Class *cl, struct Gadget *g, struct gpRender *msg);

#ifdef DEBUG
#define Deb(x) kputs(x);
#else
#define Deb(x)
#endif

struct TSTButDt {
    int bidon;
};

static __geta4 ULONG DispatchTSTBut( __a0 Class *, __a2 Object *, __a1 Msg);

Class *InitTSTClass( void ){ // Création de la classe
    Class *cl;

    if(cl = MakeClass( NULL,"gadgetclass", NULL, sizeof ( struct TSTButDt ), 0 ))
        cl->cl_Dispatcher.h_Entry = DispatchTSTBut;

    return cl;
}

static __geta4 ULONG DispatchTSTBut( __a0 Class *cl, __a2 Object *o, __a1 Msg msg){
    struct TSTButDt *inst;
    ULONG retval = FALSE;
    Object *object;

    switch (msg->MethodID){
    case OM_NEW: // Création du gadget
        Deb("OM_NEW");
        if(retval = (ULONG)(object = (Object *)DoSuperMethodA(cl, o, msg))){ // Init des données internes
            inst = INST_DATA(cl, object);
            inst->bidon = 0;
        }
        break;

    case GM_HITTEST: // Test si le gadget a réèlement été clické -> Oui pour les rectangles
        Deb("GM_HITTEST");
        retval = GMR_GADGETHIT;
        break;

    case GM_GOACTIVE:   // Le gadget devient actif
        Deb("GM_GOACTIVE");
        inst = INST_DATA(cl, o);
        if(((struct gpInput *)msg)->gpi_IEvent){
            ((struct Gadget *)o)->Flags |= GFLG_SELECTED;
            RenderTSTBut(cl, (struct Gadget *)o, (struct gpRender *)msg);
            retval = GMR_MEACTIVE;
        } else
            retval = GMR_NOREUSE;
        break;

    case GM_RENDER:   // Le gadget devient actif
        Deb("GM_RENDER");
        retval = RenderTSTBut(cl, (struct Gadget *)o, (struct gpRender *)msg);
        break;

    case GM_HANDLEINPUT:
        Deb("GM_HANDLEINPUT");
        {
            struct Gadget *g = (struct Gadget *)o;
            struct gpInput *gpi = (struct gpInput *)msg;
            struct InputEvent *ie = gpi->gpi_IEvent;
            inst = INST_DATA(cl, o);

            retval = GMR_MEACTIVE;

            if (ie->ie_Class == IECLASS_RAWMOUSE){
                switch (ie->ie_Code){
                case SELECTUP:
                    if ( ((gpi->gpi_Mouse).X < g->LeftEdge) ||
                         ((gpi->gpi_Mouse).X > g->LeftEdge + g->Width) ||
                         ((gpi->gpi_Mouse).Y < g->TopEdge) ||
                         ((gpi->gpi_Mouse).Y > g->TopEdge + g->Height) )
                        retval = GMR_NOREUSE | GMR_VERIFY;
                    else
                        retval = GMR_NOREUSE;
                    break;
                case MENUDOWN:
                    retval = GMR_REUSE;
                    break;
                default:
                    retval = GMR_MEACTIVE;
                }
            }
        }
        break;

    case GM_GOINACTIVE:
        ((struct Gadget *)o)->Flags &= ~GFLG_SELECTED;
        RenderTSTBut(cl, (struct Gadget *)o, (struct gpRender *)msg);
        break;

    case OM_SET:
        if ( FindTagItem(GA_Width,  ((struct opSet *)msg)->ops_AttrList) ||
                FindTagItem(GA_Height, ((struct opSet *)msg)->ops_AttrList) ||
                FindTagItem(GA_Top,    ((struct opSet *)msg)->ops_AttrList) ||
                FindTagItem(GA_Left,   ((struct opSet *)msg)->ops_AttrList) ) {
            struct RastPort *rp;
            struct Gadget *g = (struct Gadget *)o;

            WORD x,y,w,h;

            x = g->LeftEdge;
            y = g->TopEdge;
            w = g->Width;
            h = g->Height;

            inst = INST_DATA(cl, o);

            retval = DoSuperMethodA(cl, o, msg);

            if(rp = ObtainGIRPort( ((struct opSet *)msg)->ops_GInfo)){
                UWORD *pens = ((struct opSet *)msg)->ops_GInfo->gi_DrInfo->dri_Pens;
                SetAPen(rp, pens[BACKGROUNDPEN]); // Effacement de l'ancien gadget
                SetDrMd(rp, JAM1);
                RectFill(rp, x, y, x+w, y+h);

                    // Affiche un nouveau gadget
                DoMethod(o, GM_RENDER, ((struct opSet *)msg)->ops_GInfo, rp, GREDRAW_REDRAW);
                ReleaseGIRPort(rp);
            }
        } else
            retval = DoSuperMethodA(cl, o, msg);
        break;
    default: // C'est une méthode inconnue... sauf pour la superclasse peut être?
        retval = DoSuperMethodA(cl, o, msg);
        break;
    }
    return(retval);
}

ULONG RenderTSTBut(Class *cl, struct Gadget *g, struct gpRender *msg){
    struct TSTButDt *inst = INST_DATA(cl, (Object *)g);
    struct RastPort *rp = (msg->MethodID == GM_RENDER)?msg->gpr_RPort:ObtainGIRPort(msg->gpr_GInfo);
    ULONG retval = TRUE;
    UWORD *pens = msg->gpr_GInfo->gi_DrInfo->dri_Pens;

    if(rp){
        UWORD back, shine, shadow;

        if (g->Flags & GFLG_SELECTED){
            back   = pens[FILLPEN];
            shine  = pens[SHADOWPEN];
            shadow = pens[SHINEPEN];
        } else {
            back   = pens[BACKGROUNDPEN];
            shine  = pens[SHINEPEN];
            shadow = pens[SHADOWPEN];
        }
        SetDrMd(rp, JAM1);

        SetAPen(rp, back); // Efface l'ancien gadget
        RectFill(rp, g->LeftEdge, g->TopEdge, g->LeftEdge + g->Width, g->TopEdge + g->Height);

        SetAPen(rp, shadow); // Coté ombré
        Move(rp, g->LeftEdge + 1, g->TopEdge + g->Height);
        Draw(rp, g->LeftEdge + g->Width, g->TopEdge + g->Height);
        Draw(rp, g->LeftEdge + g->Width, g->TopEdge + 1);

        SetAPen(rp, shine); // Coté ombré
        Move(rp, g->LeftEdge, g->TopEdge + g->Height - 1);
        Draw(rp, g->LeftEdge, g->TopEdge);
        Draw(rp, g->LeftEdge + g->Width - 1, g->TopEdge);

        if (msg->MethodID != GM_RENDER)
            ReleaseGIRPort(rp);
    } else
        retval = FALSE;
    return retval;
}

    /************************************************************************
                                    Fonctions principales
     ************************************************************************/
struct Window *w=NULL;
Class *tstcl=NULL;
struct Gadget *tst=NULL;

void menage( void ){
    if(tst){
        RemoveGList(w, tst, -1);
        DisposeObject(tst);
    }
    if(tstcl) FreeClass(tstcl);
    if(w) CloseWindow(w);
}

int main(void){
    atexit(menage);

    if(!(w = OpenWindowTags(NULL, WA_Flags, WFLG_DEPTHGADGET | WFLG_DRAGBAR | WFLG_CLOSEGADGET | WFLG_SIZEGADGET,
      WA_IDCMP, IDCMP_CLOSEWINDOW, WA_Width, 640, WA_Height, 200, TAG_END)))
        exit(10);

    if(!(tstcl = InitTSTClass()))
        exit(11);

    if(!(tst = (struct Gadget *)NewObject(tstcl, NULL, GA_ID, 2L, GA_Top, (w->BorderTop) + 5L, GA_Left, (w->BorderLeft) + 5L,
      GA_Width, 50, GA_Height, 50, TAG_END)))
        exit(12);

    AddGList(w, tst, -1, -1, NULL);
    RefreshGList(tst, w, NULL, -1);

    { // Attente que l'on sorte
        ULONG done = FALSE;
        struct IntuiMessage *msg;

//        SetGadgetAttrs(tst, w, NULL, attr, value, TAG_DONE);

        while (done == FALSE)
        {
            WaitPort((struct MsgPort *)w->UserPort);
            while (msg = (struct IntuiMessage *)
               GetMsg((struct MsgPort *)w->UserPort))
            {
                if (msg->Class == IDCMP_CLOSEWINDOW)
                {
                    done = TRUE;
                }
                ReplyMsg((struct Message *)msg);
            }
        }
    }
}