#include "KickItUp.h"

#include "sound/FmodSoundStore.h"
#include "video/SDLSurfaceStore.h"
#include "video/Surface.h"
#include "util/Timer.h"
#include "stage/Context.h"
#include "input/InputManager.h"
#include "data/SongMgr.h"

KickItUp::KickItUp(void)	:   m_bQuit( false )
{
}

KickItUp::~KickItUp(void)
{
}

// Initialize kickitup.
bool KickItUp::Initialize()
{
	// SDL 초기화
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 || !SDL_GetVideoInfo() )
		return false;

	// initialize Surface.
	g_pSurfaceStore = new SDLSurfaceStore();
	if( !g_pSurfaceStore->Initialize() )
		return false;

	// Initialize Sound.
	g_pSoundStore = new FmodSoundStore();
	if( !g_pSoundStore->Initialize() )
		return false;

	// Initialize Input.
	g_Input.Initialize();

	// TODO: load config.

    // Load Song Data
    _loadImage();
    _loadSound();

    g_SongMgr.Load( "song" );

	// Initialize Stage.
	m_pContext = new Context( *this );
	if( !m_pContext->Initialize() )
		return false;

	return true;
}

// main loop.
bool KickItUp::Run()
{
	Timer fps;
	fps.Start();

	m_bQuit = false;
	while( !m_bQuit ) {
		SDL_Event	event;
		
		while( SDL_PollEvent( &event ) ) {
			switch( event.type ) {
				case SDL_QUIT:
					m_bQuit = true;
					break;
			}
		}

		// 100 fps
		static const unsigned long fps_100 = (1000/100);
		if( fps.GetTicks() < fps_100 ) {
			SDL_Delay( fps_100 - fps.GetTicks() );
		}

		Process( fps.GetTicks() );

		fps.Start();
	}
	return true;
}

// end.
void KickItUp::Destroy()
{
	m_pContext->Destroy();
	delete m_pContext;
	m_pContext = 0;

	// TODO: save config.

	// TODO: Input destory

	// Destroy Sound.
	g_pSoundStore->Destroy();
	delete g_pSoundStore;
	g_pSoundStore = 0;

	// Destroy SurfaceStore.
	g_pSurfaceStore->Destroy();
	delete g_pSurfaceStore;
	g_pSurfaceStore = 0;
	
	SDL_Quit();
}

// Display
bool KickItUp::Process( unsigned long delta )
{
	g_Input.Update( delta );
	g_pSoundStore->Process( delta );
	m_pContext->Process( delta );
	return g_pSurfaceStore->Process( delta );
}

bool KickItUp::_loadImage()
{
    // no_disc �̹����� �̸� �޸𸮿� �ε��Ѵ�.
    Surface * pNoDiskImg = g_pSurfaceStore->Order( "no_disc" );
    pNoDiskImg->Load( "images/no_disc.bmp" );
    pNoDiskImg->SetColorKey();

    // back.bmp�� �̸� �޸𸮿� �ε� �Ѵ�.
    Surface * pBack = g_pSurfaceStore->Order( "bgImage" );
    pBack->Load( "images/back.bmp" );
    pBack->SetColorKey();

    return true;
}

bool KickItUp::_loadSound()
{
    return true;
}
