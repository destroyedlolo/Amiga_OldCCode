#include <stdio.h>
#include <time.h>

char heure[40];
const char *mois[]={"janvier","février","mars","avril","mai","juin","juillet",
            "aout","septembre","octobre","novembre","decembre"};
const char *jours[]={"dimanche","lundi","mardi","mercredi","jeudi","vendredi","samedi"};

char *cheure()
{
    time_t t;
    struct tm *ptm;

    time(&t);
    ptm=localtime(&t);
    sprintf(heure,"%s %d %s %d, %02d:%02d:%02d",
        jours[ptm->tm_wday],ptm->tm_mday,mois[ptm->tm_mon],1900+ptm->tm_year,
        ptm->tm_hour,ptm->tm_min,ptm->tm_sec);
    return(heure);
}
