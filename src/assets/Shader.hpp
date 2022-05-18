#ifndef __SHADER_H__
#define __SHADER_H__

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES // enable EXT prototype functions in gl2.h
#endif

#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>
#include <SDL2/SDL.h>

/** Loads a vertex and fragment shader from disk, then compiles and links them
 * into a shader program.
 *
 * This will print any errors to the console.
 *
 * @param vertFilename filename for the vertex shader
 * @param fragFilename the fragment shader's filename.
 *
 * @return GLuint the shader program's ID, or 0 if failed.
 */
GLuint loadShaderProgram(const char* vertFilename, const char* fragFilename);

/** Destroys a shader program.
 */
void destroyShaderProgram(GLuint shaderProg);

#endif // !__SHADER_H__