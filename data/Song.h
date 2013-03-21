#ifndef _KICKITUP_SONG_H
#define _KICKITUP_SONG_H

#include "../common.h"
#include "Ksf.h"
#include <string>

using std::string;

class Surface;

class Song
{
private:
	const static	int	MAX_DATA	=	2048;

	ePlayMode	m_playMode;

	string	m_introWavPath;	// intro wav file.
	string	m_introMp3Path;	// intro mp3 file.
	string	m_discImagePath;
	string	m_BGImgPath;
	string	m_MSDPath;
    string	m_songPath;

	Surface *	m_pDiscImg;

public:
    Ksf         m_ksf[2];   // KickItUp Step Format.

public:
	Song(void);
	~Song(void);

	void		SetPlayMode( const ePlayMode playmode )			{
        m_playMode = playmode;
    }
    ePlayMode	GetPlayMode() const	                            {
        return m_playMode;
    }

	bool		ReadKsf( const int & index, const string & filename );
    Surface *   GetDiscImg() const  {   return m_pDiscImg;    }

    string      GetIntroPath() const;
    string      GetBGImgPath() const;
    string      GetSongPath() const;

private:
	bool		_readKsf( const string & filename, Ksf & ktf );
	bool		_getFullPath( const string & inFileName, string & outFileName );
	bool		_findFile ( const string & filename, string & destFiename );
	bool		_loadDiscImg( const string & filename );
};

#endif // _KICKITUP_SONG_H
