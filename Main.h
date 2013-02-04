#ifndef __MAIN_INCLUDED__
#define __MAIN_INCLUDED__

#include <ddraw.h>
#include "media.h"
#include <stdio.h>

#define NORMAL		0
#define	PRESS		1

#define	GAMETITLE	0
#define MODESELECT	1
#define SELECTSONG	2
#define	STAGE1		3
#define	DOUBLE		4
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

#define	TRUE		1
#define FALSE		0

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

extern LPDIRECTDRAW			g_pDD;
extern LPDIRECTDRAWSURFACE	g_pDDSPrimary;
extern LPDIRECTDRAWSURFACE	g_pDDSBack;

extern HWND hWnd;

extern	BOOL	SongFlag;

extern	BOOL	bModeMirror1p;
extern	BOOL	bModeNonstep1p;
extern	BOOL	bModeSynchro;
extern	BOOL	bModeUnion1p;
extern	BOOL	bModeRandom1p;
extern	BOOL	b4dMix1p;
extern	BOOL	bModeVanish1p;
extern	BOOL	bModeCrazy1p;
extern	BOOL	bModeSuddenR1p;
extern	BOOL	bModeRandomS1p;

extern	BOOL	bModeMirror2p;
extern	BOOL	bModeNonstep2p;
extern	BOOL	bModeUnion2p;
extern	BOOL	bModeRandom2p;
extern	BOOL	b4dMix2p;
extern	BOOL	bModeVanish2p;
extern	BOOL	bModeCrazy2p;
extern	BOOL	bModeSuddenR2p;
extern	BOOL	bModeRandomS2p;

extern	int		HighSpeed1p;
extern	int		HighSpeed2p;

extern	int		HighSpeed1p_1;
extern	int		HighSpeed1p_3;
extern	int		HighSpeed1p_5;
extern	int		HighSpeed1p_7;
extern	int		HighSpeed1p_9;

extern	int		HighSpeed2p_1;
extern	int		HighSpeed2p_3;
extern	int		HighSpeed2p_5;
extern	int		HighSpeed2p_7;
extern	int		HighSpeed2p_9;

extern	int		MaxSpeed;
extern	int		MinSpeed;

extern	int		Gauge1p;
extern	int		Gauge2p;

extern long		Score1p;
extern long		Score2p;

extern	DWORD	Combo1p;
extern	DWORD	Combo2p;
extern	DWORD	MaxCombo1p;
extern	DWORD	MaxCombo2p;

extern	char	Judgement1p;
extern	char	Judgement2p;
extern	DWORD	dwState;
extern	DWORD	dwState2;
extern	BOOL	Start1p;
extern	BOOL	Start2p;
extern	DWORD	cMaxCombo1p;

extern	DWORD	cPerfect1p;
extern	DWORD	cGreat1p;
extern	DWORD	cGood1p;
extern	DWORD	cBad1p;
extern	DWORD	cMiss1p;

extern	DWORD	cPerfect2p;
extern	DWORD	cGreat2p;
extern	DWORD	cGood2p;
extern	DWORD	cBad2p;
extern	DWORD	cMiss2p;
extern	DWORD	cMaxCombo2p;

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


extern	DWORD				PressedKey1p[10];
extern	DWORD				PressedKey2p[10];
extern	DWORD				PressedKey_Joy[10];

extern	int				ALPHA;
extern	int				inc;
extern	DWORD	CKey_CFont;

extern	CMedia	*intro;
extern	CMedia	*song;

void			DebugPrintf(char *str,...);

void			DisplayStageCount(DWORD Count);
void			GameOver1(void);
void			DrawArrow1p(DWORD cur);
void			DrawArrow2p(DWORD cur);
void			DrawJudge1p(void);
void			DrawJudge2p(void);
void			DrawScore1p(void);
void			DrawScore2p(void);
void			DisplayMessage(int x, int y, char *message);
HRESULT			ClpBlt(int x ,int y ,LPDIRECTDRAWSURFACE ds,LPRECT srect,DWORD mode);
void			StageTitle(void);
HRESULT			RestoreAll(void);
void			Flipp(void);
void		ReleaseAllObjects(void);
void			UpdateFrame(void);
long FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HRESULT			InitDI(HINSTANCE hInstance);
HRESULT			InitDD(void);
HRESULT			KLoadImage(void);
HRESULT			InitFail(HWND hWnd, HRESULT hRet, LPCTSTR szError,...);
HRESULT			InitWin(HINSTANCE hInstance, DWORD Width, DWORD Height, int nCmdShow);
int PASCAL		WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine,int nCmdShow);
HRESULT TransAlphaImproved(LPDIRECTDRAWSURFACE src, LPDIRECTDRAWSURFACE dest, 
				   LONG lDestX, LONG lDestY, RECT srcRect, WORD ALPHA, DWORD ColorKey, WORD BPP);
#endif