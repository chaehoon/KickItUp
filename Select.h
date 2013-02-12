#ifndef	_KICKITUP_SELECT_H
#define	_KICKITUP_SELECT_H

#ifdef _WIN32
#	pragma warning(disable:4996)
#endif // _WIN32

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

void SelectSong(void);
void Read(void);
int	ScanHiddenMode1p(void);
int	ScanHiddenMode2p(void);
void DrawMode(int x, int y, int Mode);

#endif	// _SELECT_H
