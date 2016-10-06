#ifndef _RASTERIZER_H_
#define _RASTERIZER_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>

#include "Window.h"
#include "OBJObject.h"

using namespace std;

class Rasterizer
{
public:
	static int window_width;
	static int window_height;
	static float* pixels;
	static OBJObject * object;
	static float pointSize;

	static glm::mat4 M;
	static glm::mat4 C_inverse;
	static glm::mat4 P;
	static glm::mat4 D;

	static std::vector<glm::vec3> vertices;
	static std::vector<glm::vec3> normals;
	static std::vector<glm::vec4> point;
	static std::vector<glm::vec4> transformedPoint;
	static std::vector<glm::vec2> pixelPoint;

	static void clearBuffer();
	static void drawPoint(int, int, float, float, float);
	static void rasterize();
	static void resizeCallback(GLFWwindow*, int, int);
	static void displayCallback(GLFWwindow*);
	static void printMatrix(glm::mat4);
};

#endif

