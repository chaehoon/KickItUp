#ifndef _KICKITUP_STAGESELECT_H
#define _KICKITUP_STAGESELECT_H

#include "../common.h"
#include "Stage.h"
#include "../Input/iInputHandler.h"
#include "../Video/ZoomAnimation.h"
#include <deque>
using std::deque;

class Surface;
class Sound;
class SoundStore;
class SurfaceStore;
class InputStore;
class Input;
class Context;
class Song;
class ZoomAnimation;

class StageSelect	:	public Stage , public iInputHandler
{
private:
	Context &			m_context;

	Surface *	m_pBG;				// BackGround Image.
	Surface *	m_pStateComment;	// 하단 상태 표시
	Surface *	m_pShiftLeft;		// 하단 왼쪽 버튼(다른곡선택버튼)
	Surface *	m_pShiftRight;		// 하단 오른쪽 버튼(다른곡선택버튼)
    Surface *   m_pModeIcon[ePM_Max];        // easy, hard, double crazy icon
	Sound *		m_pBGM;				// BackGround Music
    Sound *     m_pMove;            // shiftRight ShiftLeft Button을 눌렀을때 나는 소리.

    Song *      m_pSongLeft;        // 왼쪽 노래
    Song *      m_pSongRight;       // 오른쪽 노래.
    Song *      m_pSongSelect;      // 선택된 노래.
    Sound *     m_pSoundIntro;      // 선택된 사운드

    ZoomAnimation   m_buttonAni[4];    // 버튼

	deque<eInputName>	m_chatkeys[2];

public:
	StageSelect( Context & context );
	virtual ~StageSelect(void);

	virtual bool 		Initialize();
	virtual void 		Destroy();

	virtual bool		Think( unsigned long delta );
	virtual bool		Render( unsigned long delta );

	virtual void		GetIn();	// get in the stage.
	virtual void		GetOut();	// get out the stage.

	virtual bool		InputHandleMessage( const eInputName name, const eInputState is );

	virtual void		GoPreStage();	///< goto prev stage.
	virtual void		GoNextStage();	///< goto next stage.

private:
    void                _turnLeft();    ///< 왼쪽으로 화면이동
    void                _turnRight();   ///< 오른쪽으로 화면이동
    void                _resetSelectSong(); ///< 선택된곡 리셀
    void                _selectSong( const int direction );      ///< 곡 선택
    void                _selectRightSong();  ///< 오른쪽 곡 선택
    void                _checkHiddenMode(); ///< check hidden mode.
};

#endif // _KICKITUP_STAGESELECT_H
