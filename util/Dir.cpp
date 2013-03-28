/*
 * Dir.cpp
 *
 *  Created on: 2013. 3. 28.
 *      Author: chaehoon-win
 */

#include "Dir.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <strings.h>
#include <iostream>

/**
 * 현재 디렉토리 리스트를 얻는다.
 * @param	dirs	디렉토리 리스트.
 * @return	디렉토리 리스트가 있는지 없는지.
 */
bool Dir::GetDirs ( vector<string> & dirs )
{
	DIR * dp = opendir ( "." );

	while ( dp != NULL ) {
		struct dirent * item = readdir ( dp );
		if ( item == NULL )
			break;

		if ( item->d_name[0] == '.' )
			continue;

		// is dir?
		struct stat stat_p;
		if ( stat ( item->d_name, &stat_p ) == -1 )
			continue;
		if ( !S_ISDIR ( stat_p.st_mode ) )
			continue;

		string	dirname ( item->d_name );
		dirs.push_back ( dirname );
	}
	closedir ( dp );

	if ( dirs.empty() ) {
		std::cerr << "GetDirs() - directory or file is not found\n";
		return false;
	}
	return true;
}
