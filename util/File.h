/*
 * File.h
 *
 *  Created on: 2013. 3. 28.
 *      Author: chaehoon-win
 */

#ifndef FILE_H_
#define FILE_H_

#include <string>
using std::string;

class File {
public:
	static	bool GetRealFileName(const string & filename, string * realname);
	static	bool Access( const string & filename );
};

#endif /* FILE_H_ */
