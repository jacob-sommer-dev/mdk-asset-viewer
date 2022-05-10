#ifndef __ROTATABLE_H__
#define __ROTATABLE_H__

/**
* I don't know if Entity Component System is used this far down
* May want to refactor after more functionality is implemented
*/
class Rotatable 
{

public:
	virtual void setRotation(float radians, float x, float y, float z) = 0;
	virtual void rotate(float radians, float x, float y, float z) = 0;

};

#endif// !__ROTATABLE_H__