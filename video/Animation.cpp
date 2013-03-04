/*
 * Animation.cpp
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#include "Animation.h"
#include "Surface.h"

Animation::Animation()  :
    _ticks( 0 ),
    _currentFrame( 0 ),
    _frameInc( 1 ),
    _frameRate( 0 ),   // Milliseconds
    _oldTime( 0 ),
    _maxFrames( 0 ),
    _oscillate( false ),
    _bPause( false ),
    _pSurface( 0 ),
    _coord()
{
}

Animation::~Animation()
{
}

void Animation::OnAnimate( const unsigned long delta )
{
    if( _bPause )
        return;

    _ticks += delta;

    if( _ticks < _oldTime + _frameRate ) {
        return;
    }

    _oldTime = _ticks;

    _currentFrame += _frameInc;

    // Animation을 계속할지 판단.
    if( _bLoop == false ) {
        if( _maxFrames - 1 <= _currentFrame ) {
            return;
        }
    }


    if( _oscillate ) {
        if( 0 < _frameInc ) {
            if( _maxFrames - 1 <= _currentFrame ) {
                _frameInc = -_frameInc;
            }
        }
        else {
            if( _currentFrame <= 0 ) {
                _frameInc = -_frameInc;
            }
        }
    }
    else {
        if( _maxFrames - 1 <= _currentFrame ) {
            _currentFrame = 0;
        }
    }
}


void Animation::setCurrentFrame( const int frame )
{
    if( frame < 0 || _maxFrames <= frame )
        return;
    _currentFrame = frame;
}

// Animation 프레임이 끝났는지 판단.
bool Animation::isLoopEnd() const
{
    return _currentFrame == _maxFrames;
}

void Animation::OnRender()
{
    SDL_Rect    rt;
    rt.x = _rect.w * getCurrentFrame();
    rt.y = _rect.y;
    rt.w = _rect.w;
    rt.h = _rect.h;
    // m_pSurface->Blit( m_coord.getX(), m_coord.getY(), &rt );
}
