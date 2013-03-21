#ifndef _KICKITUP_CONTEXT_H
#define _KICKITUP_CONTEXT_H

/// @file	Context.h
/// @brief	state pattern.

#include "Stage.h"

class KickItUp;

class Context
{
public:
	static const int	MAX_PLAY_CNT	=	4;	// max count to play

private:
	KickItUp &	m_kickItUp;
	int			m_curPlayCnt;			///< 연속한 게임 횟수. 여기서는 4회를넘기면 게임이 끝난다.

	Stage *		m_pStageTitle;
	Stage *		m_pStageSelect;
	Stage *		m_pStageNormal;
	Stage *		m_pStageDouble;
	Stage *		m_pStageResult;
	Stage *		m_pStageGameOver;

	Stage *		m_curentStage;

public:
	Context( KickItUp & kickItUp );
	~Context(void);

	KickItUp &	GetKickItUp() const	{	return m_kickItUp;	}

	virtual bool	Process( unsigned long delta );
	virtual bool	Initialize();
	virtual void	Destroy();

	// Get Set member function.
	void		SetState( Stage * state );
	Stage *		GetState() const			{	return m_curentStage;	}

	int			GetStageCnt() const			{	return m_curPlayCnt;		}
	void		SetStageCnt( const int curPlayCnt )	{	m_curPlayCnt = curPlayCnt;	}

	Stage *		GetStateTitle() const		{	return m_pStageTitle;	}
	Stage *		GetStateSelect() const		{	return m_pStageSelect;	}
	Stage *		GetStateNormal() const		{	return m_pStageNormal;	}
	Stage *		GetStateDouble() const		{	return m_pStageDouble;	}
	Stage *		GetStateResult() const		{	return m_pStageResult;	}
	Stage *		GetStateGameOver() const	{	return m_pStageGameOver;	}
};

#endif // _KICKITUP_CONTEXT_H
