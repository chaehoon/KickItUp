#include "SongMgr.h"
#include <SDL/SDL.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <strings.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>

using std::vector;

SongMgr g_SongMgr;

SongMgr::SongMgr(void)  :   m_leftSongIndex( 0 ), m_rightSongIndex( 1 ), m_pSongSelect( 0 )
{
}

SongMgr::~SongMgr(void)
{
    for( SongListIter i = m_songList.begin() ; i != m_songList.end() ; ++i ) {
        delete *i;
    }
    m_songList.clear();
}



/**
 * 현재 디렉토리 리스트를 얻는다.
 * @param	dirs	디렉토리 리스트.
 * @return	디렉토리 리스트가 있는지 없는지.
 */
bool SongMgr::_getDirs ( vector<string> & dirs )
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


bool SongMgr::Load( const string & dirName )
{
	string songDir;
	if ( !_getRealFileName( dirName, &songDir ) )
		return false;

	chdir ( songDir.c_str() );
	vector<string> dirs;
	if ( !_getDirs ( dirs ) ) {
		return false;
	}

	for ( vector<string>::iterator i = dirs.begin() ; i != dirs.end(); ++i ){
		string dirName = *i;
		chdir ( dirName.c_str() );
		_readStepFile();
		chdir ( ".." );
	}

	chdir ( ".." );
	return true;
}


void SongMgr::TurnLeft()
{
    if( m_songList.size() != 0 ) {
        m_leftSongIndex = (m_leftSongIndex - 2) % m_songList.size();
        m_rightSongIndex = (m_rightSongIndex - 2) % m_songList.size();;
    }
    SetSelectSong( 0 );
}

void SongMgr::TurnRight()
{
    if( m_songList.size() != 0 ) {
        m_leftSongIndex = (m_leftSongIndex + 2) % m_songList.size();
        m_rightSongIndex = (m_rightSongIndex + 2) % m_songList.size();
    }
    SetSelectSong( 0 );
}

// Step File을 읽어들임.
bool SongMgr::_readStepFile()
{
	for( int i = ePM_Min ; i < ePM_Max ; ++i ) {
		Song *	pSong = new Song;
		if( _readStep( static_cast<ePlayMode>( i ), pSong ) ) {
			m_songList.push_back( pSong );
		}
        else {
            delete pSong;
        }
	}

    // Select Stage에서 보일 왼쪽 오른쪽 곡을 선택
    m_leftSongIndex = m_rightSongIndex = -1;

    if( 0 < m_songList.size() ) {
        m_leftSongIndex = 0;
        m_rightSongIndex = 0;
        if( 1 < m_songList.size() )
            m_rightSongIndex = 1;
    }

    // 선택된 곡 없음.
    m_pSongSelect = 0;

	return true;
}

bool SongMgr::_readStep( const ePlayMode playmode, Song * pSong )
{
	string	strPlayeMode;

	/* 발판 file 개수
		스텝 파일은 easy_1.ksf, easy_2.ksf, hard_1.ksf, hard_2.ksf 등으로 같은 종류의 게임모드에 2개씩존재한다.
		단 Double모드는 Double.ksf 하나만 있다.
	*/
	switch( static_cast<int>(playmode) ) {
	case ePM_Easy:
		strPlayeMode = "easy_";
		break;
	case ePM_Hard:
		strPlayeMode = "hard_";
		break;
	case ePM_Crazy:
		strPlayeMode = "hard_";
		break;
	case ePM_Double:
		strPlayeMode = "double";
		break;
	}

	bool	ret = false;
    if( playmode == ePM_Double ) {
        std::ostringstream	fileName;
        fileName << strPlayeMode << ".ksf";

        string realFileName;
        if( _getRealFileName(fileName.str(), &realFileName ) ) {
            pSong->SetPlayMode( playmode );
            ret = pSong->ReadKsf( 0, realFileName );
        }
    }
    else {
		for( int i = 0 ; i < 2 ; ++i ) {
			std::ostringstream	fileName;
			fileName << strPlayeMode << i + 1 << ".ksf";

			string realFileName;
			if( _getRealFileName(fileName.str(), &realFileName ) ) {
				pSong->SetPlayMode( playmode );
				ret = pSong->ReadKsf( i, realFileName );
			}
        }
    }
	return ret;
}

bool SongMgr::_access( const string & filename )
{
	return ::access( filename.c_str(), 02 ) == 0;
}

/**
 * 파일이나 디렉토리명을 대소문자 구분없이 찾아 파일명을 반환한다.
 *
 * @param	filename	찾을 파일명이나 디렉토리명.
 * @param	buff		반환할 찾은 파일명.
 * @param	size		buff size.
 * @return	해당 파일이나 디렉토리명이 있는지.
 */
bool SongMgr::_getRealFileName(const string & filename, string * realname)
{
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
