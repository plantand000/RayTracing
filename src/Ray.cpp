#include "Ray.h"

Ray::Ray() 
{
    this->origin = glm::vec3(0.0,0.0,0.0);
    this->direction = glm::vec3(0.0,0.0,0.0);
}

Ray::~Ray() 
{

}

Ray::Ray(glm::vec3 origin, glm::vec3 direction) 
{
    this->origin = origin;
    this->direction = direction;
}