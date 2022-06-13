#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "../ecs/Renderable.hpp"

class Widget : public Renderable
{
    GLuint texture;
    GLuint bufs[2]{0};
    GLuint vao;
    GLuint shaderProg;

private:


public:
    Widget(GLuint, u_short, u_short, const u_int *, const u_int *, GLuint);
    ~Widget();

    // Renderable
    void draw(glm::mat4 *projMat, glm::mat4 *viewMat) override;


};


#endif // __WIDGET_H__