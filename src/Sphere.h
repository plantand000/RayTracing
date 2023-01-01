#pragma once
#include "Shape.h"


class Sphere :
	public Shape
{
public:
	Sphere();
	~Sphere();
	
	Sphere(glm::vec3 position, float radius, glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, glm::vec3 km, float n);
	float Intersect(Ray ray, float t0, float t1);
	glm::vec3 GetNormal(glm::vec3 p);
	
	glm::vec3 position;
	float radius;
};
