#include "Scene.h"
#include <iostream>
Scene::~Scene()
{
}

Scene::Scene()
{
	
}

Scene::Scene(std::vector<Shape*> shapes, std::vector<Light*> lights)
{
    this->shapes = shapes;
    this->lights = lights;
}

float Scene::Hit(Ray ray, float t0, float t1, Shape *& rec) {
    float minT = std::numeric_limits<float>::infinity();
    int mi = 0;
    for (int i = 0; i < shapes.size(); i++) {
        float t = shapes[i]->Intersect(ray, t0, t1);
        if (t >= 0 && t < minT) {
            minT = t;
            mi = i;
        }
    }
    
    if (minT >= t0 && minT < t1) {
        rec = shapes[mi];
        return minT;
    }
    return -1.0f;
}
