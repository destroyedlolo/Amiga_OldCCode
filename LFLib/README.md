This directory contains the source code of my very own LFLib library (targeting DCC).

This linkable library contains several utilities functions used in most of my other codes.

* `char *cheure()` - returns a static string containing the local time and day in **French**.
It has been made long time before AmigaOS 2.1 localisation

* `char exist(char *file)` - checks if a file exist (1) or not (0)

* `long LFicos( short x )` - returns the **cosinus value** of the argument, expressed in degree, using a static conversion table. 
Made to speed up calculation on my **68010 powered Amiga 1000**.

* `void LFatal(char *msg)` - Displays a recoverable Guru based on the given message.

* `char *LFCrypt(char *msg,char *pw,ULONG *len)` - (too) simple XOR string encryption. Mostly used for code obstruction. 

* `char *LFCryptB(char *msg,char *pw,char *buff,ULONG len)` is doing the same but binary data compliant and store the result in the provided buffer. 

*`char *LFDecrypt(const char *msg,const char *pw,ULONG len)` is doing the reverse of LFCrypt()

* `char *LFDecryptB(const char *msg,const char *pw,char *buff,ULONG len)` is doing the reverse of LFCryptB()

* `char *LFgets(FILE *f,char *l,size_t max)` - fgets() but remove a potential trailling linefeed

* `UBYTE LFInJoy( UBYTE port )` - read joystick ports condition.
Part of a projet to build an interactive presentation box where joysticks ports were used as keyboard.

* `LFPrintf` and `LFSPrintf` - Use ?printf() ARP counterpart to avoid stdio code.. in order to make executables smallers

* `char *LFStrDos(const char *p,const char *f)` - Concatenate a filename to a pathname in AmigaDOS way.

---

## Shortkeys

Manage a lookup table of shortkeys (menu, button, whatever ...)

* `int LFAddKey( struct List *lst, char key, Tag tags,...)` - add a key in the list

* `void *LFFindKey( struct List *lst, char key, Tag tags,...)` - find a key

* `void LFFreeKey( struct List *lst, char key)` - remove a key

* `void LFFreeKeyList( struct List *lst )` - remove a list

---

## Double buffering

As far I remember, doesn't work on 3.0+

* `struct Window *LFDB_OpenScreen( struct NewScreen * )` - open a custom screen with an overall window.

* `void LFDB_SwapBuffers( struct Window * )` - as the name said, swap between buffers

* `void LFDB_CloseScreen( struct Window * )`


