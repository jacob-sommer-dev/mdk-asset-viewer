#ifndef __SCALABLE_H__
#define __SCALABLE_H__

#include <glm/vec3.hpp>

/**
* I don't know if Entity Component System is used this far down
* May want to refactor after more functionality is implemented
*/
class Scalable 
{

protected:

	glm::vec3 scale = glm::vec3(1.0f);

public:
	virtual void setScale(float x, float y, float z)
	{
		scale = glm::vec3(x, y, z);
	}

	virtual void scaleBy(float x, float y, float z)
	{
		scale *= glm::vec3(x, y, z);
	}
	
};

#endif// !__SCALABLE_H__