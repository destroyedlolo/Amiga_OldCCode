#include <exec/types.h>
#include <intuition/intuition.h>
#include <proto/intuition.h>
#include <string.h>
#include <stdlib.h>

__regargs void LFatal(char *msg)
{
    char buff[100];

    buff[0]=0;buff[1]=50;buff[2]=17;
    strcpy(&buff[3],msg);
    buff[4+strlen(msg)]=0;
    DisplayAlert(RECOVERY_ALERT,buff,30);
}
