#include "Shader.hpp"

#include <cstdio>
#include <cstdlib>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#ifdef _MSC_VER
#pragma warning(disable:4996) // allow fopen() portable function without warnings
#endif // _MSC_VER

/** Gets the file's length.
 *
 * @param file the file
 *
 * @return size_t the file's length in bytes
 */
static size_t getFileLength(FILE* file) {
	size_t length;
	size_t currentPos = ftell(file);

	fseek(file, 0, SEEK_END);
	length = ftell(file);

	// return file seek to initial position
	fseek(file, currentPos, SEEK_SET);

	return length;
}

/** Loads and compiles a shader from a file.
 *
 * This will print any errors to the console.
 *
 * @param filename the shader's filename
 * @param shaderType the shader type (e.g., GL_VERTEX_SHADER)
 *
 * @return GLuint the shader's ID, or 0 if failed
 */
static GLuint loadShader(const char* filename, GLenum shaderType) {
	FILE* file = fopen(filename, "r");
	if (!file) {
		SDL_Log("Can't open file: %s\n", filename);
		return 0;
	}

	size_t length = getFileLength(file);

	// allocate space for shader data plus null terminator
	GLchar* shaderSrc = (GLchar*)calloc(length + 1, 1);
	if (!shaderSrc) {
		SDL_Log("Couldn't allocate memory for shader: %s\n", filename);
		fclose(file);
		file = NULL;
		return 0;
	}

	fread(shaderSrc, 1, length, file);

	fclose(file);
	file = NULL;

	// create shader and free src buffer
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, (const GLchar**)&shaderSrc, NULL);
	
	free(shaderSrc);
	shaderSrc = NULL;

	glCompileShader(shader);
	GLint success = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		SDL_Log("Couldn't compile shader %s...\n", filename);
		GLint logLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		GLchar* errLog = (GLchar*)malloc(logLen);
		if (errLog) {
			glGetShaderInfoLog(shader, logLen, &logLen, errLog);
			SDL_Log("%s\n", errLog);
			free(errLog);
		}
		else {
			SDL_Log("Couldn't get shader log, out of memory\n");
		}

		glDeleteShader(shader);
		shader = 0;
	}

	return shader;
}

/** Delete a shader
* 
*/
static void destroyShader(GLint shaderID) {
	glDeleteShader(shaderID);
}

GLuint loadShaderProgram(const char* vertFilename, const char* fragFilename) {
	GLuint vertShader = loadShader(vertFilename, GL_VERTEX_SHADER);
	if (!vertShader) {
		SDL_Log("Couldn't load shader %s\n", vertFilename);
		return 0;
	}

	GLuint fragShader = loadShader(fragFilename, GL_FRAGMENT_SHADER);
	if (!fragShader) {
		SDL_Log("Couldn't load shader %s\n", fragFilename);

		//don't forget to clear the vertex shader
		destroyShader(vertShader);
		vertShader = 0;

		return 0;
	}

	GLuint shaderProgram = glCreateProgram();
	if (shaderProgram) {
		glAttachShader(shaderProgram, vertShader);
		glAttachShader(shaderProgram, fragShader);

		glLinkProgram(shaderProgram);

		GLint linkSuccess = GL_FALSE;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkSuccess);
		if (!linkSuccess) {
			SDL_Log("Shader linking not sucessful (Vert: %s, Frag: %s)\n", vertFilename, fragFilename);
			GLint logLen = 0;
			glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLen);
			GLchar* errLog = (GLchar*)malloc(logLen);
			if (errLog) {
				glGetProgramInfoLog(shaderProgram, logLen, &logLen, errLog);
				SDL_Log("%s\n", errLog);
			}
			else {
				SDL_Log("Couldn't get shader link log; out of memory\n");
			}
			glDeleteProgram(shaderProgram);
			shaderProgram = 0;
		}
	}
	else {
		SDL_Log("Couldn't create shader program\n");
	}

	// clean up shaders now that we have a program
	destroyShader(vertShader);
	destroyShader(fragShader);

	return shaderProgram;
}

void destroyShaderProgram(GLuint shaderProg) {
	glDeleteProgram(shaderProg);
}