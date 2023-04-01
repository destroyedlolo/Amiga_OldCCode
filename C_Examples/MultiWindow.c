/***********************************************************************
                        MultiWindow.c

            Teste pour gerer plusieurs fenetres depuis 1 programmes.

***********************************************************************/

#include <intuition/intuition.h>
#include <stdio.h>
#include <stdlib.h>
#include <LF.h>

struct IntuiMessage *IM = NULL;
struct Window *win1=NULL, *win2=NULL;

// Ces definitions sont partager avec la seconde fenetre.

SHORT Border1_Vectors1[] = {
        0,0,
        138,0,
        138,14,
        0,14,
        0,0
};
struct Border Border1_1 = {
        -2,-1,  /* XY origin relative to container TopLeft */
        3,0,JAM1,       /* front pen, back pen and drawmode */
        5,      /* number of XY vectors */
        Border1_Vectors1,        /* pointer to XY vectors */
        NULL    /* next border in list */
};

struct IntuiText IText1_1 = {
        3,0,JAM2,       /* front and back text pens, drawmode and fill byte */
        32,2,   /* XY origin relative to container TopLeft */
        NULL,   /* font pointer or NULL for default */
        "Gadget1",      /* pointer to text */
        NULL    /* next IntuiText structure */
};

struct Gadget gadw1_1 = {
        NULL,   /* next gadget */
        19,14,  /* origin XY of hit box relative to window TopLeft */
        135,13, /* hit box width and height */
        NULL,   /* gadget flags */
        RELVERIFY,      /* activation flags */
        BOOLGADGET,     /* gadget type flags */
        (APTR)&Border1_1,        /* gadget border or image to be rendered */
        NULL,   /* alternate imagery for selection */
        &IText1_1,       /* first IntuiText structure */
        NULL,   /* gadget mutual-exclude long word */
        NULL,   /* SpecialInfo structure */
        1,      /* user-definable data */
        NULL    /* pointer to user-definable data */
};

#define 1GadgetList1 gadw1_1

struct NewWindow NewWindowStructure1 = {
        70,32,  /* window XY origin relative to TopLeft of screen */
        189,34, /* window width and height */
        0,1,    /* detail and block pens */
        GADGETUP|CLOSEWINDOW,       /* IDCMP flags */
        WINDOWSIZING+WINDOWDRAG+WINDOWDEPTH|WINDOWCLOSE,    /* other window flags */
        &gadw1_1, /* first gadget in gadget list */
        NULL,   /* custom CHECKMARK imagery */
        "fenetre1",     /* window title */
        NULL,   /* custom screen pointer */
        NULL,   /* custom bitmap */
        5,5,    /* minimum width and height */
        640,200,        /* maximum width and height */
        WBENCHSCREEN    /* destination screen type */
};

struct Gadget gadw2_1 = {
        NULL,   /* next gadget */
        19,14,  /* origin XY of hit box relative to window TopLeft */
        135,13, /* hit box width and height */
        NULL,   /* gadget flags */
        RELVERIFY,      /* activation flags */
        BOOLGADGET,     /* gadget type flags */
        (APTR)&Border1_1,        /* gadget border or image to be rendered */
        NULL,   /* alternate imagery for selection */
        &IText1_1,       /* first IntuiText structure */
        NULL,   /* gadget mutual-exclude long word */
        NULL,   /* SpecialInfo structure */
        1,      /* user-definable data */
        NULL    /* pointer to user-definable data */
};

struct IntuiText IText2_2 = {
        3,0,JAM2,       /* front and back text pens, drawmode and fill byte */
        36,3,   /* XY origin relative to container TopLeft */
        NULL,   /* font pointer or NULL for default */
        "Gadget2",      /* pointer to text */
        NULL    /* next IntuiText structure */
};

struct Gadget gadw2_2 = {
        &gadw2_1,   /* next gadget */
        16,32,  /* origin XY of hit box relative to window TopLeft */
        146,15, /* hit box width and height */
        NULL,   /* gadget flags */
        RELVERIFY,      /* activation flags */
        BOOLGADGET,     /* gadget type flags */
        (APTR)&Border1_1,        /* gadget border or image to be rendered */
        NULL,   /* alternate imagery for selection */
        &IText2_2,       /* first IntuiText structure */
        NULL,   /* gadget mutual-exclude long word */
        NULL,   /* SpecialInfo structure */
        2,      /* user-definable data */
        NULL    /* pointer to user-definable data */
};

#define gadgetList2 gadw2_2

struct NewWindow NewWindowStructure2 = {
        71,66,  /* window XY origin relative to TopLeft of screen */
        186,60, /* window width and height */
        0,1,    /* detail and block pens */
        GADGETUP|CLOSEWINDOW,       /* IDCMP flags */
        WINDOWSIZING+WINDOWDRAG+WINDOWDEPTH|WINDOWCLOSE,    /* other window flags */
        &gadw2_2,      /* first gadget in gadget list */
        NULL,   /* custom CHECKMARK imagery */
        "fenetre2",     /* window title */
        NULL,   /* custom screen pointer */
        NULL,   /* custom bitmap */
        5,5,    /* minimum width and height */
        640,200,        /* maximum width and height */
        WBENCHSCREEN    /* destination screen type */
};


void cleanup() {
        if(win1 !=NULL)CloseWindow(win1);
        if(win2 !=NULL)CloseWindow(win2);
}

void main(void)
{
    atexit(cleanup);

    if((win1 = (struct Window *)
        OpenWindow(&NewWindowStructure1))==0)
            exit(10);

    if((win2 = (struct Window *)
        OpenWindow(&NewWindowStructure2))==0)
            exit(10);

    while(win1 || win2){
        Wait((win1 ? (1L << win1->UserPort->mp_SigBit): 0) | \
                   (win2 ? (1L << win2->UserPort->mp_SigBit):0) );
        if(win1)
          while (IM = (struct IntuiMessage *) GetMsg(win1->UserPort)){
            switch(IM->Class){
                case CLOSEWINDOW :
                    ReplyMsg(IM);
                    puts("Fermeture de la fenetre 1");
                    if(win1 !=NULL){ CloseWindow(win1); win1=NULL; }
                    break;
                case GADGETUP :
                    printf("Gad %d de la fenetre 1\n",
                        ((struct Gadget *)IM->IAddress)->GadgetID);
                    ReplyMsg(IM);
                    break;
                default:
                    ReplyMsg(IM);
                    break;
            }
            if(!win1) break;
        }
        if(win2)
          while (IM = (struct IntuiMessage *) GetMsg(win2->UserPort)){
            switch(IM->Class){
                case CLOSEWINDOW :
                    ReplyMsg(IM);
                    puts("Fermeture de la fenetre 2");
                    if(win2 !=NULL){ CloseWindow(win2); win2=NULL; }
                    break;
                case GADGETUP :
                    printf("Gad %d de la fenetre 2\n",
                        ((struct Gadget *)IM->IAddress)->GadgetID);
                    ReplyMsg(IM);
                    break;
                default:
                    ReplyMsg(IM);
                    break;
            }
            if(!win2) break;
        }
    }
}
