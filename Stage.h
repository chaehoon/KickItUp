#ifndef _STAGE_H
#define _STAGE_H

#include "Surface.h"
#include "SurfaceManager.h"

class Stage
{
private:
	Surface &		m_screen;
	SurfaceManager	m_SurfaceManger;
	bool			m_loaded;

public:
	Stage( Surface & screen );
	virtual ~Stage(void);

	virtual bool	Load() = 0;
	void	Release();

protected:
	Surface *	MakeSurface();
	Surface *	GetScreen();
	bool		IsLoaded() const;
	void		SetLoadState(const bool state = true);
};

#endif // _STAGE_H
