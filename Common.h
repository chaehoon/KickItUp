#ifndef	_KICKITUP_COMMON_H
#define	_KICKITUP_COMMON_H

#include <SDL/SDL.h>
#include <assert.h>

#define PATH_LEN    255

#	define MsgBox( A, B, C )   printf( A )

#ifdef _WIN32
#	include <io.h>
#	include <direct.h>
#	define	snprintf	_snprintf
#	define	getcwd		_getcwd
#else // _WIN32
#	include <unistd.h>
#endif // _WIN32

bool GetRealFileName(const char * filename, char * buff, unsigned int size);

#endif // _COMMON_H
