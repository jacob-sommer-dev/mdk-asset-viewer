#ifndef __SCALABLE_H__
#define __SCALABLE_H__

/**
* I don't know if Entity Component System is used this far down
* May want to refactor after more functionality is implemented
*/
class Scalable 
{

public:
	virtual void setScale(float x, float y, float z) = 0;
	virtual void scaleBy(float x, float y, float z) = 0;
	
};

#endif// !__SCALABLE_H__