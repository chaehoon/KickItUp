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
#include <vector>
#include <string>
#include <sys/stat.h>
#include <dirent.h>

#include "Common.h"
#include "Main.h"
#include "Input.h"
#include "Select.h"
#include "Result.h"
#include "Song.h"
#include "Surface.h"
#include "Chunk.h"

using std::string;
using std::vector;

// Hidden Mode define
#define	HMODE_CANCEL	99
#define HMODE_2X		1
#define	HMODE_4X		2
#define	HMODE_8X		3
#define	HMODE_VANISH	4
#define HMODE_MIRROR	5
#define HMODE_RANDOM	6
#define HMODE_COUPLE	7
#define	HMODE_SYNCHRO	8
#define	HMODE_NONSTEP	9
#define	HMODE_NONSTOPDOUBLE	10
#define	HMODE_UNION		11
#define HMODE_SELECTALL	12
#define HMODE_4DMIX		13
#define	HMODE_NONE		0
#define HMODE_SUDDENR	14
#define	HMODE_RANDOMS	15

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
extern	Uint32				bunki,bunki2;

extern char					SongName[PATH_LEN+1];
extern char					SongName2[PATH_LEN+1];
extern char					SongName3[PATH_LEN+1];
extern char					Title[PATH_LEN+1];
extern	int					start1;

extern	char				gchGameMode;

extern	char				Couple;
extern	char				Double;
// End of the COUPLE DATA

extern Chunk			gSndMode;
extern Chunk            gSndCancel;
extern Chunk			gSndMove;
extern Chunk			gSndSelect;

extern char First;
extern bool	IntroFlag;

Uint32	startTimer, curTimer;
static char sKeyHistory[2][10];

void _init();
void _setHiddenMode();
int	_scanHiddenMode(const int player, const Uint32 * pPressedKey);
void _drawMode(const int x, const int y, const int Mode);
void _sortSong();
int _zoomInOut();
void _setHiddenMode(const int player, const int mode);
void _chageSelectScreen(int * selectedIndex, int * selected, int * move);
bool _isLeftMoved();
void _moveToLeftSong(int * pCurrent, int * pSelected, int * pSelectCurrent);
bool _isRightMoved();
void _moveToRightSong(int * pCurrent, int * pSelected, int * pSelectCurrent);
bool _isStartBtnPressed();
void _startStage(int * pSelected, int * pSelectCurrent);
void _drawScreen(const int * pCurrent, const int * pSelected, int * pIMove);
bool _getDirs ( vector<string> & dirs );


void SelectSong ( void )
{
	static 	int _current;
	static 	int _selectCurrent;
	static 	int _selected;
	static	int _iMove;

	if ( First==0 )	{
		_init();
		First++;
	}

	ReadGameInput();

	if ( PressedKey1p[5]==true ) {
		Start1p=true;
	}

	if ( PressedKey2p[5]==true ) {
		Start2p=true;
	}

	_setHiddenMode();

	// 1, 3 번 버튼을 누르면 화면을 좌우로 움직인다.
	_chageSelectScreen(&_current, &_selected, &_iMove);

	// select the left song.
	if(_isLeftMoved()) {
		_moveToLeftSong(&_current, &_selected, &_selectCurrent);
	}

	if(_isRightMoved()) {
		_moveToRightSong(&_current, &_selected, &_selectCurrent);
	}

	if(_isStartBtnPressed()) {
		_startStage(&_selected, &_selectCurrent);
		return;
	}

	_drawScreen(&_current, &_selected, &_iMove);
}


void ReadData()
{
	char  cPathStr[PATH_LEN+1] = { 0, };
	int	Count=0;

	if ( !GetRealFileName( "song", cPathStr, sizeof ( cPathStr ) ) )
		return;

	chdir ( cPathStr );
	vector<string> dirs;
	if ( !_getDirs ( dirs ) ) {
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

void _init()
{
	startTimer=SDL_GetTicks();
	if ( Start1p==false ) {
		gMode[0].Reset();
		gSpeed[0].reset();
	}

	if ( Start2p==false )	{
		gMode[1].Reset();
		gSpeed[1].reset();
	}

	// 선택화면에서 보이는 좌우 음악을 연결 시킨다.
	_sortSong();

	// paint the background black.
	gScreen.FillRect ( 0, 0 );

	// Draw BackGround as select image.
	gSelectBack.BltFast ( 0, 0, gScreen );
	gSndSelect.Play ( true );

	Couple = false;
}

/**
 * 현재 디렉토리 리스트를 얻는다.
 * @param	dirs	디렉토리 리스트.
 * @return	디렉토리 리스트가 있는지 없는지.
 */
bool _getDirs ( vector<string> & dirs )
{
	struct dirent * item;
	DIR * dp;

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
		if ( stat ( item->d_name, &stat_p ) == -1 )
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

/**
 * 음악을 정렬시킨다.
 * 링크를 연결한다.
 */
void _sortSong()
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
int _zoomInOut()
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


void _setHiddenMode(const int player, const int mode)
{
	if(mode == HMODE_NONE) {
		return;
	}

	if ( IntroFlag ) {
		gMusicIntro.Halt();
		IntroFlag=false;
	}

	GameMode & gameMode = gMode[player];
	st_HighSpeed & speed = gSpeed[player];

	switch ( mode ) {
		case HMODE_SUDDENR:
			gameMode.Set(GameMode::eMODE_SUDDENR);
			gameMode.UnSet(GameMode::eMODE_VANISH);
			gSndMode.Play();
			break;
		case HMODE_RANDOMS:
			gameMode.Set(GameMode::eMODE_RAMDOMS);
			speed.reset(1);
			gSndMode.Play();
			break;
		case HMODE_2X:
			speed.reset(2);
			gSndMode.Play();
			break;
		case HMODE_4X:
			speed.reset(4);
			gSndMode.Play();
			break;
		case HMODE_8X:
			speed.reset(8);
			gSndMode.Play();
			break;
		case HMODE_MIRROR:
			gameMode.Set(GameMode::eMODE_MIRROR);
			gSndMode.Play();
			break;
		case HMODE_NONSTEP:
			gameMode.Set(GameMode::eMODE_NONSTOP);
			gSndMode.Play();
			break;
		case HMODE_SYNCHRO:
			gameMode.Set(GameMode::eMODE_SYNCHRO);
			gSndMode.Play();
			break;
		case HMODE_UNION:
			gameMode.Set(GameMode::eMODE_UNION);
			gSndMode.Play();
			break;
		case HMODE_RANDOM:
			gameMode.Set(GameMode::eMODE_RAMDOM);
			gSndMode.Play();
			break;
		case HMODE_4DMIX:
			speed.setRandom();
			gameMode.Set(GameMode::eMODE_MIX);
			gSndMode.Play();
			break;
		case HMODE_VANISH:
			gameMode.Set(GameMode::eMODE_VANISH);
			gameMode.UnSet(GameMode::eMODE_SUDDENR);
			gSndMode.Play();
			break;
		case HMODE_CANCEL:
			gSndCancel.Play();
			speed.reset();
			gameMode.Reset();
			break;
		default:
			break;
	}
}

/**
 * set hidden Mode for 1Player
 */
void _setHiddenMode()
{

	// Get 1Player hidden mode.
	_setHiddenMode(0, _scanHiddenMode(0, PressedKey1p));

	// Get 1Player hidden mode.
	_setHiddenMode(1, _scanHiddenMode(1, PressedKey2p));
}


void _chageSelectScreen(int * selectedIndex, int * selected, int * move)
{
	// change screen to left.
	if ( ( Start1p && PressedKey1p[1] ) || ( Start2p && PressedKey2p[1] ) )	{
		if ( IntroFlag ) {
			gMusicIntro.Halt();
			IntroFlag=false;
		}
		gSndMove.Play();

		*selected =0 ;

		*move = 1;
		*selectedIndex = CSONG[CSONG[*selectedIndex].Prev].Prev;
	}

	// change screen to right.
	if ( ( Start1p && PressedKey1p[3] ) || ( Start2p && PressedKey2p[3] ) )	{
		if ( IntroFlag ) {
			gMusicIntro.Halt();
			IntroFlag=false;
		}
		gSndMove.Play();

		*selected = 0;

		*move = -1;
		*selectedIndex = CSONG[CSONG[*selectedIndex].Next].Next;
	}
}

// 음악 왼쪽을 선택하고자 하는가?
bool _isLeftMoved()
{
	if(( Start1p && PressedKey1p[7] ) || ( Start2p && PressedKey2p[7] ))
		return true;
	return false;
}

// 화면을 왼쪽으로 옮김
void _moveToLeftSong(int * pCurrent, int * pSelected, int * pSelectCurrent)
{
	// if 7button pressed twice then move to the next stage.
	if ( *pSelected == 7 )	{
		*pSelectCurrent = *pCurrent;
		PressedKey1p[0] = 3;
	}
	// if 7button pressed once then select music.
	else	{
		// if intro music is playing then stop.
		if ( IntroFlag ) {
			gMusicIntro.Halt();
			IntroFlag=false;
		}

		// if moving music is playing then stop.
		gSndMove.Play();

		// start New intro music.
		Song & curSong = CSONG[*pCurrent];
		gMusicIntro.Halt();
		if ( access ( curSong.IntroWavPath,04 ) ==0 ) {
			IntroFlag=true;
			gMusicIntro.Load ( curSong.IntroWavPath );
			gMusicIntro.Play ( true );
		}
		else if ( access ( curSong.IntroMp3Path,04 ) ==0 ) {
			IntroFlag=true;
			gMusicIntro.Load ( curSong.IntroMp3Path );
			gMusicIntro.Play ( true );
		}

		*pSelected = 7;
	}
}

// 음악 오른쪽을 선택하고자 하는가?
bool _isRightMoved()
{
	if( ( Start1p && PressedKey1p[9] ) || ( Start2p && PressedKey2p[9] ))
		return true;
	return false;
}

// 화면을 오른쪽으로 옮김
void _moveToRightSong(int * pCurrent, int * pSelected, int * pSelectCurrent)
{
	// if 9button pressed twice then move to the next stage.
	if( *pSelected == 9 ) {
		*pSelectCurrent = CSONG[*pCurrent].Next;
		PressedKey1p[0]=3;
	}
	// if 9button pressed once select the right song.
	else {
		// intro sound stop.
		if ( IntroFlag ) {
			gMusicIntro.Halt();
			IntroFlag=false;
		}

		// moving sound stop.
		gSndMove.Play();

		// selected song intro sound start.
		Song & curSong = CSONG[*pCurrent];
		Song & nextSong = CSONG[curSong.Next];
		if ( access ( nextSong.IntroWavPath,04 ) ==0 ) {
			IntroFlag=true;
			gMusicIntro.Load ( nextSong.IntroWavPath );
			gMusicIntro.Play ( true );
		}
		else if ( access ( nextSong.IntroMp3Path,04 ) ==0 ) {
			IntroFlag=true;
			gMusicIntro.Load ( nextSong.IntroMp3Path );
			gMusicIntro.Play ( true );
		}

		*pSelected = 9;
	}

}

bool _isStartBtnPressed()
{
	return PressedKey1p[0] == 3;
}

// Game Start( Change the Next Stage. )
void _startStage(int * pSelected, int * pSelectCurrent)
{
	PressedKey1p[0] = 0;

	// Game Mode setting.
	Song &	selectedSong = CSONG[*pSelectCurrent];
	if ( selectedSong.HaveCrazy==true ) gchGameMode=MODE_CRAZY,Double=false;
	else if ( selectedSong.HaveDouble==true ) gchGameMode=MODE_DOUBLE,Double=true;
	else if ( selectedSong.HaveEasy==true ) gchGameMode=MODE_EASY,Double=false;
	else if ( selectedSong.HaveHard==true ) gchGameMode=MODE_HARD,Double=false;
	else return;

	// Couple mode setting.
	if ( Start1p && Start2p )
	{
		Couple=true;
		if ( selectedSong.HaveCouple==false ) {
			gMode[0].Set(GameMode::eMODE_SYNCHRO);
			gMode[1].Set(GameMode::eMODE_SYNCHRO);
		}
	}
	else
		Couple=false;

	// Cancel sound restart.
	gSndCancel.Play();

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
	if ( gchGameMode == MODE_DOUBLE ) {
		memcpy ( &Data_Double, &selectedSong.getStep1().step, sizeof ( selectedSong.getStep1().step ) );
	}
	// couple mode
	else if ( Couple==true ) {
		switch ( gchGameMode )		{
			case MODE_CRAZY:
				if (gMode[0].IsSet(GameMode::eMODE_SYNCHRO) )	{
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
				if ( gMode[0].IsSet(GameMode::eMODE_SYNCHRO) )	{
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
				if ( gMode[0].IsSet(GameMode::eMODE_SYNCHRO) )	{
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
		switch ( gchGameMode )	{
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

	switch ( gchGameMode )	{
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

	if ( gchGameMode==MODE_DOUBLE )
		g_ProgramState=DOUBLEST;
	else
		g_ProgramState=STAGE1;

	if ( gchGameMode==MODE_DOUBLE )	{
		if ( Start1p && Start2p )
			Start2p=false;
	}

	gSndSelect.Halt();
	if ( IntroFlag )	{
		gMusicIntro.Halt();
		IntroFlag=false;
	}

	// open play song.
	if ( access ( SongName,04 ) ==0 )	{
		SongFlag=true;
		gMusicSong.Load ( SongName );
	}
	else if ( access ( SongName2,04 ) ==0 )	{
		SongFlag=true;
		gMusicSong.Load ( SongName2 );
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

	*pSelectCurrent = 0;
	*pSelected = 0;

	return;
}

void _drawScreen(const int * pCurrent, const int * pSelected, int * pIMove)
{
	// Draw background select image.
	gSelectBack.BltFast ( 0, 0, gScreen );

	curTimer = SDL_GetTicks();

	char s[50];
	const int playSec = ( int ) ( curTimer-startTimer ) / 1000;
	sprintf ( s,"%02d", ( 40 - playSec ) );

	DisplayNumber(560, 8, s);

	// 선택된 타이틀을 줌인 줌아웃 시킨다.
	int zoom = _zoomInOut();

	SDL_Rect Screen;

	// selected left song.
	if ( *pSelected == 7 )	{
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

	const Song & curSong = CSONG[*pCurrent];
	const Song & prevSong = CSONG[curSong.Prev];
	const Song & nextSong = CSONG[curSong.Next];
	if ( *pIMove<0 )	{
		if ( *pIMove<=-640 )
			*pIMove=0;
		else	{
			*pIMove-=8;

			const Song & leftSong = CSONG[prevSong.Prev];
			const Song & rightSong = curSong;

			leftSong.mpDiskImage->BltFast(10+(*pIMove), 50, gScreen, &DiscSize );
			rightSong.mpDiskImage->BltFast(650+(*pIMove), 50, gScreen, &DiscSize );
		}
	}
	// change left screen.
	else if ( *pIMove>0 )	{
		if ( *pIMove>=640 )
			*pIMove=0;
		else	{
			*pIMove+=8;

			const Song & leftSong = curSong;
			const Song & rightSong = CSONG[nextSong.Next];

			leftSong.mpDiskImage->BltFast(-630+(*pIMove), 50, gScreen, &DiscSize );
			rightSong.mpDiskImage->BltFast(10+(*pIMove), 50, gScreen, &DiscSize );
		}
	}
	else if ( *pIMove==0 ) {
		curSong.mpDiskImage->BltFast ( Screen.x, Screen.y, gScreen, &DiscSize );
	}

	// selected right song.
	if ( *pSelected == 9 )	{
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
	if ( *pIMove<0 )	{
		if ( *pIMove<=-640 )
			*pIMove=0;
		else	{
			*pIMove-=8;

			const Song & leftSong = prevSong;
			const Song & rightSong = nextSong;

			leftSong.mpDiskImage->BltFast(330+(*pIMove), 50, gScreen, &DiscSize);
			rightSong.mpDiskImage->BltFast(970+(*pIMove), 50, gScreen, &DiscSize);
		}
	}
	// draw right disk image.
	else if ( *pIMove>0 )	{
		if ( *pIMove>=640 )
			*pIMove=0;
		else	{
			*pIMove+=8;

			const Song & leftSong = nextSong;
			const Song & rightSong = CSONG[CSONG[nextSong.Next].Next];

			leftSong.mpDiskImage->BltFast(-330+(*pIMove), 50, gScreen, &DiscSize);
			rightSong.mpDiskImage->BltFast(330+(*pIMove), 50, gScreen, &DiscSize);

		}
	}
	else if ( *pIMove==0 ) {
		nextSong.mpDiskImage->BltFast ( Screen.x, Screen.y, gScreen, &DiscSize );
	}

	// draw shiftleft image.
	gShiftLeft.BltFast ( 0, 250, gScreen );

	// draw shiftright image.
	gShiftRight.BltFast ( 320, 250, gScreen );

	// print song title to the screen.
	if ( *pSelected == 7 )
		DisplayMessage ( 200, 300, curSong.SongTitle );
	else if ( *pSelected==9 )
		DisplayMessage ( 200, 300, nextSong.SongTitle );

	// draw 1p mode pictures.
	DrawModeIcon(0);
	DrawModeIcon(1);

	// Draw level icon to next to the title image.
	if ( curSong.HaveDouble && *pIMove==0 )
		gDoubleIcon.BltFast ( 0,50, gScreen );
	if ( nextSong.HaveDouble && *pIMove==0 )
		gDoubleIcon.BltFast ( 320,50, gScreen );

	if ( curSong.HaveCrazy && *pIMove==0 )
		gCrazyIcon.BltFast ( 0,50, gScreen );
	if ( nextSong.HaveCrazy && *pIMove==0 )
		gCrazyIcon.BltFast ( 320,50, gScreen );

	if ( curSong.HaveHard && *pIMove==0 )
		gHardIcon.BltFast ( 0,50, gScreen );
	if ( nextSong.HaveHard && *pIMove==0 )
		gHardIcon.BltFast ( 320,50, gScreen );

	if ( curSong.HaveEasy && *pIMove==0 )
		gEasyIcon.BltFast ( 0,50, gScreen );
	if ( nextSong.HaveEasy && *pIMove==0 )
		gEasyIcon.BltFast ( 320,50, gScreen );

	// Draw to screen "FREE PLAY!"
	SDL_Rect	sRect;
	sRect.x = 0;
	sRect.y = 46;
	sRect.w = 220;
	sRect.h = 69-46;
	gStateComment.SetAlpha ( 255 );
	gStateComment.BltFast ( 210, 450, gScreen, &sRect );

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

int	_scanHiddenMode(const int player, const Uint32 * pPressedKey)
{
	char * pKeyHistory = sKeyHistory[player];
	if ( pPressedKey[1] || pPressedKey[3] || pPressedKey[5] || pPressedKey[7] || pPressedKey[9] ) {
		for(int i=0 ; i<7 ; i++ ) {
			pKeyHistory[i] = pKeyHistory[i+1];
		}
	}

	if( pPressedKey[1] )
		pKeyHistory[7]='1';
	if( pPressedKey[3])
		pKeyHistory[7]='3';
	if(pPressedKey[5])
		pKeyHistory[7]='5';
	if(pPressedKey[7])
		pKeyHistory[7]='7';
	if(pPressedKey[9])
		pKeyHistory[7]='9';

	// 2x Mode
	if ( strcmp ( pKeyHistory,"55755595" ) == 0 ) {
		pKeyHistory[7]='0';
		return HMODE_2X;
	}
	// 4x Mode
	else if ( strcmp ( pKeyHistory,"55355755" ) == 0 ) {
		pKeyHistory[7]='0';
		return HMODE_4X;
	}
	// 8x Mode
	else if ( strcmp ( pKeyHistory,"55153555" ) ==0 ) {
		pKeyHistory[7]='0';
		return HMODE_8X;
	}
	// Banish Mode
	else if ( strcmp ( pKeyHistory,"55975315" ) == 0 ) {
		pKeyHistory[7]='0';
		return HMODE_VANISH;
	}
	// 미러모드 Mode
	else if ( strcmp ( pKeyHistory,"55159357" ) == 0 ) {
		pKeyHistory[7]='0';
		return HMODE_MIRROR;
	}
	// Ramdorm Mode
	else if ( strcmp ( pKeyHistory,"51535957" ) == 0 ) {
		pKeyHistory[7]='0';
		return HMODE_RANDOM;
	}
	/*	// couple Mode
		else if(strcmp(IntKey1p,"55979755")==0)
		{
			IntKey1p[7]='0';
			return HMODE_COUPLE;
		}
	*/	// Synchro Mode
	else if ( strcmp ( pKeyHistory,"55797955" ) == 0 ) {
		pKeyHistory[7]='0';
		return HMODE_SYNCHRO;
	}
	// Nonstep Mode
	else if ( strcmp ( pKeyHistory,"79579579" ) == 0 ) {
		pKeyHistory[7]='0';
		return HMODE_NONSTEP;
	}
	/*	// Nonstep double Mode
		else if(strcmp(IntKey1p,"17159395")== 0) {
			IntKey1p[7]='0';
			return HMODE_NONSTOPDOUBLE;
		}*/
	// Union Mode
	else if ( strcmp ( pKeyHistory,"13573159" ) == 0 ) {
		pKeyHistory[7]='0';
		return HMODE_UNION;
	}
	/*	// select Mode
		else if(strcmp(IntKey1p,"95197537") == 0) {
			IntKey1p[7]='0';
			return HMODE_SELECTALL;
		}*/
	// Cancel
	else if ( strcmp ( pKeyHistory,"55555555" ) == 0 ) {
		pKeyHistory[7]='0';
		return HMODE_CANCEL;
	}
	else if ( strcmp ( pKeyHistory,"55955575" ) ==0 ) {
		pKeyHistory[7]='0';
		return HMODE_4DMIX;
	}
	else if ( strcmp ( pKeyHistory,"79513579" ) ==0 ) {
		pKeyHistory[7]='0';
		return HMODE_SUDDENR;
	}
	else if ( strcmp ( pKeyHistory,"17931793" ) ==0 ) {
		pKeyHistory[7]='0';
		return HMODE_RANDOMS;
	}

	return HMODE_NONE;
}


void _drawMode(const int x, const int y, const int Mode)
{
	SDL_Rect	modeRect = {0, };

	modeRect.w = 37;
	modeRect.h = 37;

	switch ( Mode )	{
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

void DrawModeIcon(const int player)
{
	int	x[2] = {0, 600};
	if(gMode[player].IsSet(GameMode::eMODE_MIRROR))
		_drawMode(x[player], 200, HMODE_MIRROR);

	if(gMode[player].IsSet(GameMode::eMODE_NONSTOP))
		_drawMode(x[player], 240, HMODE_NONSTEP);

	if(gMode[player].IsSet(GameMode::eMODE_SYNCHRO))
		_drawMode(x[player], 280, HMODE_SYNCHRO);

	if(gMode[player].IsSet(GameMode::eMODE_UNION))
		_drawMode(x[player], 320, HMODE_UNION);

	if(gMode[player].IsSet(GameMode::eMODE_RAMDOM))
		_drawMode(x[player], 360, HMODE_RANDOM);
	if(gMode[player].IsSet(GameMode::eMODE_VANISH))
		_drawMode(x[player], 400, HMODE_VANISH);

	if(1 < gSpeed[player].step)
		_drawMode(x[player], 160, HMODE_2X);
}
