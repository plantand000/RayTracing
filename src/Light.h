#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	~Light();

	Light(glm::vec3 position, glm::vec3 color);

	glm::vec3 position;
	glm::vec3 color;
};
