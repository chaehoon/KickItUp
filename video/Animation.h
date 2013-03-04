/*
 * Animation.h
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "../common/common.h"

class Surface;
class Animation {
public:
	Animation();
	virtual ~Animation();

	virtual void    OnAnimate( const unsigned long delta );
	virtual void    OnRender();

    void    setFrameRate( const int rate )    {   _frameRate = rate; }
    int     getFrameRate() const  {   return _frameRate; }

    void    setCurrentFrame( const int frame );
    int     getCurrentFrame() const { return _currentFrame;  }

    void    setMaxFrames( const int maxFrame ) {  _maxFrames = maxFrame; }
    int     getMaxFrames() const   {   return _maxFrames; }

    void    setOscillate( const bool oscillate ) { _oscillate = oscillate;   }
    bool    getOscillate() const  {   return _oscillate; }

    void    setLoop( const bool bLoop )   {   _bLoop = bLoop; }
    bool    getLoop() const       {   return _bLoop; }

    void    setSurface( Surface * pSurface )  {   _pSurface = pSurface;  }
    Surface *   getSurface() const  {   return _pSurface;  }

    void    setRect( const SDL_Rect & rect )    {   _rect = rect;  }

    void    setCoord( const Coord & coord )     {   _coord = coord;    }
    void    setCoord( const short x, const short y ) {
        _coord.setX( x );
        _coord.setY( y );
    }
    Coord   getCoord() const                    {   return _coord;     }

    void    setPause( const bool bPause = true )    {   _bPause = bPause;  }
    bool    getPause() const                    {   return _bPause;    }

    bool    isLoopEnd() const;


private:
    unsigned long _ticks;
    int     _currentFrame;
    int     _frameInc;

    int     _frameRate; //Milliseconds
    unsigned long _oldTime;

    int     _maxFrames;
    bool    _oscillate;
    bool    _bLoop;        // 애니메이션을 계속 할것인지 판단.
    bool    _bPause;       // 일시 정지 여부.

    Surface *   _pSurface; // 애니메이션 Surface
    SDL_Rect    _rect;     // 그림의 속성.

    Coord       _coord;    // 화면에 출력할 위치
};

#endif /* ANIMATION_H_ */
