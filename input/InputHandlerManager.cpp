/*
 * InputHandlerManager.cpp
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#include "InputHandlerManager.h"

#include "iInputHandler.h"

using std::string;

InputHandlerManager::InputHandlerManager(void)
{
}

InputHandlerManager::~InputHandlerManager(void)
{
}


// 옵저버에 키 입력값을 전달한다.
bool InputHandlerManager::SendMessage( const eInputName name, const eInputState state )
{
	for( InputHandlerListIter h = m_inputHandlerList.begin() ; h != m_inputHandlerList.end() ; ++h ) {
		iInputHandler * pInputhandler = h->second;
		if( pInputhandler->InputHandleMessage( name, state ) )
			return true;
	}

	return false;
}

// 옵저버 등록.
void InputHandlerManager::RegisterObserver( const char * name, iInputHandler * pInputHandler )
{
	m_inputHandlerList.insert( std::make_pair(string(name), pInputHandler) );
}

// 옵저버 삭제.
void InputHandlerManager::DeleteObserver( const char * name )
{
	InputHandlerListIter i = m_inputHandlerList.find( string(name) );
	if( i != m_inputHandlerList.end() )
		m_inputHandlerList.erase( i );
}
