/*****************************************************************************
        Programme pour tester RawKey (05/11/1992)

            Par L.Faillie

*****************************************************************************/

#include <lf.h>
#include <intuition/intuition.h>

struct IntuiMessage *IM = NULL;
struct Window *window=NULL;

/*---------------------------------------------------*
  Gadgets created with PowerSource V3.0
  which is (c) Copyright 1990-91 by Jaba Development
  written by Jan van den Baard
 *---------------------------------------------------*/

struct NewWindow new_window = {
  50,25,538,83,0,1,
  CLOSEWINDOW+RAWKEY,
  WINDOWSIZING+WINDOWDRAG+WINDOWDEPTH+WINDOWCLOSE+NOCAREREFRESH+SMART_REFRESH+ACTIVATE,
  NULL,NULL,
  (UBYTE *)"Test de Raw Key",NULL,NULL,
  150,50,640,263,WBENCHSCREEN };

void fini(){
    if(window !=NULL)CloseWindow(window);
}

void main(){
    char rep[9],lg;

    if((window = (struct Window *)
        OpenWindow(&NewWindowStructure1))==0)
            Erreur(err_ow[lang]);

    atexit(fini);

    FOREVER{
        Wait(1L << window->UserPort->mp_SigBit);
        while(Message = (struct IntuiMessage *)GetMsg(window->UserPort)) {
            long class = Message->Class;
            ReplyMsg(Message);

            if(class== CLOSEWINDOW)
                exit(0);
            if(class== RAWKEY){
