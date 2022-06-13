#include "Model.hpp"
#include "../MDK_Context.hpp"

Model::Model(std::string name, void* data)
{
    this->name = name;

    // data index
    u_int idx = 0;

    // number of materials
    numMats = *((unsigned int*)data);
    idx += sizeof(int);

    // material names
    //matNames[numMats];
    //materials[numMats];
    for(int i = 0; i < numMats; i++)
    {
        char name[17]{0};
        memcpy(name, (char*)data+idx, 16);
        matNames.push_back(new std::string(name));
        idx += 16;
    }

    // number of meshes
    numChildren = *((unsigned int*)((char*)data+idx));
    idx += 4;

    // find appropriate shader
    std::string sname = std::string("Mesh");
    GLuint shader = MDK_Context::get()->assets->findShader(&sname);

    //children[numChildren];
    // meshes construct themselves and increment idx
    for(int i = 0; i < numChildren; i++)
    {
        children.push_back(new Mesh(data, &idx, numMats, shader));
    }
}

Model::~Model()
{
    for(int i = 0; i < numMats; i++)
    {
        delete matNames[i];
    }

    for(int i = 0; i < numChildren; i++)
    {
        children[i]->~Mesh();
    }
}

void Model::load()
{
    MDK_Context* mdk_context = MDK_Context::get();
    // find materials in name order
    for(int i = 0; i < numMats; i++)
    {
        materials.push_back(mdk_context->assets->findMaterial(matNames[i]));
    }

    for(Mesh* m : children)
    {
        m->load(&materials);
    }
    ready = true;
}

void Model::draw(glm::mat4 *projMat, glm::mat4 *viewMat)
{
    if(!ready)
    {
        load();
    }

    for (Mesh* m : children)
    {
        m->draw(projMat, viewMat, &materials);
    }
}

