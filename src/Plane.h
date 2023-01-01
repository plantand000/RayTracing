#pragma once
#include "Shape.h"

class Plane :
	public Shape
{
public:
	Plane();
	~Plane();

	Plane(glm::vec3 center, glm::vec3 normal, glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, glm::vec3 km, float n);
	float Intersect(Ray ray, float t0, float t1);
	glm::vec3 GetNormal(glm::vec3 p);

	glm::vec3 center;
	glm::vec3 normal;
};
