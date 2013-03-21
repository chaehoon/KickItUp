#ifndef _KICKITUP_GAMECONFIG_H
#define _KICKITUP_GAMECONFIG_H

enum	ePlayer
{
	eP_Min,
	eP_1 = eP_Min,
	eP_2,
	eP_Max
};

class GameConfig
{
private:
	bool	m_bStart[eP_Max];		// 1p 2p 시작여부.

public:
	GameConfig(void);
	~GameConfig(void);

	bool	IsStarted( const ePlayer p )	const	{	return m_bStart[p];	}
	void	SetStart( const ePlayer p, const bool bStart = true )	{	m_bStart[p] = bStart;	}
};

extern	GameConfig	g_GameConfig;

#endif // _KICKITUP_GAMECONFIG_H
