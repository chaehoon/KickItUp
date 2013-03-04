/*
 * FmodSoundStore.h
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#ifndef FMODSOUNDSTORE_H_
#define FMODSOUNDSTORE_H_

#include <fmodex/fmod.h>
#include "SoundStore.h"

class FmodSoundStore	: public SoundStore
{
private:
	FMOD_SYSTEM *	m_system;

public:
	FmodSoundStore(void);
	virtual ~FmodSoundStore(void);

	virtual bool	Process( unsigned long );
	virtual bool	Initialize();
	virtual void	Destroy();

protected:
	virtual Sound *	create();


private:
	bool _isErr(const FMOD_RESULT result);
};
#endif /* FMODSOUNDSTORE_H_ */
