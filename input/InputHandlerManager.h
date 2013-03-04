/*
 * InputHandlerManager.h
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#ifndef INPUTHANDLERMANAGER_H_
#define INPUTHANDLERMANAGER_H_

#include <map>
#include <string>
#include "InputEnum.h"

class iInputHandler;

class InputHandlerManager {
private:
	typedef std::map<std::string, iInputHandler *>	InputHandlerList;
	typedef InputHandlerList::iterator	InputHandlerListIter;

private:
	InputHandlerList		m_inputHandlerList;

public:
	InputHandlerManager(void);
	~InputHandlerManager(void);

	// 옵저버 패턴
	bool	SendMessage( const eInputName name, const eInputState state );
	void	RegisterObserver( const char * name, iInputHandler * pInputHandler );
	void	DeleteObserver( const char * name );
};

#endif /* INPUTHANDLERMANAGER_H_ */
