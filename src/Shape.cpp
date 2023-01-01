#include "Shape.h"

Shape::Shape(void)
{
    this->ka = glm::vec3(0.0,0.0,0.0);
    this->kd = glm::vec3(0.0,0.0,0.0);
    this->ks = glm::vec3(0.0,0.0,0.0);
    this->km = glm::vec3(0.0,0.0,0.0);
    this->n = 0.0;
}

Shape::~Shape(void)
{
    
}

