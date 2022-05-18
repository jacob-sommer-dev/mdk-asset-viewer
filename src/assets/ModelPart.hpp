#ifndef __MODELPART_H__
#define __MODELPART_H__

#include <GL/glew.h>
#include <vector>
#include <string>

#include "Model.hpp"
#include "../ecs/Renderable.hpp"
#include "../ecs/Movable.hpp"
#include "../ecs/Rotatable.hpp"
#include "../ecs/Scalable.hpp"

class ModelPart : Renderable, Movable, Rotatable, Scalable
{

private:
    std::string name;
    GLuint* textures;
    //Model* parent;

public:
    ModelPart();
    ~ModelPart();

    // Renderable
    void draw(glm::mat4 *projMat, glm::mat4 *viewMat) override;
    // Movable
    void setPosition(float x, float y, float z) override;
	void translate(float x, float y, float z) override;
    // Rotatable
    void setRotation(float radians, float x, float y, float z) override;
	void rotate(float radians, float x, float y, float z) override;
    // Scalable
    void setScale(float x, float y, float z) override;
	void scaleBy(float x, float y, float z) override;

};

#endif // __MODELPART_H__