#ifndef __BRUSH_HPP__
#define __BRUSH_HPP__

#include <glm/vec4.hpp>

#include "Material.hpp"

class Brush : public Material
{

private:
    
    
public:
    glm::vec4 color;

    Brush(unsigned char, unsigned char, unsigned char, unsigned char);

};


#endif // __BRUSH_HPP__