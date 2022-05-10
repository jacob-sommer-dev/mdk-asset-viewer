#ifndef __MOVABLE_H__
#define __MOVABLE_H__

/**
* I don't know if Entity Component System is used this far down
* May want to refactor after more functionality is implemented
*/
class Movable 
{

public:
	virtual void setPosition(float x, float y, float z) = 0;
	virtual void translate(float x, float y, float z) = 0;
	
};

#endif // !__MOVABLE_H__