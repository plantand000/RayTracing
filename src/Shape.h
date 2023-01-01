#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Ray.h"
#include <limits>
class Shape
{
public:
	Shape(void);
	~Shape(void);
	virtual float Intersect(Ray ray, float t0, float t1){return -1;};
	virtual glm::vec3 GetNormal(glm::vec3 p){return glm::vec3(0.0,0.0,0.0);};

	glm::vec3 ka;
	glm::vec3 kd;
	glm::vec3 ks;
	glm::vec3 km;
	float n;
};
