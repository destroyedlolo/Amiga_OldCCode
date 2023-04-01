/* Animated Gadget */

#include <exec/types.h>
#include <graphics/gfxbase.h>
#include <exec/memory.h>
#include <intuition/intuition.h>

struct GfxBase *GfxBase=NULL;
struct IntuitionBase *IntuitionBase=NULL;
struct Screen *Screen=NULL, *OpenScreen();
struct Window *Window=NULL;
struct IntuiMessage *Message;

#define RP Window->RPort

__chip USHORT ImageData1[] = {
        0xFFFF,0xFFFF,0x8000,0x0001,0x8000,0x0001,0x8000,0x0001,
        0x8007,0xF001,0x801F,0xFC01,0x807F,0xFF01,0x80FF,0xFF81,
        0x81FF,0xFFC1,0x83C7,0xE3E1,0x83BB,0xDDE1,0x877D,0xBEF1,
        0x876D,0xB6F1,0x8F45,0xA2F9,0x8FAB,0xD5F9,0x8FC7,0xE3F9,
        0x8FFF,0xFFF9,0x8FFF,0xFFF9,0x8FFF,0xFFF9,0x8FFF,0xFFF9,
        0x87FF,0xFEF1,0x87EF,0xFDF1,0x83F3,0xF9E1,0x83F8,0x03E1,
        0x81FF,0xFFC1,0x80FF,0xFF81,0x807F,0xFF01,0x801F,0xFC01,
        0x8007,0xF001,0x8000,0x0001,0xFFFF,0xFFFF,0x0000,0x0000,
        0x7FFF,0xFFFE,0x7FFF,0xFFFE,0x7FFF,0xFFFE,0x7FFF,0xFFFE,
        0x7FFF,0xFFFE,0x7FFF,0xFFFE,0x7FFF,0xFFFE,0x7FFF,0xFFFE,
        0x7FFF,0xFFFE,0x7FFF,0xFFFE,0x7FFF,0xFFFE,0x7FEF,0xF7FE,
        0x7FC7,0xE3FE,0x7FEF,0xF7FE,0x7FFF,0xFFFE,0x7FFF,0xFFFE,
        0x7FFF,0xFFFE,0x7FFF,0xFFFE,0x7FFF,0xFFFE,0x7FFF,0xFEFE,
        0x7FEF,0xFDFE,0x7FF3,0xF9FE,0x7FF8,0x03FE,0x7FFF,0xFFFE,
        0x7FFF,0xFFFE,0x7FFF,0xFFFE,0x7FFF,0xFFFE,0x7FFF,0xFFFE,
        0x7FFF,0xFFFE,0x0000,0x0000
};

struct Image Image1 = {
        0,0,    /* XY origin relative to container TopLeft */
        32,31,  /* Image width and height in pixels */
        2,      /* number of bitplanes in Image */
        ImageData1,     /* pointer to ImageData */
        0x0003,0x0000,  /* PlanePick and PlaneOnOff */
        NULL    /* next Image structure */
};

__chip USHORT ImageData2[] = {
        0xFFFF,0xFFFF,0x8000,0x0001,0x8000,0x0001,0x8000,0x0001,
        0x8007,0xF001,0x801F,0xFC01,0x807F,0xFF01,0x80FF,0xFF81,
        0x81FF,0xFFC1,0x83C7,0xE3E1,0x839B,0xDDE1,0x872D,0xAEF1,
        0x8745,0x86F1,0x8F6D,0xAEF9,0x8FBB,0xDDF9,0x8FC7,0xE3F9,
        0x8FFF,0xFFF9,0x8FFF,0xFFF9,0x8FFF,0xFFF9,0x8FFF,0xFFF9,
        0x87FF,0xFFF1,0x87FF,0xFFF1,0x83FF,0xFFE1,0x83FF,0xFFE1,
        0x81FF,0xFFC1,0x80FF,0xFF81,0x807F,0xFF01,0x801F,0xFC01,
        0x8007,0xF001,0x8000,0x0001,0xFFFF,0xFFFF,0x0000,0x0000,
        0x7FFF,0xFFFE,0x7FFF,0xFFFE,0x7FFF,0xFFFE,0x7FFF,0xFFFE,
        0x7FFF,0xFFFE,0x7FFF,0xFFFE,0x7FFF,0xFFFE,0x7FFF,0xFFFE,
        0x7FFF,0xFFFE,0x7FFF,0xFFFE,0x7FFF,0xFFFE,0x7FFF,0xFFFE,
        0x7FFF,0xFFFE,0x7FFF,0xFFFE,0x7FFF,0xFFFE,0x7FFF,0xFFFE,
        0x7FFF,0xFFFE,0x7FFF,0xFFFE,0x7FFC,0x1FFE,0x7FF9,0xC4FE,
        0x7FE7,0xF0FE,0x7FFF,0xF9FE,0x7FFF,0xFFFE,0x7FFF,0xFFFE,
        0x7FFF,0xFFFE,0x7FFF,0xFFFE,0x7FFF,0xFFFE,0x7FFF,0xFFFE,
        0x7FFF,0xFFFE,0x0000,0x0000
};

struct Image Image2 = {
        0,0,    /* XY origin relative to container TopLeft */
        32,31,  /* Image width and height in pixels */
        2,      /* number of bitplanes in Image */
        ImageData2,     /* pointer to ImageData */
        0x0003,0x0000,  /* PlanePick and PlaneOnOff */
        NULL    /* next Image structure */
};

struct Gadget Gadget1 = {
        NULL,   /* next gadget */
        19,27,  /* origin XY of hit box relative to window TopLeft */
        32,31,  /* hit box width and height */
        GADGHIMAGE+GADGIMAGE,   /* gadget flags */
        RELVERIFY, /* activation flags */
        BOOLGADGET,     /* gadget type flags */
        (APTR)&Image1,  /* gadget border or image to be rendered */
        (APTR)&Image2,  /* alternate imagery for selection */
        NULL,   /* first IntuiText structure */
        NULL,   /* gadget mutual-exclude long word */
        NULL,   /* SpecialInfo structure */
        1,   /* user-definable data */
        NULL    /* pointer to user-definable data */
};

#define GadgetList1 Gadget1

struct NewWindow NewWindowStructure1 = {
        187,60, /* window XY origin relative to TopLeft of screen */
        112,66, /* window width and height */
        0,1,    /* detail and block pens */
        GADGETUP+CLOSEWINDOW,        /* IDCMP flags */
        WINDOWSIZING+WINDOWDRAG+WINDOWDEPTH+WINDOWCLOSE,        /* other window flags */
        &Gadget1,       /* first gadget in gadget list */
        NULL,   /* custom CHECKMARK imagery */
        "Es",   /* window title */
        NULL,   /* custom screen pointer */
        NULL,   /* custom bitmap */
        5,5,    /* minimum width and height */
        640,200,        /* maximum width and height */
        WBENCHSCREEN    /* destination screen type */
};


/* end of PowerWindows source generation */
VOID FinProg(char *chaine) {
        puts(chaine);
        exit(0);
}

void ferme_tout( void ){
        if(Window)
            CloseWindow(Window);
        if (GfxBase)
                CloseLibrary(GfxBase);
        if (IntuitionBase)
                CloseLibrary(IntuitionBase);
}

main() {

int fin=0,im=0,NuGadg;
long class;

atexit(ferme_tout);

if ((GfxBase = (struct GfxBase *)
        OpenLibrary("graphics.library",0)) == 0)
        FinProg("Impossible ouvrir graphics.library");

if ((IntuitionBase = (struct IntuitionBase *)
        OpenLibrary("intuition.library",0)) == 0)
        FinProg("Impossible ouvrir intuition.library");

if ((Window =(struct Window *) OpenWindow(&NewWindowStructure1))==0)
                FinProg("Impossible ouvrir Window");


while(!fin)
{
    Delay(40);
    DrawImage(RP,((im=!im)?&Image2:&Image1),52,27);
    Gadget1.GadgetRender=((im)?&Image1:&Image2);
    Gadget1.SelectRender=((im)?&Image2:&Image1);
    RefreshGadgets(&Gadget1,Window,NULL);

        while(Message = (struct IntuiMessage *)GetMsg(Window->UserPort))
        {
                NuGadg=((struct Gadget *)Message->IAddress)->GadgetID;
                class=Message->Class;
                ReplyMsg(Message);

                if (class==GADGETUP)
                    puts("Gadget up!");
                else if(class==CLOSEWINDOW) fin=1;
        }
}
FinProg("Au revoir");

}


