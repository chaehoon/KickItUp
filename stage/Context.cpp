#include <cassert>
#include "Context.h"

#include "StageTitle.h"
#include "StageSelect.h"
#include "StageNormal.h"
/*
#include "StateDouble.h"
#include "StateResult.h"
#include "StateGameOver.h"
*/

Context::Context( KickItUp & kickItUp ) : m_kickItUp( kickItUp ),
	m_curPlayCnt( 4 ),
	m_pStageTitle( 0 ),
	m_pStageSelect( 0 ),
	m_pStageNormal( 0 ),
	m_pStageDouble( 0 ),
	m_pStageResult( 0 ),
	m_pStageGameOver( 0 ),
	m_curentStage( 0 )
{
}

Context::~Context(void)
{
}


bool	Context::Initialize()
{
	m_pStageTitle = new StageTitle( *this );
	if( !m_pStageTitle->Initialize() )
		return false;

	m_pStageSelect = new StageSelect( *this );
	if( !m_pStageSelect->Initialize() )
		return false;

    m_pStageNormal = new StageNormal( *this );
    if( 
        !m_pStageNormal->Initialize() )
        return false;
    /*
	m_pStageDouble = new StateDouble( *this );
	m_pStageResult = new StateResult( *this );
	m_pStageGameOver = new StateGameOver( *this );
	m_curentStage = m_pStageTitle;
	*/

	SetState( m_pStageTitle );
	return true;
}
bool	Context::Process( unsigned long delta )
{
	return m_curentStage->Process( delta );
}

void	Context::SetState( Stage * stage )
{
	assert( stage != 0 );

	if( m_curentStage )
		m_curentStage->GetOut();
	m_curentStage = stage;
	m_curentStage->GetIn();
}

void	Context::Destroy()
{
	m_pStageTitle->Destroy();
	delete m_pStageTitle;
}
