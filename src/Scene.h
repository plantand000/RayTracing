#pragma once
#include <vector>
#include "Shape.h"
#include "Light.h"


class Scene
{
public:
	Scene();
	~Scene();
	Scene(std::vector<Shape*> shapes, std::vector<Light*> lights);

	float Hit(Ray ray, float t0, float t1, Shape *&rec);
	
	std::vector<Shape*> shapes;
	std::vector<Light*> lights;

};
