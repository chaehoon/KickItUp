#include "SurfaceManager.h"

SurfaceManager::SurfaceManager(void)
{
}

SurfaceManager::~SurfaceManager(void)
{
	Release();
}

Surface *	SurfaceManager::MakeSurface()
{
	Surface *	pSurface = new Surface();
	m_Surfacelist.push_back( pSurface );
	return pSurface;
}


void SurfaceManager::Release()
{
	for( iter i = m_Surfacelist.begin() ; i != m_Surfacelist.end() ; ++i ) {
		Surface * pSurface = *i;
		delete pSurface;
	}
	m_Surfacelist.clear();
}
