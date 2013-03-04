#ifndef	_KICKITUP_MAIN_H
#define	_KICKITUP_MAIN_H

#include "video/Surface.h"
#include "Music.h"
#include <stdio.h>
#include <algorithm>
#include "GameMode.h"

using std::max;
using std::min;

extern Music    gMusicIntro;
extern Music    gMusicSong;

#define	NORMAL		0
#define	PRESS		1

#define	GAMETITLE	0
#define	MODESELECT	1
#define	SELECTSONG	2
#define	STAGE1		3
#define	DOUBLEST    4
#define	COUPLE		5
#define DEAD		6
#define CONFIG		7
#define	RESULT		8
#define	GAMEOVER	9

#define	MODE_EASY	0
#define	MODE_HARD	1
#define	MODE_DOUBLE	2
#define	MODE_CRAZY  3
#define	MODE_REMIX  4

#define END			99

#define	true		1
#define false		0

#define MAX_DATA	2048

#define NONE	0
#define PERFECT	1
#define GREAT	2
#define GOOD	3
#define BAD		4
#define	MISS	5

#define ARROW_X	50

#define PUMP_SPRITE 50
#define PUMP_SPRITE_Y 55
#define PUMP_SPRITE_NEW	60

#define LP1_X		30
#define LP7_X		30+PUMP_SPRITE
#define LP5_X		32+PUMP_SPRITE*2
#define LP9_X		35+PUMP_SPRITE*3
#define	LP3_X		35+PUMP_SPRITE*4

#define LP1_X1		350
#define LP7_X1		350+PUMP_SPRITE
#define LP5_X1		352+PUMP_SPRITE*2
#define LP9_X1		355+PUMP_SPRITE*3
#define	LP3_X1		355+PUMP_SPRITE*4

#define	JUDGE_SIZE_X	250
#define	JUDGE_SIZE_Y	50

struct st_HighSpeed {
	int		step;
	int		step1;
	int		step3;
	int		step5;
	int		step7;
	int		step9;

public:
	void reset(const int speed = 1) {
		step = speed;
		step1 = speed;
		step3 = speed;
		step5 = speed;
		step7 = speed;
		step9 = speed;
	}

	int getMax() const {
		int ret = step;
		ret = max(ret, step1);
		ret = max(ret, step3);
		ret = max(ret, step5);
		ret = max(ret, step7);
		ret = max(ret, step9);
		return ret;
	}

	int getMin() const {
		int ret = step;
		ret = min(ret, step1);
		ret = min(ret, step3);
		ret = min(ret, step5);
		ret = min(ret, step7);
		ret = min(ret, step9);
		return ret;
	}

	void setRandom()  {
		step1 = 1+rand() % 8;
		step3 = 1+rand() % 8;
		step5 = 1+rand() % 8;
		step7 = 1+rand() % 8;
		step9 = 1+rand() % 8;
	}
};

extern Surface gSelectBack;
extern Surface gScreen;

extern	bool	SongFlag;

extern	GameMode	gMode[2];
extern	st_HighSpeed	gSpeed[2];

extern	int		MaxSpeed;
extern	int		MinSpeed;

extern	int		Gauge1p;
extern	int		Gauge2p;

extern long		Score1p;
extern long		Score2p;

extern	Uint32	Combo1p;
extern	Uint32	Combo2p;
extern	Uint32	MaxCombo1p;
extern	Uint32	MaxCombo2p;

extern	char	Judgement1p;
extern	char	Judgement2p;
extern	Uint32	dwState;
extern	Uint32	dwState2;
extern	bool	Start1p;
extern	bool	Start2p;
extern	Uint32	cMaxCombo1p;

extern	Uint32	cPerfect1p;
extern	Uint32	cGreat1p;
extern	Uint32	cGood1p;
extern	Uint32	cBad1p;
extern	Uint32	cMiss1p;

extern	Uint32	cPerfect2p;
extern	Uint32	cGreat2p;
extern	Uint32	cGood2p;
extern	Uint32	cBad2p;
extern	Uint32	cMiss2p;
extern	Uint32	cMaxCombo2p;

extern char				g_ProgramState;

extern	char			Data[MAX_DATA+1][14];
extern	char			Data_Judge[MAX_DATA+1][14];
extern	double			Data_y[MAX_DATA+1];

extern	char			Data1[MAX_DATA+1][14];
extern	char			Data_Judge1[MAX_DATA+1][14];
extern	double			Data_y1[MAX_DATA+1];

extern char				Data_Double[MAX_DATA+1][14];
extern char				Data_Double_Judge[MAX_DATA+1][14];
extern double			Data_Double_y[MAX_DATA+1];//족보 데이터의 Y값을 가지고 있는 배열

extern	char				ArrowState1p[10];
extern	char				ArrowState2p[10];
extern	char				ArrowState_Joy[10];


extern	Uint32				PressedKey1p[10];
extern	Uint32				PressedKey2p[10];
extern	Uint32				PressedKey_Joy[10];

extern	int				ALPHA;
extern	int				inc;
extern	

void			DebugPrintf(char *str,...);

void			DisplayStageCount(const int count);
void			GameOver1(void);
void			DrawArrow1p(Uint32 cur);
void			DrawArrow2p(Uint32 cur);
void			DrawJudge1p(void);
void			DrawJudge2p(void);
void			DrawScore1p(void);
void			DrawScore2p(void);
void			DisplayMessage(int x, int y, const char *message);
bool			ClpBlt(int x ,int y ,Surface & surface, const SDL_Rect & srect);
void			StageTitle(void);
void			UpdateFrame(void);
bool			KLoadImage(void);

void			ERRCHECK(FMOD_RESULT result);

#endif // _MAIN_H
