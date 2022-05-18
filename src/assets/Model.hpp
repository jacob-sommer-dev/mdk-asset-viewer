#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <GL/glew.h>
#include <vector>
#include <map>
#include <string>

#include "Animation.hpp"
#include "ModelPart.hpp"
#include "../ecs/Renderable.hpp"
#include "../ecs/Movable.hpp"
#include "../ecs/Rotatable.hpp"
#include "../ecs/Scalable.hpp"

class Model : Renderable, Movable, Rotatable, Scalable
{

private:
    std::string name;
    std::vector<ModelPart> children;
    std::map<std::string, Animation> anims;

public:
    Model();
    ~Model();

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

#endif