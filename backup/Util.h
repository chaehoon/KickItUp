#ifndef	_KICKITUP_COMMON_H
#define	_KICKITUP_COMMON_H

#include <SDL/SDL.h>
#include <assert.h>

#define PATH_LEN    255

#define MsgBox( A, B, C )   printf( A )

#include <unistd.h>

bool GetRealFileName(const char * filename, char * buff, unsigned int size);

#endif // _COMMON_H
