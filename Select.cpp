/*
	1999-2000 Copyright (c) mahalo, the nogada worker.. (Amigo~*). All rights reserved.
	          Copyright (c) Hyung-seok Choi, the Graphics Designer. All rights reserved.

  	2000/07/22 'Work for Opening Source'
			- Removed Motion-Video Support (for improve safety and speed)
			- Removed Xaudio player mp3 play (this required their own dlls)

			- Added Audio-Media play using DirectShow.
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sstream>

#include "Common.h"
#include "Main.h"
#include "Input.h"
#include "Select.h"
#include "Result.h"

#include "Song.h"
#include "Surface.h"

#define DISCSIZE_X	300
#define DISCSIZE_Y	200

#define	STILL_DRAWING	100

extern Song					CSONG[512];

extern Surface gSongBack;
extern Surface gSongTitle;
extern Surface gSelectBack;
extern Surface gStateComment;
extern Surface gShiftLeft;
extern Surface gShiftRight;
extern Surface	gModeIcon;

extern	Surface	gDoubleIcon;
extern	Surface	gCrazyIcon;
extern	Surface	gHardIcon;
extern	Surface	gEasyIcon;

extern double				bpm,bpm2,bpm3;
extern int					start,start2,start3;
extern	int					tick;
extern	Uint32					bunki,bunki2;

extern char					SongName[PATH_LEN+1];
extern char					SongName2[PATH_LEN+1];
extern char					SongName3[PATH_LEN+1];
extern char					Title[PATH_LEN+1];
extern	int					start1;

extern	char				GameMode;

extern	char				Couple;
extern	char				Double;
// End of the COUPLE DATA


#include "Chunk.h"
extern Chunk			gMode;
extern Chunk            gCancel;
extern Chunk			gMove;
extern Chunk			gSelectSong;

extern char First;

Uint32	startTimer, curTimer;

extern bool	IntroFlag;

bool	ClpBlt3 ( int x ,int y ,Surface & surface, const SDL_Rect & srect )
{
	SDL_Rect sRect;

	sRect = srect;

	if ( 640 < x || 480 < y )
		return true;

	if ( sRect.h < -y || sRect.w < -x )
		return true;

	if ( 480 < ( y + sRect.h ) )
		sRect.h = 480 - y;

	if ( y < 0 )	{
		sRect.y -= y;
		sRect.h += y;
		y = 0;
	}

	if ( 640 < x + sRect.w )
		sRect.w = 640 - x;

	if ( x < 0 )	{
		sRect.x -= x;
		sRect.w += x;
		x = 0;
	}

	surface.BltFast ( x, y, gScreen, &sRect );

	return true;

}

#ifdef _WIN32

#include <Windows.h>
void Read()
{
	HANDLE	hFind;
	Uint32	Count=0;
	WIN32_FIND_DATA lpData;

	char    cPathStr[PATH_LEN+1];;

	hFind= ( HANDLE ) FindFirstFile ( "*.*",&lpData );
	strncpy ( cPathStr, lpData.cFileName, sizeof ( cPathStr ) );
	if ( stricmp ( cPathStr,"SONG" ) !=0 )
	{
		for ( ;; )
		{
			if ( FindNextFile ( hFind,&lpData ) ==false ) break;
			strncpy ( cPathStr, lpData.cFileName, sizeof ( cPathStr ) );
			if ( stricmp ( cPathStr,"SONG" ) ==0 ) break;
		}
	}
	if ( lpData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY )
	{
		SetCurrentDirectory ( cPathStr );
		hFind=FindFirstFile ( "*.*",&lpData );
		if ( lpData.cFileName[0]!='.' && lpData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY )
		{
			SetCurrentDirectory ( lpData.cFileName );

			if ( access ( "Crazy_2.stf",04 ) ==0 )
				CSONG[Count].ReadCrazy_2( "Crazy_2.stf" );
			else if ( access ( "Crazy_2.ksf",04 ) ==0 )
				CSONG[Count].ReadCrazy_2 ( "Crazy_2.ksf" );

			if ( access ( "Crazy_1.stf",04 ) ==0 )
				CSONG[Count].ReadCrazy_1( "Crazy_1.stf" ),Count++;
			else if ( access ( "Crazy_1.ksf",04 ) ==0 )
				CSONG[Count].ReadCrazy_1 ( "Crazy_1.ksf" ),Count++;

			if ( access ( "Hard_2.stf",04 ) ==0 )
				CSONG[Count].ReadHard_2( "Hard_2.stf" );
			else if ( access ( "Hard_2.ksf",04 ) ==0 )
				CSONG[Count].ReadHard_2 ( "Hard_2.ksf" );

			if ( access ( "Hard_1.stf",04 ) ==0 )
				CSONG[Count].ReadHard_1( "Hard_1.stf" ),Count++;
			else if ( access ( "Hard_1.ksf",04 ) ==0 )
				CSONG[Count].ReadHard_1( "Hard_1.ksf" ),Count++;

			if ( access ( "Easy_2.stf",04 ) ==0 )
				CSONG[Count].ReadEasy_2( "Easy_2.stf" );
			else if ( access ( "Easy_2.ksf",04 ) ==0 )
				CSONG[Count].ReadEasy_2( "Easy_2.ksf" );

			if ( access ( "Easy_1.stf",04 ) ==0 )
				CSONG[Count].ReadEasy_1( "Easy_1.stf" ),Count++;
			else if ( access ( "Easy_1.ksf",04 ) ==0 )
				CSONG[Count].ReadEasy_1( "Easy_1.ksf" ),Count++;

			if ( access ( "Double.stf",04 ) ==0 )
				CSONG[Count].ReadDouble( "Double.stf" ),Count++;
			else if ( access ( "Double.ksf",04 ) ==0 )
				CSONG[Count].ReadDouble( "Double.ksf" );

			//if(CSONG[Count].bpm!=0)Count++;
			SetCurrentDirectory ( "..\\" );
		}

		for ( ;; )
		{
			if ( FindNextFile ( hFind,&lpData ) ==false ) break;
			else
			{
				if ( lpData.cFileName[0]!='.' && lpData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY )
				{
					SetCurrentDirectory ( lpData.cFileName );

					if ( access ( "Crazy_2.stf",04 ) == 0 )
						CSONG[Count].ReadCrazy_2 ( "Crazy_2.stf" );
					else if ( access ( "Crazy_2.ksf",04 ) ==0 )
						CSONG[Count].ReadCrazy_2 ( "Crazy_2.ksf" );

					if ( access ( "Crazy_1.stf",04 ) ==0 )
						CSONG[Count].ReadCrazy_1( "Crazy_1.stf" ),Count++;
					else if ( access ( "Crazy_1.ksf",04 ) ==0 )
						CSONG[Count].ReadCrazy_1( "Crazy_1.ksf" ),Count++;

					if ( access ( "Hard_2.stf",04 ) ==0 )
						CSONG[Count].ReadHard_2( "Hard_2.stf" );
					else if ( access ( "Hard_2.ksf",04 ) ==0 )
						CSONG[Count].ReadHard_2( "Hard_2.ksf" );

					if ( access ( "Hard_1.stf",04 ) ==0 )
						CSONG[Count].ReadHard_1( "Hard_1.stf" ),Count++;
					else if ( access ( "Hard_1.ksf",04 ) ==0 )
						CSONG[Count].ReadHard_1( "Hard_1.ksf" ),Count++;

					if ( access ( "Easy_2.stf",04 ) ==0 )
						CSONG[Count].ReadEasy_2( "Easy_2.stf" );
					else if ( access ( "Easy_2.ksf",04 ) ==0 )
						CSONG[Count].ReadEasy_2( "Easy_2.ksf" );

					if ( access ( "Easy_1.stf",04 ) ==0 )
						CSONG[Count].ReadEasy_1( "Easy_1.stf" ),Count++;
					else if ( access ( "Easy_1.ksf",04 ) ==0 )
						CSONG[Count].ReadEasy_1( "Easy_1.ksf" ),Count++;

					if ( access ( "Double.stf",04 ) ==0 )
						CSONG[Count].ReadDouble( "Double.stf" ),Count++;
					else if ( access ( "Double.ksf",04 ) ==0 )
						CSONG[Count].ReadDouble( "Double.ksf" ),Count++;

					//if(CSONG[Count].bpm!=0)Count++;
					SetCurrentDirectory ( "..\\" );
				}
			}
		}
		SetCurrentDirectory ( "..\\" );
	}
	FindClose ( hFind );

	if ( CSONG[0].getStep1().bpm1==0 && CSONG[0].getStep2().bpm1==0 )
		MsgBox ( "Song directory not found or No song data.","KICKITUP ERROR",MB_OK );
}

#else // _WIN32

#include <vector>
#include <string>
#include <sys/stat.h>
#include <dirent.h>
using std::string;
using std::vector;

/**
 * 현재 디렉토리 리스트를 얻는다.
 * @param	dirs	디렉토리 리스트.
 * @return	디렉토리 리스트가 있는지 없는지.
 */
bool GetDirs ( vector<string> & dirs )
{
	struct dirent * item;
	DIR * dp;
	char    cPathStr[PATH_LEN+1] = { 0, };

	dp = opendir ( "." );

	while ( dp != NULL )
	{
		item = readdir ( dp );
		if ( item == NULL )
			break;

		if ( item->d_name[0] == '.' )
			continue;

		// is dir?
		struct stat stat_p;
		if ( lstat ( item->d_name, &stat_p ) == -1 )
			continue;
		if ( !S_ISDIR ( stat_p.st_mode ) )
			continue;

		string	dirname ( item->d_name );
		dirs.push_back ( dirname );
	}
	closedir ( dp );

	if ( dirs.empty() ) {
		std::stringstream	str;
		str << "GetDirs() - directory or file is not found";
		MsgBox ( str.str().c_str(),"KICKITUP ERROR",MB_OK );
		return false;
	}
	return true;
}

void Read()
{
	char  cPathStr[PATH_LEN+1] = { 0, };
	int	Count=0;

	if ( !GetRealFileName( "song", cPathStr, sizeof ( cPathStr ) ) )
		return;

	chdir ( cPathStr );
	vector<string> dirs;
	if ( !GetDirs ( dirs ) ) {
		chdir ( ".." );
		return;
	}

	for ( vector<string>::iterator i = dirs.begin() ; i != dirs.end(); ++i ){
		string dirName = *i;
		chdir ( dirName.c_str() );

		if ( GetRealFileName("crazy_2.stf", cPathStr, sizeof ( cPathStr ) ) )
			CSONG[Count].ReadCrazy_2( cPathStr );
		else if ( GetRealFileName("crazy_2.ksf", cPathStr, sizeof ( cPathStr ) ) )
			CSONG[Count].ReadCrazy_2( cPathStr );

		if ( GetRealFileName("crazy_1.stf", cPathStr, sizeof ( cPathStr ) ) ) {
			CSONG[Count].ReadCrazy_1( cPathStr );
			++Count;
		}
		else if ( GetRealFileName("crazy_1.ksf", cPathStr, sizeof ( cPathStr ) ) ) {
			CSONG[Count].ReadCrazy_1( cPathStr );
			++Count;
		}

		if ( GetRealFileName("hard_2.stf", cPathStr, sizeof ( cPathStr ) ) )
			CSONG[Count].ReadHard_2( cPathStr );
		else if ( GetRealFileName("hard_2.ksf", cPathStr, sizeof ( cPathStr ) ) )
			CSONG[Count].ReadHard_2( cPathStr );

		if ( GetRealFileName("hard_1.stf", cPathStr, sizeof ( cPathStr ) ) ) {
			CSONG[Count].ReadHard_1( cPathStr );
			++Count;
		}
		else if ( GetRealFileName("hard_1.ksf", cPathStr, sizeof ( cPathStr ) ) ) {
			CSONG[Count].ReadHard_1( cPathStr );
			++Count;
		}

		if ( GetRealFileName("easy_2.stf", cPathStr, sizeof ( cPathStr ) ) )
			CSONG[Count].ReadEasy_2( cPathStr );
		else if ( GetRealFileName("easy_2.ksf", cPathStr, sizeof ( cPathStr ) ) )
			CSONG[Count].ReadEasy_2( cPathStr );

		if ( GetRealFileName("easy_1.stf", cPathStr, sizeof ( cPathStr ) ) ) {
			CSONG[Count].ReadEasy_1( cPathStr );
			++Count;
		}
		else if ( GetRealFileName("easy_1.ksf", cPathStr, sizeof ( cPathStr ) ) ) {
			CSONG[Count].ReadEasy_1( cPathStr );
			++Count;
		}

		if ( GetRealFileName("double.stf", cPathStr, sizeof ( cPathStr ) ) ) {
			CSONG[Count].ReadDouble( cPathStr );
			++Count;
		}
		else if ( GetRealFileName("double.ksf", cPathStr, sizeof ( cPathStr ) ) ) {
			CSONG[Count].ReadDouble( cPathStr );
			++Count;
		}

		chdir ( ".." );
	}
	chdir ( ".." );
}
#endif // _WIN32

/**
 * 음악을 정렬시킨다.
 * 링크를 연결한다.
 */
void SortSong()
{
	for(int i=0;;i++ ) {
		if ( i!=0 )
			CSONG[i].Prev=i-1;

		CSONG[i].Next=i+1;

		if ( CSONG[i].getStep1().bpm1 == 0 && CSONG[i].getStep2().bpm1 == 0 ) {
			CSONG[i].Prev=0;
			i--;
			CSONG[i].Next=0;
			CSONG[0].Prev=i;
			break;
		}
	}
}

/**
 *  줌인 줌아웃 시킨다.
 *
 *  @param	curZoom	현재 확대 배율(0 ~ 10배)
 *  @return	다음 확대 배율
 */
int ZoomInOut()
{
	static int	curZoom = 0;
	static int	toogleFlag = 1;

	if(curZoom <= 0) {
		toogleFlag = 1;
	} else if(10 <= curZoom) {
		toogleFlag = -1;
	}

	curZoom += toogleFlag;
	return curZoom;

}

void SelectSong ( void )
{
	static Uint32 current;
	static Uint32 SelectCurrent;
	static int Selected;
	static	int a,b;
	static	int iMove;

	char s[50];

	if ( First==0 )	{
		startTimer=SDL_GetTicks();
		if ( Start1p==false ) {
			bModeMirror1p=false;
			bModeNonstep1p=false;
			bModeSynchro=false;
			bModeUnion1p=false;
			bModeRandom1p=false;
			b4dMix1p=false;
			bModeVanish1p=false;
			bModeSuddenR1p=false;
			bModeRandomS1p=false;

			gSpeed[0].reset();
		}

		if ( Start2p==false )	{
			bModeMirror2p=false;
			bModeNonstep2p=false;
			bModeUnion2p=false;
			bModeRandom2p=false;
			b4dMix2p=false;
			bModeVanish2p=false;
			bModeSuddenR1p=false;
			bModeRandomS1p=false;

			gSpeed[1].reset();
		}
		// paint the background black.
		gScreen.FillRect ( 0, 0 );

		// Draw BackGround as select image.
		gSelectBack.BltFast ( 0, 0, gScreen );

		a=Start1p;b=Start2p;
		First++;
		gSelectSong.Play ( true );
	}

	// 선택화면에서 보이는 좌우 음악을 연결 시킨다.
	SortSong();

	ReadGameInput();

	if ( PressedKey1p[5]==true ) {
		Start1p=true;
	}

	if ( PressedKey2p[5]==true ) {
		Start2p=true;
	}

	// Get 1Player hidden mode.
	int ModeTemp1p = ScanHiddenMode1p();

	if ( ModeTemp1p )
	{
		if ( IntroFlag )
		{
			gIntro.Halt();
			IntroFlag=false;
		}
	}

	switch ( ModeTemp1p )
	{
		case HMODE_SUDDENR:
			bModeSuddenR1p=true;
			bModeVanish1p=false;
			gMode.Play();
			break;
		case HMODE_RANDOMS:
			bModeRandomS1p=true;
			gSpeed[0].step=1;
			gMode.Play();
			break;
		case HMODE_2X:
			gSpeed[0].step=2;
			gMode.Play();
			break;
		case HMODE_4X:
			gSpeed[0].step=4;
			gMode.Play();
			break;
		case HMODE_8X:
			gSpeed[0].step=8;
			gMode.Play();
			break;
		case HMODE_MIRROR:
			bModeMirror1p=true;
			gMode.Play();
			break;
		case HMODE_NONSTEP:
			bModeNonstep1p=true;
			gMode.Play();
			break;
		case HMODE_SYNCHRO:
			bModeSynchro=true;
			gMode.Play();
			break;
		case HMODE_UNION:
			bModeUnion1p=true;
			gMode.Play();
			break;
		case HMODE_RANDOM:
			bModeRandom1p=true;
			gMode.Play();
			break;
		case HMODE_4DMIX:
			gSpeed[0].setRandom();

			b4dMix1p=true;
			gMode.Play();
			break;
		case HMODE_VANISH:
			bModeVanish1p=true;
			bModeSuddenR2p=false;
			gMode.Play();
			break;
			/*		case HMODE_NONSTOPDOUBLE:
						if(Start1p&&Start2p)break;
						Double=true;
						gMode.Play();
						break;*/
		case HMODE_CANCEL:
			gCancel.Play();
			gSpeed[0].reset();

			bModeMirror1p=false;
			bModeNonstep1p=false;
			bModeSynchro=false;
			bModeUnion1p=false;
			bModeRandom1p=false;
			b4dMix1p=false;

			bModeVanish1p=false;
			bModeSuddenR1p=false;
			bModeRandomS1p=false;
			Double=false;
			break;
		default:
			break;
	}

	// Get 2Player hidden mode.
	int ModeTemp2p=ScanHiddenMode2p();
	if ( ModeTemp2p )
		if ( IntroFlag )
		{
			gIntro.Halt();
			IntroFlag=false;
		}

	switch ( ModeTemp2p )
	{
		case HMODE_SUDDENR:
			bModeSuddenR2p=true;
			bModeVanish2p=false;

			gMode.Play();
			break;
		case HMODE_RANDOMS:
			bModeRandomS2p=true;
			gSpeed[1].step=1;

			gMode.Play();
			break;
		case HMODE_2X:
			gSpeed[1].step=2;
			gMode.Play();
			break;
		case HMODE_4X:
			gSpeed[1].step=4;
			gMode.Play();
			break;
		case HMODE_8X:
			gSpeed[1].step=8;
			gMode.Play();
			break;
		case HMODE_MIRROR:
			bModeMirror2p=true;
			gMode.Play();
			break;
		case HMODE_NONSTEP:
			bModeNonstep2p=true;
			gMode.Play();
			break;
		case HMODE_SYNCHRO:
			bModeSynchro=true;
			gMode.Play();
			break;
		case HMODE_UNION:
			bModeUnion2p=true;
			gMode.Play();
			break;
		case HMODE_RANDOM:
			bModeRandom2p=true;
			gMode.Play();
			break;
		case HMODE_4DMIX:
			gSpeed[1].setRandom();

			b4dMix2p=true;
			gMode.Play();
			break;
		case HMODE_VANISH:
			bModeVanish2p=true;
			bModeSuddenR2p=false;
			gMode.Play();
			break;

		case HMODE_CANCEL:
			gCancel.Play();
			gSpeed[1].step=1;
			bModeMirror2p=false;
			bModeNonstep2p=false;
			bModeUnion2p=false;
			bModeRandom2p=false;
			b4dMix2p=false;
			gSpeed[1].step1=1;
			gSpeed[1].step3=1;
			gSpeed[1].step5=1;
			gSpeed[1].step7=1;
			gSpeed[1].step9=1;
			bModeVanish2p=false;
			bModeSuddenR2p=false;
			bModeRandomS2p=false;
			Double=false;
			break;
	}

	// change screen to left.
	if ( ( Start1p && PressedKey1p[1] ) || ( Start2p && PressedKey2p[1] ) )
	{
		if ( IntroFlag )
		{
			gIntro.Halt();
			IntroFlag=false;
		}
		gMove.Play();

		Selected=0;

		iMove=1;
		current = CSONG[CSONG[current].Prev].Prev;
	}

	// change screen to right.
	if ( ( Start1p && PressedKey1p[3] ) || ( Start2p && PressedKey2p[3] ) )
	{
		if ( IntroFlag )
		{
			gIntro.Halt();
			IntroFlag=false;
		}
		gMove.Play();

		Selected=0;

		iMove=-1;
		current = CSONG[CSONG[current].Next].Next;
	}

	// select the left song.
	if ( ( Start1p && PressedKey1p[7] ) || ( Start2p && PressedKey2p[7] ) )
	{
		// if 7button pressed twice then move to the next stage.
		if ( Selected==7 )
		{
			SelectCurrent=current;
			PressedKey1p[0]=3;
		}
		// if 7button pressed once then select music.
		else
		{
			// if intro music is playing then stop.
			if ( IntroFlag )
			{
				gIntro.Halt();
				IntroFlag=false;
			}

			// if moving music is playing then stop.
			gMove.Play();

			// start New intro music.
			gIntro.Halt();
			if ( access ( CSONG[current].IntroWavPath,04 ) ==0 )
			{
				IntroFlag=true;
				gIntro.Load ( CSONG[current].IntroWavPath );
				gIntro.Play ( true );
			}
			else if ( access ( CSONG[current].IntroMp3Path,04 ) ==0 )
			{
				IntroFlag=true;
				gIntro.Load ( CSONG[current].IntroMp3Path );
				gIntro.Play ( true );
			}

			Selected=7;
		}
	}

	// select the right song.
	if ( ( Start1p && PressedKey1p[9] ) || ( Start2p && PressedKey2p[9] ) )
	{
		// if 9button pressed twice then move to the next stage.
		if ( Selected==9 )
		{
			SelectCurrent=CSONG[current].Next;
			PressedKey1p[0]=3;
		}
		// if 9button pressed once select the right song.
		else
		{
			// intro sound stop.
			if ( IntroFlag )
			{
				gIntro.Halt();
				IntroFlag=false;
			}

			// moving sound stop.
			gMove.Play();

			// selected song intro sound start.
			gIntro.Halt();
			if ( access ( CSONG[CSONG[current].Next].IntroWavPath,04 ) ==0 )
			{
				IntroFlag=true;
				gIntro.Load ( CSONG[CSONG[current].Next].IntroWavPath );
				gIntro.Play ( true );
			}
			else if ( access ( CSONG[CSONG[current].Next].IntroMp3Path,04 ) ==0 )
			{
				IntroFlag=true;
				gIntro.Load ( CSONG[CSONG[current].Next].IntroMp3Path );
				gIntro.Play ( true );
			}

			Selected=9;
		}
	}

	// Game Start( Change the Next Stage. )
	if ( PressedKey1p[0]==3 )
	{
		PressedKey1p[0]=0;

		// Game Mode setting.
		Song &	selectedSong = CSONG[SelectCurrent];
		if ( selectedSong.HaveCrazy==true ) GameMode=MODE_CRAZY,Double=false;
		else if ( selectedSong.HaveDouble==true ) GameMode=MODE_DOUBLE,Double=true;
		else if ( selectedSong.HaveEasy==true ) GameMode=MODE_EASY,Double=false;
		else if ( selectedSong.HaveHard==true ) GameMode=MODE_HARD,Double=false;
		else return;

		// Couple mode setting.
		if ( Start1p && Start2p )
		{
			Couple=true;
			if ( selectedSong.HaveCouple==false )
				bModeSynchro=true;
		}
		else
			Couple=false;

		// Cancel sound restart.
		gCancel.Play();

		// selected song background setting.
		gSongBack.LoadBmp ( selectedSong.BgImgPath );

		// selected song title setting.
		gSongTitle.LoadBmp ( selectedSong.TitleImgPath );

		// if background is not set then background set title image.
		if ( !gSongBack.IsLoaded() )
		{
			if ( gSongTitle.IsLoaded() )
				gSongBack.LoadBmp ( selectedSong.TitleImgPath );
			else
				gSongBack.LoadBmp ( "Images/Back.bmp" );
		}

		if ( !gSongTitle.IsLoaded() )
			gSongTitle.LoadBmp ( "Images/NoTitle.bmp" );

		// draw title image.
		gSongTitle.BltFast ( 0, 0, gScreen );

		// double mode
		if ( GameMode == MODE_DOUBLE ) {
			memcpy ( &Data_Double, &selectedSong.getStep1().step, sizeof ( selectedSong.getStep1().step ) );
		}
		// couple mode
		else if ( Couple==true ) {
			switch ( GameMode )		{
				case MODE_CRAZY:
					if ( bModeSynchro )	{
						memcpy ( &Data,  &selectedSong.getStep1().step, sizeof ( selectedSong.getStep1().step ) );
						memcpy ( &Data1, &selectedSong.getStep1().step, sizeof ( selectedSong.getStep1().step ) );

						for( int i=0;i<MAX_DATA;i++ )	{
							Data1[i][5] = Data1[i][0];
							Data1[i][6] = Data1[i][1];
							Data1[i][7] = Data1[i][2];
							Data1[i][8] = Data1[i][3];
							Data1[i][9] = Data1[i][4];
						}
					}
					else	{
						memcpy ( &Data, &selectedSong.getStep2().step, sizeof ( selectedSong.getStep2().step ) );
						memcpy ( &Data1,&selectedSong.getStep2().step, sizeof ( selectedSong.getStep2().step ) );
					}
					break;

				case MODE_EASY:
					if ( bModeSynchro )	{
						memcpy ( &Data, &selectedSong.getStep1().step, sizeof ( selectedSong.getStep1().step ) );
						memcpy ( &Data1,&selectedSong.getStep1().step, sizeof ( selectedSong.getStep1().step ) );

						for ( int i=0;i<MAX_DATA;i++ )
						{
							Data1[i][5] = Data1[i][0];
							Data1[i][6] = Data1[i][1];
							Data1[i][7] = Data1[i][2];
							Data1[i][8] = Data1[i][3];
							Data1[i][9] = Data1[i][4];
						}
					}
					else	{
						memcpy ( &Data, &selectedSong.getStep2().step, sizeof ( selectedSong.getStep2().step ) );
						memcpy ( &Data1,&selectedSong.getStep2().step, sizeof ( selectedSong.getStep2().step ) );
					}
					break;

				case MODE_HARD:
					if ( bModeSynchro )	{
						memcpy ( &Data, &selectedSong.getStep1().step, sizeof ( selectedSong.getStep1().step ) );
						memcpy ( &Data1,&selectedSong.getStep1().step, sizeof ( selectedSong.getStep1().step ) );

						for ( int i=0;i<MAX_DATA;i++ )	{
							Data1[i][5]=Data1[i][0];
							Data1[i][6]=Data1[i][1];
							Data1[i][7]=Data1[i][2];
							Data1[i][8]=Data1[i][3];
							Data1[i][9]=Data1[i][4];
						}
					}
					else	{
						memcpy ( &Data,&selectedSong.getStep2().step,sizeof ( selectedSong.getStep2().step ) );
						memcpy ( &Data1,&selectedSong.getStep2().step,sizeof ( selectedSong.getStep2().step ) );
					}
					break;
			}
		}
		// single mode.
		else	{
			switch ( GameMode )	{
				case MODE_CRAZY:
					if ( Start1p )
						memcpy ( &Data, selectedSong.getStep1().step, sizeof ( Data ) );
					else	{// 2p play
						memcpy ( &Data, selectedSong.getStep1().step, sizeof ( Data ) );
						memcpy ( &Data1,selectedSong.getStep1().step, sizeof ( Data ) );
						for ( int i=0;i<MAX_DATA;i++ )	{
							Data1[i][5] = Data[i][0];
							Data1[i][6] = Data[i][1];
							Data1[i][7] = Data[i][2];
							Data1[i][8] = Data[i][3];
							Data1[i][9] = Data[i][4];
						}
					}
					break;

				case MODE_EASY:
					if ( Start1p )
						memcpy ( &Data,selectedSong.getStep1().step,sizeof ( Data ) );
					else	{
						memcpy ( &Data,  selectedSong.getStep1().step, sizeof ( Data ) );
						memcpy ( &Data1, selectedSong.getStep1().step, sizeof ( Data ) );
						for ( int i=0;i<MAX_DATA;i++ )	{
							Data1[i][5] = Data[i][0];
							Data1[i][6] = Data[i][1];
							Data1[i][7] = Data[i][2];
							Data1[i][8] = Data[i][3];
							Data1[i][9] = Data[i][4];
						}
					}
					break;

				case MODE_HARD:
					if ( Start1p )
						memcpy ( &Data,selectedSong.getStep1().step,sizeof ( Data ) );
					else	{// 2p play
						memcpy ( &Data,  selectedSong.getStep1().step, sizeof ( Data ) );
						memcpy ( &Data1, selectedSong.getStep1().step, sizeof ( Data ) );
						for ( int i=0;i<MAX_DATA;i++ )	{
							Data1[i][5] = Data[i][0];
							Data1[i][6] = Data[i][1];
							Data1[i][7] = Data[i][2];
							Data1[i][8] = Data[i][3];
							Data1[i][9] = Data[i][4];
						}
					}
					break;
			}
		}

		bpm=selectedSong.getStep1().bpm1;
		bpm2=selectedSong.getStep1().bpm2;
		bpm3=selectedSong.getStep1().bpm3;
		bunki=selectedSong.getStep1().bunki1;
		bunki2=selectedSong.getStep1().bunki2;

		switch ( GameMode )	{
			case MODE_CRAZY:
			case MODE_HARD:
			case MODE_EASY:
			case MODE_DOUBLE:
				start = selectedSong.getStep1().start1;
				start2= selectedSong.getStep1().start2;
				start3= selectedSong.getStep1().start3;
				tick  = selectedSong.getStep1().tick;
				break;

			default:
			{
				char	buff[256] = { 0, };
				sprintf ( buff, "MODE ERROR" __FILE__ " line:%d\n", __LINE__ );
				DisplayMessage ( 0, 0, buff );
			}
			break;
		}

		strcpy ( SongName,  selectedSong.PlayWavPath );
		strcpy ( SongName2, selectedSong.PlayMp3Path );
		strcpy ( SongName3, selectedSong.PlayMpgPath );
		strcpy ( Title,     selectedSong.SongTitle );

		Judgement1p=NONE;
		Judgement2p=NONE;

		if ( GameMode==MODE_DOUBLE )
			g_ProgramState=DOUBLEST;
		else
			g_ProgramState=STAGE1;

		if ( GameMode==MODE_DOUBLE )	{
			if ( Start1p && Start2p )
				Start2p=false;
		}
		Selected=0;

		gSelectSong.Halt();
		if ( IntroFlag )	{
			gIntro.Halt();
			IntroFlag=false;
		}

		// open play song.
		if ( access ( SongName,04 ) ==0 )	{
			SongFlag=true;
			gSong.Load ( SongName );
		}
		else if ( access ( SongName2,04 ) ==0 )	{
			SongFlag=true;
			gSong.Load ( SongName2 );
		}
		else
			SongFlag=false;

		// paint background black.
		gScreen.FillRect ( 0, 0 );

		First=0;
		Combo1p=0;
		Combo2p=0;

		cPerfect1p=0;
		cGreat1p=0;
		cGood1p=0;
		cBad1p=0;
		cMiss1p=0;
		cMaxCombo1p=0;

		cPerfect2p=0;
		cGreat2p=0;
		cGood2p=0;
		cBad2p=0;
		cMiss2p=0;
		cMaxCombo2p=0;

		dwState=0;
		dwState2=0;

		start1=0;

		PressedKey1p[0]=0;
		PressedKey2p[0]=0;
		SelectCurrent=0;

		return;

	}

	// Draw background select image.
	gSelectBack.BltFast ( 0, 0, gScreen );

	curTimer = SDL_GetTicks();

	int i2;

	i2= ( int ) ( curTimer-startTimer ) /1000;
	sprintf ( s,"%02d", ( 40 - i2 ) );

	if ( 40 <= i2 )
	{
		if ( Selected == 7 )
			SelectCurrent = current, PressedKey1p[0]=3;
		else if ( Selected == 9 )
			SelectCurrent = CSONG[current].Next, PressedKey1p[0]=3;
		else
			SelectCurrent = current,PressedKey1p[0]=3;
	}

	DisplayNumber ( 560,8,s );

	// 선택된 타이틀을 줌인 줌아웃 시킨다.
	int zoom = ZoomInOut();

	SDL_Rect Screen;

	// selected left song.
	if ( Selected == 7 )	{
		Screen.y = 50 - zoom;
		Screen.h = DISCSIZE_Y;
		Screen.x = 10 - zoom;
		Screen.w = DISCSIZE_X + zoom + zoom;
	}
	// unselected left song.
	else	{
		Screen.y = 50;
		Screen.h = DISCSIZE_Y;
		Screen.x = 10;
		Screen.w = DISCSIZE_X;
	}

	// change right screen.
	SDL_Rect DiscSize;	// TODO: 정리 대상
	DiscSize.y = 0;
	DiscSize.x = 0;
	DiscSize.w = 300;
	DiscSize.h = 200;

	if ( iMove<0 )	{
		if ( iMove<=-640 )
			iMove=0;
		else	{
			iMove-=8;

			ClpBlt3 ( 10+iMove,50, *CSONG[CSONG[CSONG[current].Prev].Prev].mpDiskImage, DiscSize );
			ClpBlt3 ( 650+iMove,50, *CSONG[current].mpDiskImage, DiscSize );
		}
	}
	// change left screen.
	else if ( iMove>0 )	{
		if ( iMove>=640 )
			iMove=0;
		else	{
			iMove+=8;

			ClpBlt3 ( -630+iMove,50, *CSONG[current].mpDiskImage, DiscSize );
			ClpBlt3 ( 10+iMove,50, *CSONG[CSONG[CSONG[current].Next].Next].mpDiskImage, DiscSize );
		}
	}
	else if ( iMove==0 ) {
		CSONG[current].mpDiskImage->BltFast ( Screen.x, Screen.y, gScreen, &DiscSize );
	}

	// selected right song.
	if ( Selected == 9 )	{
		// zoom the diskImage.
		Screen.y = 50 - zoom;
		Screen.h = DISCSIZE_Y + zoom + zoom;
		Screen.x = 330 - zoom;
		Screen.w = DISCSIZE_X + zoom + zoom;
	}
	// unselected right song.
	else	{
		// make normal size the diskImage.
		Screen.y = 50;
		Screen.h = DISCSIZE_Y;
		Screen.x = 330;
		Screen.w = DISCSIZE_X;
	}

	// draw left disk image.
	if ( iMove<0 )	{
		if ( iMove<=-640 )
			iMove=0;
		else	{
			iMove-=8;

			ClpBlt3 ( 330+iMove,50, *CSONG[CSONG[CSONG[CSONG[current].Prev].Prev].Next].mpDiskImage, DiscSize );
			ClpBlt3 ( 970+iMove,50, *CSONG[CSONG[current].Next].mpDiskImage, DiscSize );
		}
	}
	// draw right disk image.
	else if ( iMove>0 )	{
		if ( iMove>=640 )
			iMove=0;
		else	{
			iMove+=8;
			ClpBlt3 ( -310+iMove,50, *CSONG[CSONG[current].Next].mpDiskImage, DiscSize );
			ClpBlt3 ( 330+iMove,50, *CSONG[CSONG[CSONG[CSONG[current].Next].Next].Next].mpDiskImage, DiscSize );
		}
	}
	else if ( iMove==0 ) {
		CSONG[CSONG[current].Next].mpDiskImage->BltFast ( Screen.x, Screen.y, gScreen, &DiscSize );
	}

	// draw shiftleft image.
	gShiftLeft.BltFast ( 0, 250, gScreen );

	// draw shiftright image.
	gShiftRight.BltFast ( 320, 250, gScreen );

	// print song title to the screen.
	if ( Selected == 7 )
		DisplayMessage ( 200, 300, CSONG[current].SongTitle );
	else if ( Selected==9 )
		DisplayMessage ( 200, 300, CSONG[CSONG[current].Next].SongTitle );

	// draw 1p mode pictures.
	if ( bModeMirror1p )
		DrawMode ( 0,200,HMODE_MIRROR );
	if ( bModeNonstep1p )
		DrawMode ( 0,240,HMODE_NONSTEP );
	if ( bModeSynchro )
		DrawMode ( 0,280,HMODE_SYNCHRO );
	if ( bModeUnion1p )
		DrawMode ( 0,320,HMODE_UNION );
	if ( bModeRandom1p )
		DrawMode ( 0,360,HMODE_RANDOM );
	if ( bModeVanish1p )
		DrawMode ( 0,400,HMODE_VANISH );
	if ( gSpeed[0].step>1 )
		DrawMode ( 0,160,HMODE_2X );

	// draw 2p mode pictures.
	if ( bModeMirror2p )
		DrawMode ( 600,200,HMODE_MIRROR );
	if ( bModeNonstep2p )
		DrawMode ( 600,240,HMODE_NONSTEP );
	if ( bModeUnion2p )
		DrawMode ( 600,320,HMODE_UNION );
	if ( bModeRandom2p )
		DrawMode ( 600,360,HMODE_RANDOM );
	if ( bModeVanish2p )
		DrawMode ( 600,400,HMODE_VANISH );
	if ( gSpeed[1].step>1 )
		DrawMode ( 600,160,HMODE_2X );

	// Draw level icon to next to the title image.
	if ( CSONG[current].HaveDouble && iMove==0 )
		gDoubleIcon.BltFast ( 0,50, gScreen );
	if ( CSONG[CSONG[current].Next].HaveDouble && iMove==0 )
		gDoubleIcon.BltFast ( 320,50, gScreen );

	if ( CSONG[current].HaveCrazy && iMove==0 )
		gCrazyIcon.BltFast ( 0,50, gScreen );
	if ( CSONG[CSONG[current].Next].HaveCrazy && iMove==0 )
		gCrazyIcon.BltFast ( 320,50, gScreen );

	if ( CSONG[current].HaveHard && iMove==0 )
		gHardIcon.BltFast ( 0,50, gScreen );
	if ( CSONG[CSONG[current].Next].HaveHard && iMove==0 )
		gHardIcon.BltFast ( 320,50, gScreen );

	if ( CSONG[current].HaveEasy && iMove==0 )
		gEasyIcon.BltFast ( 0,50, gScreen );
	if ( CSONG[CSONG[current].Next].HaveEasy && iMove==0 )
		gEasyIcon.BltFast ( 320,50, gScreen );

	// Draw to screen "FREE PLAY!"
	SDL_Rect	sRect;
	sRect.x = 0;
	sRect.y = 46;
	sRect.w = 220;
	sRect.h = 69-46;
	gStateComment.SetAlpha ( 255 );
	gStateComment.BltFast ( 210, 450, gScreen, &sRect );


	if ( Start1p )
	{
		if ( a==0 )
		{
			a++;
			if ( Start1p && Start2p )
				Couple=true;
			else
				Couple=false;
		}
	}
	if ( Start2p )
	{
		if ( b==0 )
		{
			b++;
			if ( Start1p && Start2p )
				Couple=true;
			else
				Couple=false;
		}
	}

	if ( Start1p==false )
	{
		// Draw to screen (410, 450) "PRESS CENTER BUTTON"
		sRect.x = 0;
		sRect.y = 0;
		sRect.w = 220;
		sRect.h = 23;

		gStateComment.SetAlpha ( ALPHA );
		gStateComment.BltFast ( 10, 450, gScreen, &sRect );
	}

	if ( Start2p==false )
	{
		// Draw to screen (410, 450) "PRESS CENTER BUTTON"
		sRect.x = 0;
		sRect.y = 0;
		sRect.w = 220;
		sRect.h = 23;

		gStateComment.SetAlpha ( ALPHA );
		gStateComment.BltFast ( 410, 450, gScreen, &sRect );
	}

	// Alpha
	ALPHA += inc;
	if ( ALPHA > 256 )
	{
		ALPHA = 256;
		inc = -10;
	}
	else if ( ALPHA < 0 )
	{
		ALPHA = 0;
		inc = 10;
	}
}

int	ScanHiddenMode1p ( void )
{
	static char IntKey1p[10];
	int i;

	if ( PressedKey1p[1] || PressedKey1p[3] || PressedKey1p[5] || PressedKey1p[7] || PressedKey1p[9] )
		for ( i=0;i<7;i++ ) IntKey1p[i]=IntKey1p[i+1];

	if ( PressedKey1p[1] ) IntKey1p[7]='1';
	if ( PressedKey1p[3] ) IntKey1p[7]='3';
	if ( PressedKey1p[5] ) IntKey1p[7]='5';
	if ( PressedKey1p[7] ) IntKey1p[7]='7';
	if ( PressedKey1p[9] ) IntKey1p[7]='9';

	// 2배속 ?�니??
	if ( strcmp ( IntKey1p,"55755595" ) ==0 )
	{
		IntKey1p[7]='0';
		return HMODE_2X;
	}
	// 4배속 ?�니??
	else if ( strcmp ( IntKey1p,"55355755" ) ==0 )
	{
		IntKey1p[7]='0';
		return HMODE_4X;
	}
	// 8배속 ?�니??
	else if ( strcmp ( IntKey1p,"55153555" ) ==0 )
	{
		IntKey1p[7]='0';
		return HMODE_8X;
	}
	// 배니???�니??
	else if ( strcmp ( IntKey1p,"55975315" ) ==0 )
	{
		IntKey1p[7]='0';
		return HMODE_VANISH;
	}
	// 미러모드 ?�니??
	else if ( strcmp ( IntKey1p,"55159357" ) ==0 )
	{
		IntKey1p[7]='0';
		return HMODE_MIRROR;
	}
	// ?�덤모드 ?�니??
	else if ( strcmp ( IntKey1p,"51535957" ) ==0 )
	{
		IntKey1p[7]='0';
		return HMODE_RANDOM;
	}
	/*	// 커플모드 ?�니??
		else if(strcmp(IntKey1p,"55979755")==0)
		{
			IntKey1p[7]='0';
			return HMODE_COUPLE;
		}
	*/	// ?�크�?모드?�니??
	else if ( strcmp ( IntKey1p,"55797955" ) ==0 )
	{
		IntKey1p[7]='0';
		return HMODE_SYNCHRO;
	}
	// ?�스?�모???�니??
	else if ( strcmp ( IntKey1p,"79579579" ) ==0 )
	{
		IntKey1p[7]='0';
		return HMODE_NONSTEP;
	}
	/*	// ?�스?�더�??�니??
		else if(strcmp(IntKey1p,"17159395")==0)
		{
			IntKey1p[7]='0';
			return HMODE_NONSTOPDOUBLE;
		}*/
	// ?�니?�모???�니??
	else if ( strcmp ( IntKey1p,"13573159" ) ==0 )
	{
		IntKey1p[7]='0';
		return HMODE_UNION;
	}
	/*	// ?�?�트???�니??
		else if(strcmp(IntKey1p,"95197537")==0)
		{
			IntKey1p[7]='0';
			return HMODE_SELECTALL;
		}*/
	// 캔슬 ?�니??
	else if ( strcmp ( IntKey1p,"55555555" ) ==0 )
	{
		IntKey1p[7]='0';
		return HMODE_CANCEL;
	}
	else if ( strcmp ( IntKey1p,"55955575" ) ==0 )
	{
		IntKey1p[7]='0';
		return HMODE_4DMIX;
	}
	else if ( strcmp ( IntKey1p,"79513579" ) ==0 )
	{
		IntKey1p[7]='0';
		return HMODE_SUDDENR;
	}
	else if ( strcmp ( IntKey1p,"17931793" ) ==0 )
	{
		IntKey1p[7]='0';
		return HMODE_RANDOMS;
	}
	else
		return HMODE_NONE;
}

int	ScanHiddenMode2p ( void )
{
	static char IntKey2p[10];
	int i;

	if ( PressedKey2p[1] || PressedKey2p[3] || PressedKey2p[5] || PressedKey2p[7] || PressedKey2p[9] )
		for ( i=0;i<7;i++ ) IntKey2p[i]=IntKey2p[i+1];

	if ( PressedKey2p[1] ) IntKey2p[7]='1';
	if ( PressedKey2p[3] ) IntKey2p[7]='3';
	if ( PressedKey2p[5] ) IntKey2p[7]='5';
	if ( PressedKey2p[7] ) IntKey2p[7]='7';
	if ( PressedKey2p[9] ) IntKey2p[7]='9';

	if ( strcmp ( IntKey2p,"55755595" ) ==0 )
	{
		IntKey2p[7]='0';
		return HMODE_2X;
	} // 2배속 ?�니??
	else if ( strcmp ( IntKey2p,"55355755" ) ==0 )
	{
		IntKey2p[7]='0';
		return HMODE_4X;
	} // 4배속 ?�니??
	else if ( strcmp ( IntKey2p,"55153555" ) ==0 )
	{
		IntKey2p[7]='0';
		return HMODE_8X;
	} // 8배속 ?�니??
	else if ( strcmp ( IntKey2p,"55975315" ) ==0 )
	{
		IntKey2p[7]='0';
		return HMODE_VANISH;
	} // 배니???�니??
	else if ( strcmp ( IntKey2p,"55159357" ) ==0 )
	{
		IntKey2p[7]='0';
		return HMODE_MIRROR;
	} // 미러모드 ?�니??
	else if ( strcmp ( IntKey2p,"51535957" ) ==0 )
	{
		IntKey2p[7]='0';
		return HMODE_RANDOM;
	} // ?�덤모드 ?�니??
	/*	else if(strcmp(IntKey2p,"55979755")==0)
		{
			IntKey2p[7]='0';
			return HMODE_COUPLE;
		} // 커플모드 ?�니??
	*/
	else if ( strcmp ( IntKey2p,"55797955" ) ==0 )
	{
		IntKey2p[7]='0';
		return HMODE_SYNCHRO;
	} // ?�크�?모드?�니??
	else if ( strcmp ( IntKey2p,"79579579" ) ==0 )
	{
		IntKey2p[7]='0';
		return HMODE_NONSTEP;
	} // ?�스?�모???�니??
	/*	else if(strcmp(IntKey2p,"17159395")==0)
		{
			IntKey2p[7]='0';
			return HMODE_NONSTOPDOUBLE;
		} // ?�스?�더�??�니??*/
	else if ( strcmp ( IntKey2p,"13573159" ) ==0 )
	{
		IntKey2p[7]='0';
		return HMODE_UNION;
	} // ?�니?�모???�니??
	/*	else if(strcmp(IntKey2p,"95197537")==0)
		{
			IntKey2p[7]='0';
			return HMODE_SELECTALL;
		} // ?�?�트???�니?? */
	else if ( strcmp ( IntKey2p,"55555555" ) ==0 )
	{
		IntKey2p[7]='0';
		return HMODE_CANCEL;
	} // 캔슬 ?�니??

	else if ( strcmp ( IntKey2p,"55955575" ) ==0 )
	{
		IntKey2p[7]='0';
		return HMODE_4DMIX;
	}
	else if ( strcmp ( IntKey2p,"79513579" ) ==0 )
	{
		IntKey2p[7]='0';
		return HMODE_SUDDENR;
	}
	else if ( strcmp ( IntKey2p,"17931793" ) ==0 )
	{
		IntKey2p[7]='0';
		return HMODE_RANDOMS;
	}
	else return HMODE_NONE;
}

void DrawMode ( int x, int y, int Mode )
{
	SDL_Rect	modeRect = {0, };

	modeRect.w = 37;
	modeRect.h = 37;

	switch ( Mode )
	{
		case HMODE_2X:
		case HMODE_4X:
		case HMODE_8X:
			modeRect.x=0;
			break;
		case HMODE_VANISH:
			modeRect.x=37;
			break;
		case HMODE_MIRROR:
			modeRect.x = 37*2;
			break;
		case HMODE_RANDOM:
			modeRect.x = 37*3;
			break;
		case HMODE_COUPLE:
			modeRect.x = 37*4;
			break;
		case HMODE_SYNCHRO:
			modeRect.x = 37*5;
			break;
		case HMODE_NONSTEP:
			modeRect.x = 37*6;
			break;
		case HMODE_UNION:
			modeRect.x = 37*7;
			break;
		default:
			break;
	}

	gModeIcon.BltFast ( x, y, gScreen, &modeRect );
}
