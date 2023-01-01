#pragma once
#include "Shape.h"
#include "Triangle.h"
#include <vector>

class BVH :
	public Shape
{
public:
	BVH();
	~BVH();
	
	BVH(std::vector<Triangle*> triangles);
	float Intersect(Ray ray, float t0, float t1);
	glm::vec3 GetNormal(glm::vec3 p);
    bool isLeaf() {return this->left == NULL && this->right == NULL;}
	BVH* left;
    BVH* right;

    std::vector<Triangle*> triangles;
    float xmin;
    float xmax;
    float ymin;
    float ymax;
    float zmin;
    float zmax;
    glm::vec3 normal;
};
