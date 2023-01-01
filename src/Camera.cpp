#include "Camera.h"
#include <iostream>

Camera::Camera()
{
}

Camera::~Camera()
{
}

Camera::Camera(int widthRes, int heightRes, glm::vec3 eye, glm::vec3 lookat, glm::vec3 up, float fovY, float focalDistance)
{
	this->widthRes = widthRes;
	this->heightRes = heightRes;
	renderedImage = new float[widthRes * heightRes * 3];
	this->eye = eye;
	this->lookat = lookat;
	this->up = up;
	this->fovY = fovY;
	this->focalDistance = focalDistance;
}

glm::vec3 Camera::ComputeRayColor(Ray ray, float t0, float t1, Scene *scene, int count) {
	Shape* rec = new Shape();
	Shape* srec = new Shape();
	float t = scene->Hit(ray, t0, t1, rec);
	if (t > 0) {
		glm::vec3 p = ray.origin + t * ray.direction;
		glm::vec3 N = rec->GetNormal(p); // can be a plane or a sphere or a triangle
		glm::vec3 c = rec->ka;
		glm::vec3 E = normalize(this->eye - p);
		for (int i = 0; i < scene->lights.size(); i++) {
			// calculate shadow ray
			glm::vec3 lightDir = normalize(scene->lights[i]->position - p);
			Ray sray(p, lightDir);
			float t_light = dot(scene->lights[i]->position - sray.origin, sray.direction);
			if (scene->Hit(sray, 0.000001f, t_light, srec) < t0) 
			{
				glm::vec3 L_i = normalize(scene->lights[i]->position - p);
				glm::vec3 R_i = normalize((2*dot(L_i,N)) * N - L_i);
				glm::vec3 i_diffuse = rec->kd * fmax((float)0.0, dot(L_i, N)); 
				glm::vec3 i_spec = rec->ks * pow(fmax((float)0.0, dot(R_i,E)), rec->n);
				c += (scene->lights[i]->color * (i_diffuse + i_spec));
			}
		}
		
		if (count < 4 && rec->km.x > 0.0f && rec->km.y > 0.0f && rec->km.z > 0.0f) {
			glm::vec3 V = normalize(ray.origin - p);
			glm::vec3 R = 2 * dot(V, N) * N - V;
			Ray rray(p, R);
			c += rec->km * ComputeRayColor(rray, 0.000001f, std::numeric_limits<float>::infinity(), scene, count+1);
		}

		return c;
	} else {
		return glm::vec3(0.0,0.0,0.0); // background color
	}
}


void Camera::TakePicture(Scene *scene)
{	
	memset(renderedImage, 0, sizeof(float) * widthRes * heightRes * 3);
	glm::vec3 a = this->eye - this->lookat;
	glm::vec3 b = this->up;

	glm::vec3 w = normalize(a);
	glm::vec3 u = normalize(cross(b, w));
	glm::vec3 v = cross(w, u);

	float ly = 2 * this->focalDistance * tan(this->fovY / 2);
	float lx = ly * ((float)this->widthRes / this->heightRes);
	float Pw = ly / this->heightRes;

	glm::vec3 viewdir = normalize(this->lookat - this->eye);
	glm::vec3 Ic = this->eye + viewdir*this->focalDistance;

	glm::vec3 O = Ic - ((lx / 2) * u) - ((ly / 2)* v);
	for (int i = 0; i < widthRes; i++) {
		for (int j = 0; j < heightRes; j++) {
			// compute primary ray
			glm::vec3 Pc = O + (Pw * (((float)(i + 0.5))) * u) + (Pw * (((float)(j + 0.5))) * v);
			glm::vec3 direction = normalize(Pc - this->eye);
			glm::vec3 origin = this->eye;
			
			Ray primary(origin, direction);
			//get the ray color
			glm::vec3 RayColor = this->ComputeRayColor(primary, 0, std::numeric_limits<float>::infinity(), scene, 0);
			this->renderedImage[(i + j * this->widthRes) * 3 + 0] = RayColor.x;
			this->renderedImage[(i + j * this->widthRes) * 3 + 1] = RayColor.y;
			this->renderedImage[(i + j * this->widthRes) * 3 + 2] = RayColor.z;
		}
	}
}


