#include "Rasterizer.h"

int Rasterizer::window_width = 640;
int Rasterizer::window_height = 480;
float* Rasterizer::pixels = new float[window_width * window_height * 3];
OBJObject * Rasterizer::object;
float Rasterizer::pointSize;

bool DEBUG = false;
bool DEBUG2 = false;

glm::mat4 Rasterizer::M;
glm::mat4 Rasterizer::C_inverse;
glm::mat4 Rasterizer::P;
glm::mat4 Rasterizer::D;

std::vector<glm::vec3> Rasterizer::vertices;
std::vector<glm::vec3> Rasterizer::normals;
std::vector<glm::vec4> Rasterizer::point;
std::vector<glm::vec4> Rasterizer::transformedPoint;
std::vector<glm::vec2> Rasterizer::pixelPoint;

using namespace std;

struct Color    // generic color class
{
	float r, g, b;  // red, green, blue
};

// Clear frame buffer
void Rasterizer::clearBuffer()
{
	Color clearColor = { 0.0, 0.0, 0.0 };   // clear color: black
	for (int i = 0; i<window_width*window_height; ++i)
	{
		pixels[i * 3] = clearColor.r;
		pixels[i * 3 + 1] = clearColor.g;
		pixels[i * 3 + 2] = clearColor.b;
	}
}

// Draw a point into the frame buffer
void Rasterizer::drawPoint(int x, int y, float r, float g, float b)
{
	int offset = y*window_width * 3 + x * 3;
	pixels[offset] = r;
	pixels[offset + 1] = g;
	pixels[offset + 2] = b;
}

void Rasterizer::rasterize()
{

	// Put your main rasterization loop here
	// It should go over the point model and call drawPoint for every point in it

	// === Get all points p ===

	//Get the Obj Object reference
	object = Window::getObject();

	//Get the vertices and normals
	vertices = object->getVertices();
	normals = object->getNormals();
	pointSize = object->getPointSize();

	point.clear();
	transformedPoint.clear();
	pixelPoint.clear();

	// === Make vectors homogenous ===

	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		point.push_back({ vertices[i].x, vertices[i].y, vertices[i].z, 1.0f });
	}

	// === Get all transformation matrices ===

	//Get world transformation matrix from object
	M = object->getToWorld();

	//Get camera space, then inverse of camera space
	C_inverse = glm::lookAt(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//Get perspective space
	P = glm::perspective(glm::radians(60.0f), (float)window_width / (float)window_height, 1.0f, 1000.0f);

	//Get image space

	D = glm::mat4x4(
		(float)(window_width)/2.0f, 0.0f, 0.0f, 0.0f,
		0.0f, (float)(window_height)/2.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		(float)(window_width)/2.0f, (float)(window_height)/2.0f, 0.5f, 1.0f
	);

	// === Transform all points ===

	//Transform all of the homogenous points by the world matrix
	for (unsigned int i = 0; i < point.size(); i++)
	{
		transformedPoint.push_back( D * (P * (C_inverse * (M * point[i]))));
	}

	//Convert these points to pixels
	for (unsigned int i = 0; i < transformedPoint.size(); i++) {
		pixelPoint.push_back({ (transformedPoint[i].x / transformedPoint[i].w), (transformedPoint[i].y / transformedPoint[i].w)});
	}

	//=== Draw all points ==
	unsigned int i = 0;
	//if(DEBUG) cin >> i;
	for (i = 0; i < pixelPoint.size(); i++)
	{
		for (int p = 0; p < pointSize; p++)
		{
			for (int q = 0; q < pointSize; q++)
			{
				if (pixelPoint[i].x + p >= 0 && pixelPoint[i].x + p < window_width && pixelPoint[i].y + q >= 0 && pixelPoint[i].y + q < window_height)
					drawPoint(pixelPoint[i].x + p, pixelPoint[i].y + q, (glm::normalize(normals[i].x) + 1) / 2, (glm::normalize(normals[i].y) + 1) / 2, (glm::normalize(normals[i].z) + 1) / 2);
			}
		}

		if (DEBUG)
		{
			if(DEBUG2)
			{
				cout << "point " << i << ": " << "x: " << vertices[i].x << " y: " << vertices[i].y << " z: " << vertices[i].z << endl;
				cout << "homogenized point " << i << ": " << "x: " << point[i].x << " y: " << point[i].y << " z: " << point[i].z << " w: " << point[i].w << endl;
				cout << endl;

				cout << "WORLD MAT:" << endl;
				printMatrix(M);
				cout << "world " << i << ": " << "x: " << (M * point[i]).x << " y: " << (M * point[i]).y << " z: " << (M * point[i]).z << " w: " << (M * point[i]).w << endl;
				cout << endl;

				cout << "CAMERA MAT:" << endl;
				printMatrix(C_inverse);
				cout << "camera " << i << ": " << "x: " << (C_inverse * (M * point[i])).x << " y: " << (C_inverse* (M * point[i])).y << " z: " << (C_inverse * (M * point[i])).z << " w: " << (C_inverse * (M * point[i])).w << endl;
				cout << endl;

				cout << "PERSPECTIVE MAT:" << endl;
				printMatrix(P);
				cout << "perspective " << i << ": " << "x: " << (P*(C_inverse * (M * point[i]))).x << " y: " << (P*(C_inverse* (M * point[i]))).y << " z: " << (P*(C_inverse * (M * point[i]))).z << " w: " << (P*(C_inverse * (M * point[i]))).w << endl;
				cout << endl;

				cout << "IMAGE MAT:" << endl;
				printMatrix(D);
				cout << "image " << i << ": " << "x: " << (D*(P*(C_inverse * (M * point[i])))).x << " y: " << (D*(P*(C_inverse* (M * point[i])))).y << " z: " << (D*(P*(C_inverse * (M * point[i])))).z << " w: " << (D*(P*(C_inverse * (M * point[i])))).w << endl;
				cout << endl;
			}
			cout << "pixel: " << i << ": " << "pixel x: " << pixelPoint[i].x << " pixel y: " << pixelPoint[i].y << endl;
		}
	}

	//Messing around with draw point
	/*for (unsigned int i = 0; i < window_width; i++)
	{
		for (unsigned int j = 0; j < window_height; j++)
		{
			drawPoint(i, j, (glm::normalize(normals[i].x) + 1) / 2, (glm::normalize(normals[i].y) + 1) / 2, (glm::normalize(normals[i].z) + 1) / 2);
		}
	}*/
}

// Called whenever the window size changes
void Rasterizer::resizeCallback(GLFWwindow* window, int width, int height)
{
	window_width = width;
	window_height = height;
	delete[] pixels;
	pixels = new float[window_width * window_height * 3];
}

void Rasterizer::displayCallback(GLFWwindow* window)
{
	//GET HEIGHT AND WIDTH
	window_height = Window::getHeight();
	window_width = Window::getWidth();
	resizeCallback(window, window_width, window_height);

	clearBuffer();
	rasterize();

	// glDrawPixels writes a block of pixels to the framebuffer
	glDrawPixels(window_width, window_height, GL_RGB, GL_FLOAT, pixels);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Rasterizer::printMatrix(glm::mat4 mat) {

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << mat[j][i] << " ";
		}
		cout << endl;
	}
	cout << endl;
}
