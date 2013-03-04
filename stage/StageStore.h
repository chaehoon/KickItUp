/*
 * StageStore.h
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#ifndef STAGESTORE_H_
#define STAGESTORE_H_

class Stage;

class StageStore {
	public:
	static const int	MAX_PLAY_CNT	=	4;	// max count to play

private:
	int			_curPlayCnt;			///< 연속한 게임 횟수. 여기서는 4회를넘기면 게임이 끝난다.

	Stage *		_pStageTitle;
	Stage *		_pStageSelect;
	Stage *		_pStageNormal;
	Stage *		_pStageDouble;
	Stage *		_pStageResult;
	Stage *		_pStageGameOver;

	Stage *		_curentStage;

public:
	StageStore();
	virtual ~StageStore(void);

	virtual bool	Process( unsigned long delta );
	virtual bool	Initialize();
	virtual void	Destroy();

	// Get Set member function.
	void		SetState( Stage * state );
	Stage *		GetState() const			{	return _curentStage;	}

	int			GetStageCnt() const			{	return _curPlayCnt;		}
	void		SetStageCnt( const int curPlayCnt )	{	_curPlayCnt = curPlayCnt;	}

	Stage *		GetStateTitle() const		{	return _pStageTitle;	}
	Stage *		GetStateSelect() const		{	return _pStageSelect;	}
	Stage *		GetStateNormal() const		{	return _pStageNormal;	}
	Stage *		GetStateDouble() const		{	return _pStageDouble;	}
	Stage *		GetStateResult() const		{	return _pStageResult;	}
	Stage *		GetStateGameOver() const	{	return _pStageGameOver;	}
};

#endif /* STAGESTORE_H_ */
