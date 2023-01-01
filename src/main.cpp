#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <chrono>
#include <iostream>
#include "Camera.h"
#include "Scene.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include "BVH.h"
#include <algorithm>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1200

float frameBuffer[WINDOW_HEIGHT][WINDOW_WIDTH][3];
GLFWwindow *window;

std::vector<Shape*> shapes;
std::vector<Light*> lights;

std::vector<Triangle*> triangles;

void ClearFrameBuffer()
{
	memset(&frameBuffer, 0, WINDOW_HEIGHT * WINDOW_WIDTH * 3 * sizeof(float));
}

void Display()
{	
	glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, frameBuffer);
}

// Create a vector of triangles. Considers the texture coordinates if they are available.
void CreateTriangleVector(std::vector<glm::vec3> &vertices, std::vector<glm::vec3>& normals)
{

	for (int i = 0; i < vertices.size() / 3; i++)
	{	

		Triangle* t = new Triangle(vertices[i * 3 + 0], vertices[i * 3 + 1], vertices[i * 3 + 2], 
					normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2],
					glm::vec3(0.1,0.1,0.1), glm::vec3(0.0,0.0,1.0), glm::vec3(1.0,1.0,0.5), glm::vec3(0.0,0.0,0.0),100.0f);
		//std::cout << t->ka.x << std::endl;

		triangles.push_back(t);
	}

}

void LoadModel(char* name, std::vector<glm::vec3> &vertices, std::vector<glm::vec3>& normals)
{
	// Taken from Shinjiro Sueda with slight modification
	std::string meshName(name);
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
	if (!rc) {
		std::cerr << errStr << std::endl;
	}
	else {
		// Some OBJ files have different indices for vertex positions, normals,
		// and texture coordinates. For example, a cube corner vertex may have
		// three different normals. Here, we are going to duplicate all such
		// vertices.
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					vertices.push_back(glm::vec3(attrib.vertices[3 * idx.vertex_index + 0],
												 attrib.vertices[3 * idx.vertex_index + 1],
												 attrib.vertices[3 * idx.vertex_index + 2]));
					if (!attrib.normals.empty()) {
						normals.push_back(glm::vec3(attrib.normals[3 * idx.normal_index + 0],
													attrib.normals[3 * idx.normal_index + 1],
													attrib.normals[3 * idx.normal_index + 2]));
					}
				}
				index_offset += fv;
			}
		}
	}
}
struct {
        bool operator()(Triangle* a, Triangle* b) const { return a->average.x < b->average.x; }
    } xless;
struct {
	bool operator()(Triangle* a, Triangle* b) const { return a->average.y < b->average.y; }
} yless;
struct {
	bool operator()(Triangle* a, Triangle* b) const { return a->average.z < b->average.z; }
} zless;


void DivideTriangles(BVH* node, int axis) {
	if (axis == 0) {
		for (int i = 0; i < node->triangles.size(); i++) {
			for (int j = i+1; j < node->triangles.size(); j++) {
				if (node->triangles[j]->average.x < node->triangles[i]->average.x) {
					std::swap(node->triangles[i], node->triangles[j]);
				}
			}
		}
	} else if (axis == 1) {
		for (int i = 0; i < node->triangles.size(); i++) {
			for (int j = i+1; j < node->triangles.size(); j++) {
				if (node->triangles[j]->average.y < node->triangles[i]->average.y) {
					std::swap(node->triangles[i], node->triangles[j]);
				}
			}
		}
	} else {
		for (int i = 0; i < node->triangles.size(); i++) {
			for (int j = i+1; j < node->triangles.size(); j++) {
				if (node->triangles[j]->average.z < node->triangles[i]->average.z) {
					std::swap(node->triangles[i], node->triangles[j]);
				}
			}
		}
	}
	if (node->triangles.size() <= 3) {
		return;
	}
	size_t const split = node->triangles.size() / 2;

	std::vector<Triangle*> lo(node->triangles.begin(), node->triangles.begin() + split);
	std::vector<Triangle*> hi(node->triangles.begin() + split, node->triangles.end());

	node->left = new BVH(lo);
	node->right = new BVH(hi);

	int currentAxis = axis + 1;
	if (currentAxis >= 3) {
		currentAxis = 0;
	}

	DivideTriangles(node->left, currentAxis);
	DivideTriangles(node->right, currentAxis);
}

void CreateBoundingHeirarchy(BVH* root) {
	for (int i = 0; i < root->triangles.size(); i++) {
		root->triangles[i]->average = (root->triangles[i]->P0 + root->triangles[i]->P1 + root->triangles[i]->P2) * 0.3333f;
	}
	root->left = NULL;
	root->right = NULL;
	DivideTriangles(root, 0);
}

void Init(int argc, char** argv)
{
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment5 - Andrew Plant", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	ClearFrameBuffer();
	
	if (atoi(argv[1]) == 0) 
	{
		Sphere* sphere1 = new Sphere(glm::vec3(-1.0, -0.7, 3.0), 0.3, glm::vec3(0.1,0.1,0.1), glm::vec3(0.2,1.0,0.2), glm::vec3(1.0,1.0,1.0), glm::vec3(0.0,0.0,0.0), 100.0);
		shapes.push_back(sphere1);
		Sphere* sphere2 = new Sphere(glm::vec3(1.0, -0.5, 3.0), 0.5, glm::vec3(0.1,0.1,0.1), glm::vec3(0.0,0.0,1.0), glm::vec3(1.0,1.0,1.0), glm::vec3(0.0,0.0,0.0), 10.0);
		shapes.push_back(sphere2);
		Sphere* sphere3 = new Sphere(glm::vec3(-1.0, 0.0, 0.0), 1.0, glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,0.0,0.0), glm::vec3(1.0,1.0,1.0), 0.0);
		shapes.push_back(sphere3);
		Sphere* sphere4 = new Sphere(glm::vec3(1.0, 0.0, -1.0), 1.0, glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,0.0,0.0), glm::vec3(0.8,0.8,0.8), 0.0);
		shapes.push_back(sphere4);

		Plane* plane1 = new Plane(glm::vec3(0.0,-1.0,0.0), glm::vec3(0.0,1.0,0.0), glm::vec3(0.1,0.1,0.1), glm::vec3(1.0,1.0,1.0), glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,0.0,0.0), 0.0);
		shapes.push_back(plane1);
		Plane* plane2 = new Plane(glm::vec3(0.0,0.0,-3.0), glm::vec3(0.0,0.0,1.0), glm::vec3(0.1,0.1,0.1), glm::vec3(1.0,1.0,1.0), glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,0.0,0.0), 0.0);
		shapes.push_back(plane2);
	} 
	if (atoi(argv[1]) == 1) 
	{	
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		LoadModel("../obj/bunny.obj", vertices, normals);
		CreateTriangleVector(vertices, normals);
 		BVH* bvh = new BVH(triangles);
		shapes.push_back(bvh);
	}

	if (atoi(argv[1]) == 2) {
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		LoadModel("../obj/bunny.obj", vertices, normals);
		CreateTriangleVector(vertices, normals);
		BVH* root = new BVH(triangles);
		CreateBoundingHeirarchy(root);
		shapes.push_back(root);
	}

	Light* light1 = new Light(glm::vec3(0.0,3.0,-2.0), glm::vec3(0.2,0.2,0.2));
	Light* light2 = new Light(glm::vec3(-2.0,1.0,4.0), glm::vec3(0.5,0.5,0.5));
	lights.push_back(light1);
	lights.push_back(light2);

	Scene scene = Scene(shapes, lights);
	Camera camera = Camera(WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(0.0,0.0,7.0), glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,1.0,0.0), 45.0, 1.0);
	
	auto start = std::chrono::high_resolution_clock::now();
	camera.TakePicture(&scene);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "Time passed (ms): " << duration.count() << std::endl;
	
	float *renderedImage = camera.GetRenderedImage();
	memcpy(frameBuffer, renderedImage, sizeof(float) * WINDOW_HEIGHT * WINDOW_WIDTH * 3);
}


int main(int argc, char** argv)
{	
	if (argc < 1) {
		std::cout << "Not enough parameters, pass the scene number: [0] or [1]" << std::endl;
		std::cout << "0 : Sphere Scene      1: Bunny Scene" << std::endl;
		return 0;
	}
	Init(argc, argv);
	while ( glfwWindowShouldClose(window) == 0) 
	{
		glClear(GL_COLOR_BUFFER_BIT);
		Display();
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}