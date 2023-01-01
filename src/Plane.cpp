#include "Plane.h"


Plane::Plane()
{
}

Plane::~Plane()
{
}

Plane::Plane(glm::vec3 center, glm::vec3 normal, glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, glm::vec3 km, float n) 
{
    this->center = center;    
    this->normal = normal;
    this->ka = ka;
    this->kd = kd;
    this->ks = ks;
    this->km = km;
    this->n = n;
}

float Plane::Intersect(Ray ray, float t0, float t1) {
    glm::vec3 o = ray.origin;
    glm::vec3 d = ray.direction;
    glm::vec3 p = this->center;
    glm::vec3 N = this->normal;
    float t = dot((p - o), N) / (dot(d, N));
    if (t >= t0 && t < t1) {
        return t;
    } else {
        return -1;
    }
}

glm::vec3 Plane::GetNormal(glm::vec3 p) {
    return normalize(this->normal);
}

