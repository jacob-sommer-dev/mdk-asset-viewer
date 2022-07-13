
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
* The Camera is representative of a point of view in the world.
* It can be moved and rotated, though not via the ECS
* Movable and Rotatable interfaces as we probably don't want a 
* generalized system moving the camera.
* It calculates the view matrix for this PoV for the renderer to use.
*/
class Camera 
{

private:

	float moveSpeed = 5.0f;

	glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projMatrix;// = glm::mat4(1.0f);

	glm::mat4 rotation = glm::mat4(1.0f);
	glm::vec3 position = glm::vec3(0.0f);

	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::vec3(-1.0f, 0.0f, 0.0f);

	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 velocity = glm::vec3(0.0f);
    glm::vec3 accel = glm::vec3(0.0f);

	float yaw = -90.0f;
	float pitch = 0.0f;
	float roll = 0.0f;

    const float mov = 5.0f;
    const float rot = M_PI * 0.2f; // 1/10th of 2 pi

	// so we only calculate when camera has moved (so recompute view matrix)
	// good for now, but need to figure out if this makes sense in the future
	// (a more dynamic camera, threading overhead, etc.)
	bool dirty = false;

public:

	Camera(float vfov, float aspect);
	~Camera();

	glm::mat4* update();

	// NOT from Movable.h, Rotatable.h, and Scalable.h
	// We probably want fine control over cameras, not controlled by a subsystem via ECS
	void setPosition(float x, float y, float z);
    glm::vec3* getPosition();
	void translate(float x, float y, float z);
	void setRotation(float radians, float x, float y, float z);
	void rotate(float radians, float x, float y, float z);
    void handleKeys(const Uint8 *keystates, const float* dt);
	void rotate(float add_pitch, float add_yaw);
	
};

#endif // !__CAMERA_H__