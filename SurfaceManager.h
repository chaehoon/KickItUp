#ifndef _SURFACEMANAGER_H
#define _SURFACEMANAGER_H

#include <vector>
#include "Surface.h"

class SurfaceManager
{
public:
	SurfaceManager(void);
	~SurfaceManager(void);
	void Release();

	Surface *	MakeSurface();

private:
	typedef std::vector<Surface *>				List;
	typedef std::vector<Surface *>::iterator	iter;

	List		m_Surfacelist;
};

#endif // _SURFACEMANAGER_H
