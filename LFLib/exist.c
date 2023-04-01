#include <libraries/dos.h>
#include <libraries/dosextens.h>
#include <proto/dos.h>

__regargs char exist(char *fn)
{
    BPTR fl;

    if(!(fl=Lock(fn,ACCESS_READ)))
        return(0);

    UnLock(fl);
    return(1);
}
