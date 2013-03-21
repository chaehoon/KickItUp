#include "SongMgr.h"
#include <sstream>
#include <SDL/SDL.h>

#ifdef _WIN32
#	include <Windows.h>
#	include <io.h>
#else // _WIN32
#	include <sys/types.h>
#	include <dirent.h>
#	include <strings.h>
#endif // _WIN32

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

#ifdef _WIN32
bool SongMgr::Load( const string & dirName )
{
	HANDLE	hFind;
	int		count = 0;
	WIN32_FIND_DATA	findFileData;

	string	path;

	hFind = FindFirstFile( "*.*", &findFileData );
	path = findFileData.cFileName;

	// find "song" directory.
	if( stricmp( path.c_str(), dirName.c_str() ) != 0 ) {
		while( FindNextFile( hFind, &findFileData) ) {
			if( stricmp( findFileData.cFileName, dirName.c_str() ) == 0 ) {
				path = findFileData.cFileName;
				break;
			}
		}
	}

	// Is it directory ?
	if( findFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY )
		goto error_return;

	FindClose( hFind );
	SetCurrentDirectory( path.c_str() );
	hFind = FindFirstFile( "*.*", &findFileData );

    // read song list.
	do {
		if( findFileData.cFileName[0] != '.' && findFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY ) {
			SetCurrentDirectory( findFileData.cFileName );

			_readStepFile();

			SetCurrentDirectory( "..\\" );
		}
	} while( FindNextFile( hFind, &findFileData ) );

	SetCurrentDirectory( "..\\" );
	FindClose( hFind );
	return true;

error_return:
	SetCurrentDirectory( "..\\" );
	FindClose( hFind );
	return false;
}
#endif // _WIN32


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
	int		maxStepFileCount = 2;

	switch( playmode ) {
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
		maxStepFileCount = 1;
		break;
	}

	bool	ret = false;
    if( playmode == ePM_Double ) {
        std::ostringstream	fileName;
        fileName << strPlayeMode << ".ksf";

        if( _access( fileName.str() ) ) {
            pSong->SetPlayMode( playmode );
            ret = pSong->ReadKsf( 0, fileName.str() );
        }
    }
    else {
		for( int i = 0 ; i < 2 ; ++i ) {
			std::ostringstream	fileName;
			fileName << strPlayeMode << i + 1 << ".ksf";

			if( _access( fileName.str() ) ) {
				pSong->SetPlayMode( playmode );
				ret = pSong->ReadKsf( i, fileName.str() );
			}
        }
    }
	return ret;
}

bool SongMgr::_access( const string & filename )
{
	return ::access( filename.c_str(), 02 ) == 0;
}