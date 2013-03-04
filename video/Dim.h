/*
 * Dim.h
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#ifndef DIM_H_
#define DIM_H_

class Dim
{
private:
    short   _w;        // 폭
    short   _h;        // 높이

public:
    Dim( short w = 0, short h = 0 )   :   _w( w ), _h( h )	{
    }

    Dim( const Dim & rhs )  :   _w( rhs._w ), _h( rhs._h )	{
    }

    ~Dim(void)	{
    }

    const Dim &   operator= ( const Dim & rhs )	{
        this->_w = rhs._w;
        this->_h = rhs._h;
        return *this;
    }

    void    setW( const short & w )     {   this->_w = w;    }
    short   getW() const                {   return this->_w; }
    void    setH( const short & h )     {   this->_h = h;    }
    short   getH() const                {   return this->_h; }
};

#endif /* DIM_H_ */
