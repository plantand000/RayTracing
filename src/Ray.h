#pragma once
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Ray 
{
public:
    Ray();
    ~Ray();

    Ray(glm::vec3 origin, glm::vec3 direction);

    glm::vec3 origin;
    glm::vec3 direction;
};