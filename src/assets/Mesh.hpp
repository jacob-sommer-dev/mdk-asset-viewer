#ifndef __MESH_H__
#define __MESH_H__

#include <GL/glew.h>
#include <vector>
#include <string>

#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Material.hpp"
#include "../ecs/Renderable.hpp"
#include "../ecs/Movable.hpp"
#include "../ecs/Rotatable.hpp"
#include "../ecs/Scalable.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec2 texCoords;
    glm::vec4 color;
    // glm::vec3 Normal;
};

struct Element {
    u_short v1;
    u_short v2;
    u_short v3;
    u_short mat_idx;
    float v1u;
    float v1v;
    float v2u;
    float v2v;
    float v3u;
    float v3v;
};

struct Bounds {
    glm::vec3 v1;
    glm::vec3 v2;
};

class Mesh : /*Renderable,*/ Movable, Rotatable, Scalable
{

private:
    std::string name;
    
    glm::vec3 origin;

    u_int num_verts = 0;
    Vertex* vertices = nullptr;

    u_int num_tris = 0;
    Element* elements = nullptr;

    Bounds bounds;

    std::size_t numBufs = 0;
    GLuint* VAOs = nullptr;
    GLuint* VBOs = nullptr;
    GLuint* EBOs = nullptr;
    std::size_t* ebo_sizes = nullptr;

    GLuint shader = 0;

    bool ready = false;

public:
    Mesh(void* data, u_int* start, u_int numMats, GLuint shader);
    ~Mesh();

    void load(std::vector<Material*>*);

    // Renderable
    //void draw(glm::mat4 *projMat, glm::mat4 *viewMat) override;
    void draw(glm::mat4 *projMat, glm::mat4 *viewMat, std::vector<Material*>* mats);

};

#endif // __MESH_H__