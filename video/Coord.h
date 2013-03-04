/*
 * Coord.h
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#ifndef COORD_H_
#define COORD_H_

class Coord {
public:
    Coord( short x = 0, short y = 0 )   :   _x( x ), _y( y )	{
    }

    Coord( const Coord & rhs )  :   _x( rhs._x ), _y( rhs._y )	{
    }

    ~Coord(void)	{
    }

    const Coord &   operator= ( const Coord & rhs )	{
        this->_x = rhs._x;
        this->_y = rhs._y;
        return *this;
    }

    void    setX( const short & x )     {   this->_x = x;    }
    short   getX() const                {   return this->_x; }
    void    setY( const short & y )     {   this->_y = y;    }
    short   getY() const                {   return this->_y; }

private:
    short   _x;
    short   _y;

};

#endif /* COORD_H_ */
