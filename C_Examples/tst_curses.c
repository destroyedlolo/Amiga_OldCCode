/*
 *  Tst_curses.c
 *
 *  Par L.FAILLIE
 *
 *  Test de la librairie curses.
 *
 *  24/11/1997 Premiere version.
 *
 *  Note: Si DEBUG est défini, des messages suplémentaires sont affichés mais
 *  nécessite que SUSHI soit lancé (sortie par kprintf()).
 *
 *  Compilation par:
	gcc -DDEBUG tst_curses.c -o tst -ldebug -lcurses -ltermcap
 */

#include <lflib/curses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* pour read() */

#ifdef DEBUG
#define DBG(x) x;
#else
#define DBG(x)
#endif

void fin(){
/* L'environnement est remis dans un état utilisable
 */
	echo();
	mvprintw(LINES,0,"\n");
	refresh();
	endwin();
	delwin(curscr);
	delwin(stdscr);
}

void interuption(){
/* On est ainsi sur que 'exit()' est appelée.
 */
	exit(EXIT_SUCCESS);
}

int main( void ){
	initscr();
	signal(SIGINT,interuption);
	atexit(fin);

	DBG(kprintf("Taille de la fenêtre %ld x %ld\n",COLS,LINES));

	{
		register int i;
		char x[5];
		int nl= read(STDIN_FILENO, x, 5);

		DBG(kprintf("%ld données lues :",nl); for(i=0; i<nl; i++) kprintf("%lx ",x[i]); kprintf("\n"););
	}

	exit(EXIT_SUCCESS);
}

