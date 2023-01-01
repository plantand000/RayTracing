#pragma once
#include <vector>
#include "Scene.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Ray.h"

class Camera
{
public:
	Camera();
	~Camera();

	Camera(int widthRes, int heightRes, glm::vec3 eye, glm::vec3 lookat, glm::vec3 up, float fovY, float focalDistance);

	void TakePicture(Scene *scene);
	float* GetRenderedImage() { return renderedImage; };

	glm::vec3 ComputeRayColor(Ray ray, float t0, float t1, Scene *scene, int count);

private:

	int widthRes;
	int heightRes;
	
	float *renderedImage;

	glm::vec3 eye;
	glm::vec3 lookat;
	glm::vec3 up;
	float fovY;
	float focalDistance;


};
