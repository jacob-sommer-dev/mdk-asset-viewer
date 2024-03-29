#include "Widget.hpp"


Widget::Widget(GLuint texture, unsigned short w, unsigned short h, const unsigned int *disp_w, const unsigned int *disp_h, GLuint shader)
{
    this->texture = texture;

    // find x and y via the ratio for 2d screen space
    float x = ((float)w / (float)*disp_w);
    float y = ((float)h / (float)*disp_h);

    // now set up verts, u,v
    float verts[] = 
    {
         x, -y, 0.0f,   1.0f, 1.0f,
         x,  y, 0.0f,   1.0f, 0.0f,
        -x, -y, 0.0f,   0.0f, 1.0f,
        -x,  y, 0.0f,   0.0f, 0.0f
    };
    unsigned int indices[] =
    {
        0, 1, 2,
        1, 3, 2
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(2, bufs);

    glBindBuffer(GL_ARRAY_BUFFER, bufs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufs[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    shaderProg = shader;
    glUseProgram(shaderProg);
    glUniform1i(glGetUniformLocation(shaderProg, "ourTexture"), 0);
}

Widget::~Widget()
{
    glDeleteBuffers(2, bufs);
    glDeleteVertexArrays(1, &vao);
}

void Widget::draw(glm::mat4 *projMat, glm::mat4 *viewMat)
{
    if(dodraw)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glUseProgram(shaderProg);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }
}

