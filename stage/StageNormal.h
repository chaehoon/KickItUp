#ifndef _KICKITUP_STAGENORMAL_H
#define _KICKITUP_STAGENORMAL_H

#include "../common.h"
#include "Stage.h"
#include "../Input/iInputHandler.h"
#include "../Video/Animation.h"
#include <deque>
#include <vector>

using std::deque;
using std::vector;

class Surface;
class Sound;
class SoundStore;
class SurfaceStore;
class InputStore;
class Input;
class Context;
class Song;

class StageNormal   :   public Stage , public iInputHandler
{
private:
    enum ePointZone {
        ePointZone_Min = 0,
        ePointZone_Perfect = ePointZone_Min,
        ePointZone_Great,
        ePointZone_Good,
        ePointZone_Bad,
        ePointZone_Miss,
        ePointZone_None,
        ePointZone_Max,
    };

    static const int    BACK_ARROW_Y    = 55;
    static const int    STEP_ARROW_SIZE = 60;
    Context &   m_context;

    Sound *     m_pBGM;         // background music.
    Surface *   m_pBG;          // background image.
    Surface *   m_pBackArrows[2];      //
    Surface *   m_pGaugeWaku;
    Surface *   m_pGauge;
    Surface *   m_pStepArrows;      // arrows.
    SDL_Rect    m_rtArrows[5][6];   // 화살표 조각.
    Song *      m_pSelectedSong;
    Animation   m_aniPushArrows[5];     // 화살표 버튼을 누를때 버튼의 애니메이션.
    Animation   m_AniStepArraws[5];     // Step 화살표.
    Animation   m_aniCrashArrows[5];          // Crash 효과 애니메이션.

    int         m_start[3];
    int         m_tick;
    double      m_bpm;
    double      m_stepGapTime;                  ///< 1step 시간(ms).

    int         m_y;                            ///< 화살표를 출력할 y 기준.
    int         m_playingTime;                  ///< 현재 재생중인 음악 시간.
    int         m_nStepIdx;                     ///< 현재 스탭 index
    double      m_dStepIdx;                     ///< 현재 스탭 index - 세밀한.

    double      m_stepSpeed;                    ///< 배속
    double      m_distancePerStep;              ///< 1 Step당 Step화살표가 움직이는 거리(Pixel)
    int         m_addedStep;                    ///< 화면 위까지 Step Arrow가 추가되는 step개수

    vector<bool>    m_buttonState;              ///< 버튼이 눌러진 상태.
    int         m_judgedIdx;                    ///< 판정이 완료된 index
    vector<bool>    m_showStep;                 ///< 스탭을 화면에 뿌린것인지 아닌지.
    int         m_pointTimeZone[ePointZone_Max];    ///< 판정 점수시간
    int         m_judgePoint[ePointZone_Max];   ///< 각각의 판정 개수.

public:
    StageNormal( Context & context );
    virtual ~StageNormal(void);

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
    void    _drawBackGround( unsigned int playTime ) const;
	double	_getIndexByTime( unsigned int time ) const;
    int     _getTimeByIndex( int stepIndex ) const;
    void    _drawBackArrow( unsigned int playTime ) const;
    void    _drawGauge() const;
    void    _judge();                           // 판정.
    bool    _isPress( const char * btnStr ) const;    // 해당 버튼이 눌러 졌는지 검사.
    ePointZone _getPoint( const double baseStepIndex, const int toJudgeStepIndex ) const;   // 점수판정
};

#endif // _KICKITUP_STAGENORMAL_H
