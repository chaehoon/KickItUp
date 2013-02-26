/*
 * Common.cpp
 *
 *  Created on: 2013. 2. 13.
 *      Author: chaehoon-win
 */

#include <cstring>

#ifdef _WIN32

/**
 * 윈도우에서는 대소문자를 구분하지 않아 그냥 파일명을 넘긴다.
 *
 * @param	filename	파일명
 * @param	buff		진짜 파일명 반환.
 * @param	size		buff size.
 * @return	파일이 있는지 여부.
 */
bool GetRealFileName(const char * filename, char * buff, unsigned int size)
{
	strncpy(buff, filename, size);
	return true;
}

#else	// _WIN32

#include <sys/types.h>
#include <dirent.h>
#include <strings.h>

/**
 * 파일이나 디렉토리명을 대소문자 구분없이 찾아 파일명을 반환한다.
 *
 * @param	filename	찾을 파일명이나 디렉토리명.
 * @param	buff		반환할 찾은 파일명.
 * @param	size		buff size.
 * @return	해당 파일이나 디렉토리명이 있는지.
 */
bool GetRealFileName(const char * filename, char * buff, unsigned int size)
{
	struct dirent * item;

	DIR * dp = opendir ( "." );
	bool	bFound = false;
	if ( dp != NULL ) {
		while ( true ) {
			item = readdir ( dp );
			if ( item == NULL )
				break;

			if ( strcasecmp ( item->d_name, filename ) == 0 ) {
				strncpy ( buff, item->d_name, size );
				bFound = true;
				break;
			}
		}
	}

	closedir ( dp );

	if ( !bFound )	{
		std::stringstream	str ( filename );
		str << "directory or file is not found :" << filename;
		MsgBox ( str.str().c_str(),"KICKITUP ERROR",MB_OK );
		return false;
	}
	return true;
}

#endif // ! _WIN32
