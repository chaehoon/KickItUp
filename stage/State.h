/*
 * State.h
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#ifndef STATE_H_
#define STATE_H_

class State {
public:
	State(void)	{}
	virtual ~State(void)	{}

	virtual void GoToPrevStage()		{}	///< goto prev stage.
	virtual void GoToNextStage()		{}	///< goto next stage.
};

#endif /* STATE_H_ */
