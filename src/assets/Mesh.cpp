#include "Mesh.hpp"

#include "Texture.hpp"
#include "Brush.hpp"

Mesh::Mesh(void* data, u_int* idx, u_int numMats, GLuint shader)
{
    this->shader = shader;

    // mesh name
    char cname[13]{0};
    memcpy(cname, (char*)data+*idx, 12);
    this->name.assign(cname);
    *idx += 12;// * sizeof(char);

    // origin
    this->origin.x = *((float*)((char*)data+*idx));
    this->origin.y = *((float*)((char*)data+*idx + sizeof(float)));
    this->origin.z = *((float*)((char*)data+*idx + sizeof(float)*2));
    *idx += 3 * sizeof(float);

    // num verts
    num_verts = *((int*)((char*)data+*idx));
    *idx += sizeof(int);

    // vertex positions
    vertices = (Vertex*)malloc(num_verts * sizeof(Vertex));

    for(int i = 0; i < num_verts; i++)
    {
        vertices[i].position.x = *((float*)((char*)data+*idx));
        vertices[i].position.y = *((float*)((char*)data+*idx + sizeof(float)));
        vertices[i].position.z = *((float*)((char*)data+*idx + sizeof(float)*2));

        *idx += 3 * sizeof(float);
    }

    // num tris
    num_tris = *((int*)((char*)data+*idx));
    *idx += sizeof(int);

    // element and material index arrays
    elements = (Element*)malloc(num_tris * sizeof(Element));

    for(int i = 0; i < num_tris; i++)
    {
        // the vertex indices
        elements[i].v1 = *(u_short*)((char*)data+*idx);
        elements[i].v2 = *(u_short*)((char*)data+*idx + sizeof(u_short));
        elements[i].v3 = *(u_short*)((char*)data+*idx + sizeof(u_short)*2);
        
        // increment index
        *idx += sizeof(u_short) * 3;

        // the material index
        elements[i].mat_idx = *(u_short*)((char*)data+*idx);
        *idx += sizeof(u_short);

        // absolute u,v coords. Need to fix them to be 0.0-1.0 if the material is a texture
        elements[i].v1u = *((float*)((char*)data+*idx));
        elements[i].v1v = *((float*)((char*)data+*idx + sizeof(float)));
        // vertices[elements[i].v1].texCoords.x = *((float*)((char*)data+*idx));
        // vertices[elements[i].v1].texCoords.y = *((float*)((char*)data+*idx + sizeof(float)));
        *idx += sizeof(float) * 2;
        
        elements[i].v2u = *((float*)((char*)data+*idx));
        elements[i].v2v = *((float*)((char*)data+*idx + sizeof(float)));
        // vertices[elements[i].v2].texCoords.x = *((float*)((char*)data+*idx));
        // vertices[elements[i].v2].texCoords.y = *((float*)((char*)data+*idx + sizeof(float)));
        *idx += sizeof(float) * 2;
        
        elements[i].v3u = *((float*)((char*)data+*idx));
        elements[i].v3v = *((float*)((char*)data+*idx + sizeof(float)));
        // vertices[elements[i].v3].texCoords.x = *((float*)((char*)data+*idx));
        // vertices[elements[i].v3].texCoords.y = *((float*)((char*)data+*idx + sizeof(float)));
        *idx += sizeof(float) * 2;

        // next 4 bytes are always 0 as far as I can tell... skip for now
        *idx += 4;
    }

    // bounds
    bounds.v1.x =  *((float*)((char*)data+*idx));
    bounds.v1.y = *((float*)((char*)data+*idx + sizeof(float)));
    bounds.v1.z = *((float*)((char*)data+*idx + sizeof(float)*2));
    *idx += 3 * sizeof(float);

    bounds.v2.x =  *((float*)((char*)data+*idx));
    bounds.v2.y = *((float*)((char*)data+*idx + sizeof(float)));
    bounds.v2.z = *((float*)((char*)data+*idx + sizeof(float)*2));
    *idx += 3 * sizeof(float);
}

Mesh::~Mesh()
{
    if(ready)
    {
        // gl objects, std buffers should be freed already
        glDeleteBuffers(numBufs, EBOs);
        glDeleteBuffers(numBufs, VBOs);
        glDeleteVertexArrays(numBufs, VAOs);
        free(ebo_sizes);
        free(VAOs);
        free(VBOs);
        free(EBOs);
    }
    else
    {
        // std buffers, just free
        free(vertices);
        free(elements);
    }
}

void Mesh::load(std::vector<Material*>* mats)
{
    
    if(!ready)
    {

        std::size_t num_mats = mats->size();

        numBufs = num_mats;
        VAOs = (GLuint*)malloc(sizeof(GLuint) * numBufs);
        VBOs = (GLuint*)malloc(sizeof(GLuint) * numBufs);
        EBOs = (GLuint*)malloc(sizeof(GLuint) * numBufs);
        glGenVertexArrays(num_mats, VAOs);
        glGenBuffers(num_mats, VBOs);
        glGenBuffers(num_mats, EBOs);
        ebo_sizes = (std::size_t*)malloc(sizeof(std::size_t) * numBufs);

        std::vector<u_short> vtx_idx_s;

        for(u_short i = 0; i < num_mats; i++)
        {
            Material* mat = (*mats)[i];
            

            for(int j = 0; j < num_tris; j++)
            {
                if(elements[j].mat_idx == i)
                {
                    // add vertex indices
                    vtx_idx_s.push_back(elements[j].v1);
                    vtx_idx_s.push_back(elements[j].v2);
                    vtx_idx_s.push_back(elements[j].v3);

                    if(mat->type == BRUSH) // set the color for the vertex
                    {
                        vertices[elements[j].v1].color = ((Brush*)mat)->color;
                    }
                    // else // or fix the texture u,v indices to be 0.0 - 1.0
                    // {
                        // vertices[elements[j].v1].texCoords.x = elements[j].v1u / (float)((Texture*)mat)->w;
                        // vertices[elements[j].v1].texCoords.y = elements[j].v1v / (float)((Texture*)mat)->h;
                        // vertices[elements[j].v2].texCoords.x = elements[j].v2u / (float)((Texture*)mat)->w;
                        // vertices[elements[j].v2].texCoords.y = elements[j].v2v / (float)((Texture*)mat)->h;
                        // vertices[elements[j].v3].texCoords.x = elements[j].v3u / (float)((Texture*)mat)->w;
                        // vertices[elements[j].v3].texCoords.y = elements[j].v3v / (float)((Texture*)mat)->h;
                    // }
                }
            }

            if(mat->type != BRUSH) // fix the texture u,v indices to be 0.0 - 1.0
            {
                // fix the verts here so we don't try to "fix" them more than once
                for(int k = 0; k < num_verts; k++)
                {
                    float u = 0.0;
                    float v = 0.0;
                    for(int l = 0; l < num_tris; l++)
                    {
                        // find the first element with the u,v we want
                        if(elements[l].v1 == k)
                        {
                            u = elements[l].v1u;
                            v = elements[l].v1v;
                            break;
                        }
                        else if(elements[l].v2 == k)
                        {
                            u = elements[l].v2u;
                            v = elements[l].v2v;
                            break;
                        }
                        else if(elements[l].v3 == k)
                        {
                            u = elements[l].v3u;
                            v = elements[l].v3v;
                            break;
                        }
                    }

                    vertices[k].texCoords.x = u / (float)((Texture*)mat)->w;
                    vertices[k].texCoords.y = v / (float)((Texture*)mat)->h; 
                    // needed this for flipped textures, may no need it anymore
                    //vertices[k].texCoords.y = ((float)((Texture*)mat)->h - v) / (float)((Texture*)mat)->h;
                }
            }



            // everything should be "fixed" so now
            // build GL buffers
            glBindVertexArray(VAOs[i]);
            // always include ALL of the vertices in the VBO
            glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
            glBufferData(GL_ARRAY_BUFFER, num_verts * sizeof(Vertex), vertices, GL_STATIC_DRAW);
            // include built vtx idx vector as EBO, remember to save size for draw
            ebo_sizes[i] = vtx_idx_s.size();
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[i]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, vtx_idx_s.size() * sizeof(unsigned short), &vtx_idx_s[0], GL_STATIC_DRAW);

            // set vertex attribute pointers
            // positions
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            // texture coordinates
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
            // color for brush
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

            glUseProgram(shader);
            glUniform1i(glGetUniformLocation(shader, "texture0"), 0);
            glUniform1i(glGetUniformLocation(shader, "type"), (GLint)mat->type);

            // unbind for next loop
            glBindVertexArray(0);

            // reset for next mat index
            vtx_idx_s.clear();

        }

        // destroy all the temp data buffers
        free(vertices);
        free(elements);

        // we're now ready to draw
        ready = true;
    }
}

void Mesh::draw(glm::mat4 *projMat, glm::mat4 *viewMat, std::vector<Material*>* mats)
{

    // calculate model matrix
	glm::mat4 modelMat = glm::scale(glm::mat4(1.0), scale);
	modelMat *= rotation;
	modelMat = glm::translate(modelMat, position);// + origin);
    
    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr((*viewMat)));
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr((*projMat)));


    // loop through the VAOs
    for(int i = 0; i < numBufs; i++)
    {
        Material* mat = (*mats)[i];
        
        glUniform1i(glGetUniformLocation(shader, "type"), (GLint)mat->type);

        if(mat->type != BRUSH)
        {
            // find and bind the correct texture (if mat is texture)
            Texture* tex = (Texture*)mat;

            glActiveTexture(GL_TEXTURE0);
            glUniform1i(glGetUniformLocation(shader, "texture0"), 0);
            glBindTexture(GL_TEXTURE_2D, tex->gltex);
            
        }
        
        // draw elements
        glBindVertexArray(VAOs[i]);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(ebo_sizes[i]), GL_UNSIGNED_SHORT, 0);
        

        if(mat->type != BRUSH)
        {
            // deactivate
            glActiveTexture(GL_TEXTURE0);
        }
        
        // unbind
        glBindVertexArray(0);


    }

        
}