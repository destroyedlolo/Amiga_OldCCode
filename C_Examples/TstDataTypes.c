/*
 *  TstDataTypes.c
 *
 *  Programme de test sur l'utilisation des datatypes avec Dice.
 *  D'apres la Ascii.c de la distribution de commodore.
 *
 *  Ce programme pourrait être optimisé pour DICE car je l'ai fait sur mon 4000
 *  sans que les includes et les lib v39.xxx pour dice soit installé !
 */


#include <exec/types.h>
#include <exec/memory.h>
#include <exec/execbase.h>
#include <exec/libraries.h>
#include <datatypes/datatypes.h>
#include <datatypes/datatypesclass.h>
#include <datatypes/textclass.h>
#include <dos/dos.h>
#include <intuition/icclass.h>
#include <intuition/screens.h>
#include <intuition/intuition.h>
#include <graphics/gfx.h>
#include <graphics/text.h>
#include <graphics/gfxbase.h>
#include <utility/tagitem.h>
#include <string.h>
#include <stdio.h>

#include <clib/alib_protos.h>
#include <clib/dos_protos.h>
#include <clib/exec_protos.h>
#include <clib/datatypes_protos.h>
#include <clib/intuition_protos.h>
#include <clib/graphics_protos.h>
#include <clib/utility_protos.h>

#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/datatypes.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/utility.h>

struct Library *DataTypesBase;

void main (int argc, char **argv)
{
    struct IntuiMessage *imsg;
    struct TextFont *tf;
    struct TextAttr ta;
    UBYTE fontname[32];
    struct Screen *scr;
    struct Window *win;
    BOOL going = TRUE;
    BYTE *buffer;
    ULONG length;
    Object *o;
    BPTR sfh;

    if (argc < 2)
    {
        printf ("requires a file name\n");
    }
    else if (DataTypesBase = OpenLibrary ("datatypes.library", 39))
    {
        /* Open and read the source raw sound file */
        if (sfh = Open (argv[1], MODE_OLDFILE))
        {
            Seek (sfh, 0, OFFSET_END);
            if ((length = Seek (sfh, 0, OFFSET_BEGINNING)) > 0)
            {
                if (buffer = AllocVec (length + 1, MEMF_CLEAR))
                {
                    if (Read (sfh, buffer, length) == length)
                    {
                        if (scr = LockPubScreen (NULL))
                        {
                            struct TagItem tg[] = {
                                GA_Left, scr->WBorLeft,
                                GA_Top, scr->BarHeight + 1,
                                GA_Width, 300,
                                GA_Height, 150,
                                GA_Immediate, TRUE,
                                GA_RelVerify, TRUE,
                                ICA_TARGET, ICTARGET_IDCMP,
                                DTA_SourceType, DTST_RAM,
                                DTA_GroupID, GID_TEXT,
                                DTA_TextAttr, &ta,
                                TDTA_Buffer, buffer,
                                TDTA_BufferLen, length,
                            /*     TDTA_WordWrap, TRUE, */
                                TAG_DONE
                            };
                            tf = ((struct GfxBase *) GfxBase)->DefaultFont;
                            strcpy (fontname, tf->tf_Message.mn_Node.ln_Name);
                            ta.ta_Name = fontname;
                            ta.ta_YSize = tf->tf_YSize;
                            ta.ta_Style = tf->tf_Style;
                            ta.ta_Flags = tf->tf_Flags;

                            /* Create the text object */
                            if (o = NewDTObjectA ((APTR) "ascii.test",tg))
                            {
                                if (win = OpenWindowTags (NULL,
                                                          WA_InnerWidth, 300,
                                                          WA_InnerHeight, 300,
                                                          WA_Gadgets, o,
                                                          WA_Title, argv[1],
                                                          WA_DragBar, TRUE,
                                                          WA_DepthGadget, TRUE,
                                                          WA_CloseGadget, TRUE,
                                                          WA_AutoAdjust, TRUE,
                                                          WA_SimpleRefresh, TRUE,
                                                          WA_IDCMP, IDCMP_IDCMPUPDATE | IDCMP_CLOSEWINDOW | IDCMP_VANILLAKEY,
                                                          TAG_DONE))
                                {
                                    while (going)
                                    {
                                        Wait (1L << win->UserPort->mp_SigBit);

                                        while (imsg = (struct IntuiMessage *) GetMsg (win->UserPort))
                                        {
                                            switch (imsg->Class)
                                            {
                                            case IDCMP_IDCMPUPDATE:
                                                if (FindTagItem (DTA_Sync, (struct TagItem *)imsg->IAddress))
                                                    RefreshGList ((struct Gadget *) o, win, NULL, 1);
                                                break;

                                            case IDCMP_CLOSEWINDOW:
                                                going = FALSE;
                                                break;
                                            }

                                            ReplyMsg ((struct Message *)imsg);
                                        }
                                    }

                                    CloseWindow (win);
                                }
                                else
                                {
                                    printf ("couldn't open window to display object in.\n");
                                }

                                /* Get rid of the object */
                                DisposeDTObject (o);

                                /* Once you send the buffer to the object, it belongs to it. */
                                buffer = NULL;
                            }
                            else
                            {
                                printf ("couldn't create text object\n");
                            }

                            UnlockPubScreen (NULL, scr);
                        }
                    }
                    FreeVec (buffer);
                }
            }
            Close (sfh);
        }

        CloseLibrary (DataTypesBase);
    }
    else
        printf ("couldn't open datatypes.library V39\n");
}
