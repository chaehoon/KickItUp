/*
 * iInputHandler.h
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#ifndef IINPUTHANDLER_H_
#define IINPUTHANDLER_H_

#include "InputEnum.h"

class iInputHandler
{
public:
	virtual ~iInputHandler(void) {}

	virtual bool	InputHandleMessage( const eInputName name, const eInputState is ) = 0;
};

#endif /* IINPUTHANDLER_H_ */
