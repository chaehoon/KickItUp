#ifndef _KICKITUP_ZOOMANIMATION_H
#define _KICKITUP_ZOOMANIMATION_H

#include "../common.h"
#include "Animation.h"

class ZoomAnimation : public Animation
{
private:
    float   m_minZoom;      ///< 최소 zoom.
    float   m_maxZoom;      ///< 최대 zoom.
    float   m_curZoom;      ///< 현재 Animation중인 이미지 zoom.

public:
    ZoomAnimation( const float minZoom = 1.0f, const float m_maxZoom = 1.0f );
    virtual ~ZoomAnimation(void);

    virtual void    OnAnimate( const unsigned long delta );
    virtual void    OnRender();

    void            setCurZoom( const float curZoom = 1.0f )    {   m_curZoom = curZoom;    }
    void            setZoom( const float minZoom = 1.0f, const float maxZoom = 1.0f );
};

#endif // _KICKITUP_ZOOMANIMATION_H
