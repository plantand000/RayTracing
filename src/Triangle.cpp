#include "Triangle.h"
#include <iostream>
Triangle::Triangle()
{
}

Triangle::~Triangle()
{
}

Triangle::Triangle(glm::vec3 P0, glm::vec3 P1, glm::vec3 P2, glm::vec3 N0, glm::vec3 N1, glm::vec3 N2, glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, glm::vec3 km, float n)
{
    this->P0 = P0;    
    this->P1 = P1;
    this->P2 = P2;
    this->N0 = N0;
    this->N1 = N1;
    this->N2 = N2;
    this->ka = ka;
    this->kd = kd;
    this->ks = ks;
    this->km = km;
    this->n = n;
}   

float Triangle::Intersect(Ray ray, float t0, float t1) {
    //std::cout << this->P0.x << " " << this->P1.x << " " << this->P2.x << std::endl;
    
    //implement moller trombore
    glm::vec3 D = normalize(ray.direction);
    glm::vec3 E1 = this->P1 - this->P0;
    glm::vec3 E2 = this->P2 - this->P0;
    glm::vec3 S = ray.origin - this->P0;
    glm::vec3 S1 = cross(D, E2);
    float a = dot(E1, S1);
    if (a > -0.000001 && a < 0.000001) {
        return -1;
    }
    float f = 1.0f / a;
    float u = f * dot(S, S1);
    if (u < 0.0f || u > 1.0f) {
        return -1.0;
    }
    glm::vec3 S2 = cross(S, E1);
    float v = f * dot(D, S2);
    if (v < 0.0f || u + v > 1.0f) {
        return -1.0;
    }
    float t = f * dot(E2, S2);
    if (t >= t0 && t <= t1) {
        this->alpha = 1.0f - u - v;
        this->beta = u;
        this->gamma = v;
        return t;
    } else {
        return -1.0f;
    }
    // glm::vec3 mtSolution = (1.0f / dot(S1, E1)) * (glm::vec3(dot(S2, E2), dot(S1, S), dot(S2, D)));
    // float t = mtSolution.x;
    // if (t < t0 || t > t1) {
    //     return -1.0f;
    // }
    // this->beta = mtSolution.y;
    // this->gamma = mtSolution.z;
    // this->alpha = 1.0f - beta - gamma;

    // if (this->alpha >= 0.0f && this->alpha <= 1.0f && this->beta >= 0.0f && this->beta <= 1.0f && this->gamma >= 0 && this->gamma <= 1.0f) {
    //     if (t >= t0 && t <= t1) {
    //         return t;
    //     } else {
    //         return -1.0f;
    //     }
    // } else {
    //     return -1.0f;
    // }
}

glm::vec3 Triangle::GetNormal(glm::vec3 p) {
    glm::vec3 normal = (this->alpha * this->N0) + (this->beta * this->N1) + (this->gamma * this->N2);
    return normal;
}