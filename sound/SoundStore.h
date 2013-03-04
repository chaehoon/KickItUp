/*
 * SoundStore.h
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#ifndef SOUNDSTORE_H_
#define SOUNDSTORE_H_

#include "../common/common.h"
#include <map>
#include <string>

class Sound;

/// @brief	Sound factory(factory pattern).
class SoundStore {
private:
	typedef	std::map<const std::string, Sound *>	SoundList;		///< Sound List Type.
	typedef SoundList::iterator						SoundIter;		///< SoundList Iterator type.

	SoundList		m_SoundList;		///< Sound List.

public:
	virtual ~SoundStore(void);		///< 소멸자.

	virtual bool	Initialize() = 0;	///< 초기화.
	virtual void	Destroy();			///< 정리 작업.

	Sound * Order( const std::string & name );					///< 사운드를 요청.
	Sound *	Find( const std::string & name );					///< 기존 사운드를 찾음.

	virtual bool	Process( unsigned long )	{	return true; }	/// Sound System Update.

protected:
	SoundStore(void);				///< 생성자.
	virtual	Sound *	create() = 0;	/// Sound Create.
};

extern SoundStore *	g_pSoundStore;

#endif /* SOUNDSTORE_H_ */
