#pragma once
#include "Shape.h"


class Triangle :
	public Shape
{
public:
	Triangle();
	~Triangle();
	
	Triangle(glm::vec3 P0, glm::vec3 P1, glm::vec3 P2, glm::vec3 N0, glm::vec3 N1, glm::vec3 N2, glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, glm::vec3 km, float n);
	float Intersect(Ray ray, float t0, float t1);
	glm::vec3 GetNormal(glm::vec3 p);
	
	glm::vec3 P0;
	glm::vec3 P1;
    glm::vec3 P2;
    glm::vec3 N0;
    glm::vec3 N1;
    glm::vec3 N2;

    float alpha;
    float beta;
    float gamma;

	glm::vec3 average;
};