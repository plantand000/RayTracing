#include "BVH.h"
#include <iostream>
#include <algorithm>
BVH::BVH()
{
}

BVH::~BVH()
{
}

BVH::BVH(std::vector<Triangle*> triangles)
{
    this->triangles = triangles;
    this->left = NULL;
    this->right = NULL;
    // compute the bounding box for this
    float xmin = std::numeric_limits<float>::infinity();
    float xmax = -std::numeric_limits<float>::infinity();
    float ymin = std::numeric_limits<float>::infinity();
    float ymax = -std::numeric_limits<float>::infinity();
    float zmin = std::numeric_limits<float>::infinity();
    float zmax = -std::numeric_limits<float>::infinity();
    for (int i = 0; i < triangles.size(); i++) {
        //check for xmin
        if (triangles[i]->P0.x < xmin) {
            xmin = triangles[i]->P0.x;
        }
        if (triangles[i]->P1.x < xmin) {
            xmin = triangles[i]->P1.x;
        }
        if (triangles[i]->P2.x < xmin) {
            xmin = triangles[i]->P2.x;
        }
        // check xmax
        if (triangles[i]->P0.x > xmax) {
            xmax = triangles[i]->P0.x;
        }
        if (triangles[i]->P1.x > xmax) {
            xmax = triangles[i]->P1.x;
        }
        if (triangles[i]->P2.x > xmax) {
            xmax = triangles[i]->P2.x;
        }

        // check for ymin
        if (triangles[i]->P0.y < ymin) {
            ymin = triangles[i]->P0.y;
        }
        if (triangles[i]->P1.y < ymin) {
            ymin = triangles[i]->P1.y;
        }
        if (triangles[i]->P2.y < ymin) {
            ymin = triangles[i]->P2.y;
        }
        // check ymax
        if (triangles[i]->P0.y > ymax) {
            ymax = triangles[i]->P0.y;
        }
        if (triangles[i]->P1.y > ymax) {
            ymax = triangles[i]->P1.y;
        }
        if (triangles[i]->P2.x > xmax) {
            ymax = triangles[i]->P2.y;
        }

        // check for zmin
        if (triangles[i]->P0.z < zmin) {
            zmin = triangles[i]->P0.z;
        }
        if (triangles[i]->P1.z < zmin) {
            zmin = triangles[i]->P1.z;
        }
        if (triangles[i]->P2.z < zmin) {
            zmin = triangles[i]->P2.z;
        }
        // check zmax
        if (triangles[i]->P0.z > zmax) {
            zmax = triangles[i]->P0.z;
        }
        if (triangles[i]->P1.z > zmax) {
            zmax = triangles[i]->P1.z;
        }
        if (triangles[i]->P2.z > zmax) {
            zmax = triangles[i]->P2.z;
        }
    }
    this->xmin = xmin;    
    this->xmax = xmax;
    this->ymin = ymin;
    this->ymax = ymax;
    this->zmin = zmin;
    this->zmax = zmax;

}   


float BVH::Intersect(Ray ray, float t0, float t1) {
    ray.direction = normalize(ray.direction);
    float tmin = (this->xmin - ray.origin.x) / ray.direction.x;
    float tmax = (this->xmax - ray.origin.x) / ray.direction.x;

    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (this->ymin - ray.origin.y) / ray.direction.y;
    float tymax = (this->ymax - ray.origin.y) / ray.direction.y;

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax)) {
        return -1.0f;
    }

    if (tymin > tmin) {
        tmin = tymin; 
    }
    if (tymax < tmax) { 
        tmax = tymax; 
    }

    float tzmin = (this->zmin - ray.origin.z) / ray.direction.z;
    float tzmax = (this->zmax - ray.origin.z) / ray.direction.z;

    if (tzmin > tzmax) std::swap(tzmin, tzmax); 

    if ((tmin > tzmax) || (tzmin > tmax)) {
        return -1.0f; 
    }
    if (tzmin > tmin) {
        tmin = tzmin; 
    }
    if (tzmax < tmax) {
        tmax = tzmax; 
    }

    if (tmin > tmax) { // ray does not hit
        return -1.0f;
    } else {
        if (tmax < t0) {
            return -1.0f;
        }
        if (tmin < t0 || (tmin > t0 && tmax > t0)) { // intersects the box
            if (this->isLeaf()) {
                float minT = std::numeric_limits<float>::infinity();
                int mini = -1;
                for (int i = 0; i < triangles.size(); i++) {
                    float t = triangles[i]->Intersect(ray,t0,t1);
                    if (t >= t0) {
                        if (t < minT) {
                            minT = t;
                            mini = i;
                        }
                    }
                }
                if (mini >= 0) {
                    this->ka = triangles[mini]->ka;
                    this->kd = triangles[mini]->kd;
                    this->ks = triangles[mini]->ks;
                    this->km = triangles[mini]->km;
                    this->n = triangles[mini]->n;
                    this->normal = triangles[mini]->GetNormal(glm::vec3(0.0f,0.0f,0.0f));
                    return minT;
                }
                return -1.0f;
            } else {
                float hit1 = -1.0f;
                float hit2 = -1.0f;
                if (this->left != NULL) {
                    hit1 = this->left->Intersect(ray, t0, t1);
                    
                }
                if (this->right != NULL) {
                    hit2 = this->right->Intersect(ray, t0, t1);
                }

                if (hit1 >= t0 && hit2 >= t0) {
                    if (hit1 <= hit2) {
                        this->ka = this->left->ka;
                        this->kd = this->left->kd;
                        this->ks = this->left->ks;
                        this->km = this->left->km;
                        this->n = this->left->n;
                        this->normal = this->left->GetNormal(glm::vec3(0.0f,0.0f,0.0f));
                        return hit1;
                    } else {
                        this->ka = this->right->ka;
                        this->kd = this->right->kd;
                        this->ks = this->right->ks;
                        this->km = this->right->km;
                        this->n = this->right->n;
                        this->normal = this->right->GetNormal(glm::vec3(0.0f,0.0f,0.0f));
                        return hit2;
                    }
                } else if (hit1 >= t0) {
                    this->ka = this->left->ka;
                    this->kd = this->left->kd;
                    this->ks = this->left->ks;
                    this->km = this->left->km;
                    this->n = this->left->n;
                    this->normal = this->left->GetNormal(glm::vec3(0.0f,0.0f,0.0f));
                    return hit1;
                } else if (hit2 >= t0) {
                    this->ka = this->right->ka;
                    this->kd = this->right->kd;
                    this->ks = this->right->ks;
                    this->km = this->right->km;
                    this->n = this->right->n;
                    this->normal = this->right->GetNormal(glm::vec3(0.0f,0.0f,0.0f));
                    return hit2;
                } else {
                    return -1.0f;
                }
            }
        }
        return -1.0f;
    }

}

glm::vec3 BVH::GetNormal(glm::vec3 p) {
    return this->normal;
}