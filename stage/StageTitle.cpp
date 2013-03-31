#include "StageTitle.h"
#include "../KickItup.h"
#include "../Video/Surface.h"
#include "../Sound/Sound.h"
#include "../Video/SurfaceStore.h"
#include "../Sound/SoundStore.h"
#include "../Input/InputManager.h"
#include "Context.h"
#include "../GameConfig.h"

StageTitle::StageTitle( Context & context )	: m_context( context )
{
}

StageTitle::~StageTitle(void)
{
}


bool StageTitle::Initialize()
{
	// Loading image here
	m_pGameTitle = g_pSurfaceStore->Order( "title" );
	assert( m_pGameTitle );
	
	if( m_pGameTitle->Load( "images/title.bmp" ) == false ) {
		assert( 0 );
		return false;
	}

    // "FREE PLAY!", "PRESS CENTER BUTTON" image.
	m_pStateComment = g_pSurfaceStore->Order( "c_font" );
	m_pStateComment->Load( "images/c_font.bmp" );
	m_pStateComment->SetColorKey();

    // Opening music.
	m_pOpening = g_pSoundStore->Order( "opening" );
	m_pOpening->Load( "wave/opening.wav" );

	return true;
}

void StageTitle::Destroy()
{
}

bool StageTitle::Render( unsigned long delta )
{
	m_pGameTitle->Blit( 0, 0 );

	// Draw to screen "FREE PLAY!"
	SDL_Rect	sRect;
	sRect.x = 0;
	sRect.y = 46;
	sRect.w = 220;
	sRect.h = 69-46;
	m_pStateComment->SetAlpha( 255 );
	m_pStateComment->Blit( 210, 450, &sRect );

	// Draw to screen "PRESS CENTER BUTTON"
	if( g_GameConfig.IsStarted( eP_1 ) == false ) {
		sRect.x = 0;
		sRect.y = 0;
		sRect.w = 220;
		sRect.h = 23;

		m_pStateComment->SetAlpha( g_pSurfaceStore->GetFlashAlphaValue() );
		m_pStateComment->Blit( 10, 450, &sRect );
	}

	// Draw to screen "PRESS CENTER BUTTON"
	if( g_GameConfig.IsStarted( eP_2 ) == false ) {
		sRect.x = 0;
		sRect.y = 0;
		sRect.w = 220;
		sRect.h = 23;

		m_pStateComment->SetAlpha( g_pSurfaceStore->GetFlashAlphaValue() );
		m_pStateComment->Blit( 410, 450, &sRect );
	}

	return true;
}

// Ÿ��Ʋ �������� ����.
void StageTitle::GetIn()
{
	// �÷��̾� 1p, 2p ���.
	g_GameConfig.SetStart( eP_1, false );
	g_GameConfig.SetStart( eP_2, false );

	g_Input.RegisterObserver( "StageTitle", this );
	
	// ������ �Ҹ��� �޾Ƹ� ġ�°��� �����ϱ� ���� ���� �ڷ�.
	m_pOpening->Play( true );
}

// Ÿ��Ʋ �������� ����.
void StageTitle::GetOut()
{
	// ������ �Ҹ��� �޾Ƹ� ġ�°��� �����ϱ� ���� ���� ������.
	m_pOpening->Stop();

	g_Input.DeleteObserver( "StageTitle" );

}

// Ű���� �Է� ó��.
bool StageTitle::InputHandleMessage( const eInputName name, const eInputState is )
{
	bool	ret = true;
	
	if( is != eIS_PRESS )
		return false;

	switch( name )
	{
	case eIN_BACK:
		// �÷��̾ ���ۻ����̸� ���ۻ��� ���
		if( g_GameConfig.IsStarted( eP_1) || 
			g_GameConfig.IsStarted( eP_2) )	{ 
			// �÷��̾� 1p, 2p ���.
			g_GameConfig.SetStart( eP_1, false );
			g_GameConfig.SetStart( eP_2, false );
		}
		// �÷��̾ ���ۻ��°� �ƴϸ� ���α׷� ����.
		else {
			GoPreStage();
		}
		break;

	case eIN_ARROW_1P_MIDLE:
		// 1�÷��̾� ����.
		if( g_GameConfig.IsStarted( eP_1 ) )
			GoNextStage();
		else
			g_GameConfig.SetStart( eP_1 );
		break;

	case eIN_ARROW_2P_MIDLE:
		// 1�÷��̾� ����.
		if( g_GameConfig.IsStarted( eP_2 ) )
			GoNextStage();
		else
			g_GameConfig.SetStart( eP_2 );
		break;

	default:
		ret = false;
		break;
	}
	return ret;
}

// ���� ���� ���������� �̵�.
void StageTitle::GoNextStage()
{
	m_context.SetState( m_context.GetStateSelect() );
}

// ������������ �̵�( ���α׷� ���� )
void StageTitle::GoPreStage()
{
	m_context.GetKickItUp().SetQuit( true );
}
