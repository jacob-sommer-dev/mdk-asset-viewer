#ifndef __ROTATABLE_H__
#define __ROTATABLE_H__

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
* I don't know if Entity Component System is used this far down
* May want to refactor after more functionality is implemented
*/
class Rotatable 
{

protected:

	glm::mat4 rotation = glm::mat4(1.0f);

public:
	virtual void setRotation(float radians, float x, float y, float z)
	{
		glm::vec3 norm = glm::normalize(glm::vec3(x, y, z));
		rotation = glm::rotate(glm::mat4(1.0), radians, norm);
	}

	virtual void rotate(float radians, float x, float y, float z)
	{
		glm::vec3 norm = glm::normalize(glm::vec3(x, y, z));
		rotation = glm::rotate(rotation, radians, norm);
	}

};

#endif// !__ROTATABLE_H__