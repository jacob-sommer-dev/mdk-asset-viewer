#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "../ecs/Renderable.hpp"

class Widget : public Renderable
{
    GLuint texture;
    GLuint vbo;
    GLuint ebo;
    GLuint vao;
    GLuint shaderProg;

private:


public:
    Widget(GLuint, u_short, u_short, const u_int *, const u_int *, GLuint shader);
    ~Widget();

    // Renderable
    void draw(glm::mat4 *projMat, glm::mat4 *viewMat);


};


#endif // __WIDGET_H__