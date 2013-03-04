/*
 * InputManager.h
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#ifndef INPUTMANAGER_H_
#define INPUTMANAGER_H_

#include <map>
#include "InputHandlerManager.h"

class iInputDevice;

class InputManager {
public:
	struct sRegisteredKey {
		int				keyKind;
		int				inputState;
		bool			separate;

		bool	isSameKeyKind( int kk )	{	return kk == keyKind;	}
	};

	typedef std::map<eInputName, sRegisteredKey>	RegisterKeyList;
	typedef RegisterKeyList::iterator		KeyIter;

private:
	iInputDevice *	m_pKeyBoard;
	RegisterKeyList	m_registeredKeyList;
	InputHandlerManager	m_inputHandlermanager;

public:
	InputManager(void);
	~InputManager(void);

	bool Initialize();
	void Update( long delta );

	void RegisterObserver( const char * name, iInputHandler * pInputHandler );	///< 키입력값이 필요한 옵저버를 등록
	void DeleteObserver( const char * name );									///< 키입력값이 필요한 옵저버를 등록해제

private:
	bool _createDevice();
	void _registerKey();
	void _registerKey( eInputName name, int kind );
	void _sendKeyMessage( eInputName name, sRegisteredKey & key );
};

extern	InputManager	g_Input;
#endif /* INPUTMANAGER_H_ */
