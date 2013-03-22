#include "Animation.h"
#include "Surface.h"

Animation::Animation()  :
    m_ticks( 0 ),
    m_currentFrame( 0 ),
    m_frameInc( 1 ),
    m_frameRate( 0 ),   // Milliseconds
    m_oldTime( 0 ),
    m_maxFrames( 0 ),
    m_oscillate( false ),
    m_bPause( false ),
    m_pSurface( 0 ),
    m_coord()
{
}

Animation::~Animation()
{
}

void Animation::OnAnimate( const unsigned long delta )
{
    if( m_bPause )
        return;

    m_ticks += delta;

    if( m_ticks < m_oldTime + m_frameRate ) {
        return;
    }

    m_oldTime = m_ticks;

    m_currentFrame += m_frameInc;

    // Animation을 계속할지 판단.
    if( m_bLoop == false ) {
        if( m_maxFrames - 1 <= m_currentFrame ) {
            return;
        }
    }


    if( m_oscillate ) {
        if( 0 < m_frameInc ) {
            if( m_maxFrames - 1 <= m_currentFrame ) {
                m_frameInc = -m_frameInc;
            }
        }
        else {
            if( m_currentFrame <= 0 ) {
                m_frameInc = -m_frameInc;
            }
        }
    }
    else {
        if( m_maxFrames - 1 <= m_currentFrame ) {
            m_currentFrame = 0;
        }
    }
}


void Animation::setCurrentFrame( const int frame )
{
    if( frame < 0 || m_maxFrames <= frame )
        return;
    m_currentFrame = frame;
}

// Animation 프레임이 끝났는지 판단.
bool Animation::isLoopEnd() const
{
    return m_currentFrame == m_maxFrames;
}

void Animation::OnRender()
{
    SDL_Rect    rt;
    rt.x = m_rect.w * getCurrentFrame();
    rt.y = m_rect.y;
    rt.w = m_rect.w;
    rt.h = m_rect.h;
    m_pSurface->Blit( m_coord.getX(), m_coord.getY(), &rt );
}