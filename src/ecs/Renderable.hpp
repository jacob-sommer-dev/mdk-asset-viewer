#ifndef __RENDERABLE_H__
#define __RENDERABLE_H__

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Renderable 
{

private:
    

public:
    bool dodraw = true;

    virtual void draw(glm::mat4 *projMat, glm::mat4 *viewMat) {};

};

#endif // !__RENDERABLE_H__