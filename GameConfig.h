#ifndef _KICKITUP_GAMECONFIG_H
#define _KICKITUP_GAMECONFIG_H

#include <algorithm>

using std::max;
using std::min;

enum	ePlayer
{
	eP_Min,
	eP_1 = eP_Min,
	eP_2,
	eP_Max
};

struct stStepSpeed {
	int		step;

public:
	void set(const int speed = 1) {
		step = speed;
	}

	int getMax() const {
		int ret = step;
		return ret;
	}

	int getMin() const {
		int ret = step;
		return ret;
	}

	void setRandom()  {
		step = 1+rand() % 8;
	}
};

class GameConfig
{
private:
	bool		m_bStart[eP_Max];		// 1p 2p
	stStepSpeed	m_StepSpped[eP_Max];

public:
	GameConfig(void);
	~GameConfig(void);

	bool	IsStarted( const ePlayer p )	const	{	return m_bStart[p];	}
	void	SetStart( const ePlayer p, const bool bStart = true )	{	m_bStart[p] = bStart;	}
	void	SetSpeed( const ePlayer p, const int spped )	{	m_StepSpped[p].set(spped);	}

};

extern	GameConfig	g_GameConfig;

#endif // _KICKITUP_GAMECONFIG_H
