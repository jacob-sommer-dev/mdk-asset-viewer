#ifndef __MOVABLE_H__
#define __MOVABLE_H__

#include <glm/vec3.hpp>

/**
* I don't know if Entity Component System is used this far down
* May want to refactor after more functionality is implemented
*/
class Movable 
{

protected:

	glm::vec3 position = glm::vec3(0.0f);

public:
	virtual void setPosition(float x, float y, float z)
	{
		position = glm::vec3(x, y, z);
	}

	virtual void translate(float x, float y, float z)
	{
		position += glm::vec3(x, y, z);
	}
	
};

#endif // !__MOVABLE_H__