#include "Camera.hpp"

Camera::Camera(float vfov, float aspect) 
{
	projMatrix = glm::perspective(glm::radians(vfov), aspect, 1.0f, 1000.f);
}

Camera::~Camera() 
{
	// 
}

glm::mat4* Camera::update() 
{
    if(accel != glm::vec3(0.0f, 0.0f, 0.0f))
    {
        velocity += accel;

        accel *= 0.5f;
        if(glm::abs(accel.x) < 0.1f) { accel.x = 0.0f; }
        if(glm::abs(accel.y) < 0.1f) { accel.y = 0.0f; }
        if(glm::abs(accel.z) < 0.1f) { accel.z = 0.0f; }

        velocity *= 0.5f;
        if(glm::abs(velocity.x) < 0.1f) { velocity.x = 0.0f; }
        if(glm::abs(velocity.y) < 0.1f) { velocity.y = 0.0f; }
        if(glm::abs(velocity.z) < 0.1f) { velocity.z = 0.0f; }

        position += velocity;

        dirty = true;
    }

	if (dirty) 
    { // dirty, so recalc view matrix

		viewMatrix = glm::translate(rotation, position);

		dirty = false;
	}

    return &viewMatrix;
}

void Camera::setPosition(float x, float y, float z) 
{
	position = glm::vec3(x, y, z);
	dirty = true;
}

glm::vec3* Camera::getPosition()
{
    return &position;
}

void Camera::setRotation(float radians, float x, float y, float z) 
{
	glm::vec3 norm = glm::normalize(glm::vec3(x, y, z));
	rotation = glm::rotate(glm::mat4(1.0), radians, norm);
	dirty = true;
}

void Camera::translate(float x, float y, float z) 
{
	position += glm::vec3(x, y, z);
	dirty = true;
}

void Camera::rotate(float radians, float x, float y, float z) 
{
	glm::vec3 norm = glm::normalize(glm::vec3(x, y, z));
	rotation = glm::rotate(rotation, radians, norm);
	dirty = true;
}

void Camera::handleKeys(const Uint8 *keystates, const float* dt)
{

    float scale = *dt / 0.016666f;
    float ro = rot * scale;

    if(keystates[SDL_SCANCODE_W])
    {
        accel.z += scale;
    }
    else if(keystates[SDL_SCANCODE_S])
    {
        accel.z -= scale;
    }
    else if(keystates[SDL_SCANCODE_A])
    {
        accel.x -= scale;
    }
    else if(keystates[SDL_SCANCODE_D])
    {
        accel.x += scale;
    }
    else if(keystates[SDL_SCANCODE_Q])
    {
        rotate(ro, 0.0f, 1.0f, 0.0f);
    }
    else if(keystates[SDL_SCANCODE_E])
    {
        rotate(ro, 0.0f, -1.0f, 0.0f);
    }
    else if(keystates[SDL_SCANCODE_SPACE])
    {
        accel.y += scale;
    }
    else if(keystates[SDL_SCANCODE_LCTRL])
    {
        accel.y -= scale;
    }
}