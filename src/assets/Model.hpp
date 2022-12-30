#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <GL/glew.h>
#include <map>
#include <string>
#include <vector>

//#include "Animation.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "../ecs/Renderable.hpp"
#include "../ecs/Movable.hpp"
#include "../ecs/Rotatable.hpp"
#include "../ecs/Scalable.hpp"

class Model : public Renderable, public Movable, public Rotatable, public Scalable
{

private:
    std::string name;
    unsigned int numMats;
    std::vector<std::string*> matNames;
    std::vector<Material*> materials;
    unsigned int numChildren;
    std::vector<Mesh*> children;
    //std::map<std::string, Animation*> anims;

    bool ready = false;

public:
    Model(std::string name, void* data, bool single);
    ~Model();

    void load();

    // Renderable
    void draw(glm::mat4 *projMat, glm::mat4 *viewMat) override;

};

#endif