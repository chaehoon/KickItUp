/*
 * StageStore.cpp
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#include "StageStore.h"
#include "../common/common.h"
#include "Stage.h"

StageStore::StageStore() : _curPlayCnt( 4 ),
	_pStageTitle( 0 ),
	_pStageSelect( 0 ),
	_pStageNormal( 0 ),
	_pStageDouble( 0 ),
	_pStageResult( 0 ),
	_pStageGameOver( 0 ),
	_curentStage( 0 )
{
}

StageStore::~StageStore(void)
{
}


bool	StageStore::Initialize()
{
//	m_pStageTitle = new StageTitle( *this );
//	if( !m_pStageTitle->Initialize() )
//		return false;
//
//	m_pStageSelect = new StageSelect( *this );
//	if( !m_pStageSelect->Initialize() )
//		return false;
//
//    m_pStageNormal = new StageNormal( *this );
//    if(
//        !m_pStageNormal->Initialize() )
//        return false;
    /*
	m_pStageDouble = new StateDouble( *this );
	m_pStageResult = new StateResult( *this );
	m_pStageGameOver = new StateGameOver( *this );
	m_curentStage = m_pStageTitle;
	*/

	SetState( _pStageTitle );
	return true;
}
bool	StageStore::Process( unsigned long delta )
{
	return _curentStage->Process( delta );
}

void	StageStore::SetState( Stage * stage )
{
	assert( stage != 0 );

	if( _curentStage )
		_curentStage->GetOut();
	_curentStage = stage;
	_curentStage->GetIn();
}

void	StageStore::Destroy()
{
	_pStageTitle->Destroy();
	delete _pStageTitle;
}
