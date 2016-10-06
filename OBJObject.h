#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include <stdio.h>
#include <iostream>

using namespace std;

class OBJObject
{
public:
//private:
std::vector<unsigned int> indices;
std::vector<glm::vec3> vertices;
std::vector<glm::vec3> normals;
glm::mat4 toWorld;
glm::mat4 toWorldRot;
glm::mat4 toWorldTrans;
glm::mat4 toWorldScale;
glm::mat4 toWorldOrb;

FILE * fp;     // file pointer
float x, y, z;  // vertex coordinates
float r, g, b;  // vertex color
int c1, c2, c3;    // characters read from file
int size;

//public:
	OBJObject(const char* filepath);

	void parse(const char* filepath);
	void draw();
	void update();
	void spin(float);
	void translate(glm::vec3);
	void resize(float);
	void scale(glm::vec3);
	void orbit(float);
	void reset();
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec3> getNormals();
	glm::mat4 getToWorld();
	float getPointSize();
};

#endif