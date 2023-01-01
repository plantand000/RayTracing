#include "Sphere.h"
#include <iostream>
Sphere::Sphere()
{
}

Sphere::~Sphere()
{
}

Sphere::Sphere(glm::vec3 position, float radius, glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, glm::vec3 km, float n)
{
    this->position = position;    
    this->radius = radius;
    this->ka = ka;
    this->kd = kd;
    this->ks = ks;
    this->km = km;
    this->n = n;
}   

float Sphere::Intersect(Ray ray, float t0, float t1) {
    glm::vec3 o = ray.origin;
    glm::vec3 d = ray.direction;
    glm::vec3 C = this->position;
    float R = this->radius;
    float a = dot(d, d);
    float b = dot(2.0f * (o - C), d);
    float c = dot((o - C), (o - C)) - pow(R, 2);
    float v = pow(b,2) - (4*a*c);
    if (v > (float)0.0) { // two possible solutions
        float t_neg = ((-b - pow(v, (float)0.5)) / 2*a);
        float t_pos = ((-b + pow(v, (float)0.5)) / 2*a);
        float minT = fmin(t_neg,t_pos);
        if (minT >= t0 && minT <= t1) {
            return minT;
        } else {
            return -1.0f;
        }
    } else if (v == (float)0.0) { // one possible solution
        float t = ((-b) / (2*a));
        if (t >= t0 && t < t1) {
            return t;
        } else {
            return -1.0f;
        }
    } else { // no solutions
        return -1.0f;
    }
}

glm::vec3 Sphere::GetNormal(glm::vec3 p) {
    return normalize(p - this->position);
}