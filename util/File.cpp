/*
 * File.cpp
 *
 *  Created on: 2013. 3. 28.
 *      Author: chaehoon-win
 */

#include "File.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <strings.h>
#include <iostream>

/**
 * 파일이나 디렉토리명을 대소문자 구분없이 찾아 파일명을 반환한다.
 *
 * @param	filename	찾을 파일명이나 디렉토리명.
 * @param	buff		반환할 찾은 파일명.
 * @param	size		buff size.
 * @return	해당 파일이나 디렉토리명이 있는지.
 */
bool File::GetRealFileName(const string & filename, string * realname)
{
	*realname = filename;
	return true;

	struct dirent * item;

	DIR * dp = opendir ( "." );
	bool	bFound = false;
	if ( dp != NULL ) {
		while ( true ) {
			item = readdir ( dp );
			if ( item == NULL )
				break;

			if ( strcasecmp ( item->d_name, filename.c_str() ) == 0 ) {
				*realname = item->d_name;
				bFound = true;
				break;
			}
		}
	}

	closedir ( dp );

	if ( !bFound )	{
		std::cerr << "directory or file is not found :" << filename;
		return false;
	}
	return true;
}


bool File::Access( const string & filename )
{
	return ::access( filename.c_str(), 02 ) == 0;
}
