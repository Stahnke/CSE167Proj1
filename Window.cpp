#include "Window.h"

const char* window_title = "GLFW Starter Project";
Cube cube(5.0f);

OBJObject * Window::object1;
OBJObject * Window::object2;
OBJObject * Window::object3;

int Window::width;
int Window::height;
int Window::object_num;
bool Window::rasterizeOn;

void Window::initialize_objects()
{
	object1 = new OBJObject("C:\\Users\\Ty\\Documents\\School\\FA 16\\CSE 167\\CSE167StarterCode-master\\MyResources\\bunny.obj");
	object2 = new OBJObject("C:\\Users\\Ty\\Documents\\School\\FA 16\\CSE 167\\CSE167StarterCode-master\\MyResources\\bear.obj");
	object3 = new OBJObject("C:\\Users\\Ty\\Documents\\School\\FA 16\\CSE 167\\CSE167StarterCode-master\\MyResources\\dragon.obj");
}

void Window::clean_up()
{
	//TODO:: DEALLOC THE NEW OBJECTS
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size
	glViewport(0, 0, width, height);
	// Set the matrix mode to GL_PROJECTION to determine the proper camera properties
	glMatrixMode(GL_PROJECTION);
	// Load the identity matrix
	glLoadIdentity();
	// Set the perspective of the projection viewing frustum
	gluPerspective(60.0, double(width) / (double)height, 1.0, 1000.0);
	// Move camera back 20 units so that it looks at the origin (or else it's in the origin)
	glTranslatef(0, 0, -20);
}

void Window::idle_callback()
{
	// Perform any updates as necessary. Here, we will spin the cube slightly.
	if (object_num == 0)
		object1->update();
	else if (object_num == 1)
		object2->update();
	else if (object_num == 2)
		object3->update();
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Set the matrix mode to GL_MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	// Load the identity matrix
	glLoadIdentity();
	
	// Render objects
	if (object_num == 0)
		object1->draw();
	else if (object_num == 1)
		object2->draw();
	else if (object_num == 2)
		object3->draw();

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{

		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		else if (key == GLFW_KEY_F1)
		{
			object_num = 0;
			//cout << "F1" << endl;
		}

		else if (key == GLFW_KEY_F2)
		{
			object_num = 1;
			//cout << "F2" << endl;
		}

		else if (key == GLFW_KEY_F3)
		{
			object_num = 2;
			//cout << "F3" << endl;
		}

		else if (key == GLFW_KEY_P)
		{
			if (mods == GLFW_MOD_SHIFT)
			{
				resizeObject(1.0f);
			}
			else
			{
				resizeObject(-1.0f);
			}
		}

		else if (key == GLFW_KEY_X)
		{
			if (mods == GLFW_MOD_SHIFT) 
			{
				translateObject({ 1.0f,0.0f,0.0f });
				//cout << "X" << mods << endl;
			}
			else 
			{
				translateObject({-1.0f,0.0f,0.0f });
				//cout << "x" << endl;
			}
		}

		else if (key == GLFW_KEY_Y)
		{
			if (mods == GLFW_MOD_SHIFT)
			{
				translateObject({ 0.0f,1.0f,0.0f });
				//cout << "Y" << mods << endl;
			}
			else
			{
				translateObject({ 0.0f,-1.0f,0.0f });
				//cout << "y" << endl;
			}
		}

		else if (key == GLFW_KEY_Z)
		{
			if (mods == GLFW_MOD_SHIFT)
			{
				translateObject({ 0.0f,0.0f,1.0f });
				//cout << "Z" << mods << endl;
			}
			else
			{
				translateObject({ 0.0f,0.0f,-1.0f });
				//cout << "z" << endl;
			}
		}

		else if (key == GLFW_KEY_S)
		{
			if (mods == GLFW_MOD_SHIFT)
			{
				scaleObject({2.0f,2.0f,2.0f });
				//cout << "S" << mods << endl;
			}
			else
			{
				scaleObject({ 0.5f,0.5f,0.5f });
				//cout << "s" << endl;
			}
		}

		else if (key == GLFW_KEY_O)
		{
			if (mods == GLFW_MOD_SHIFT)
			{
				orbitObject(-10.0f);
				//cout << "O" << mods << endl;
			}
			else
			{
				orbitObject(10.0f);
				//cout << "o" << endl;
			}
		}

		else if (key == GLFW_KEY_R)
		{
				resetObject();
				//cout << "r" << endl;
		}

		else if (key == GLFW_KEY_M)
		{
			if (rasterizeOn) rasterizeOn = false;
			else rasterizeOn = true;
		}
	}
}

void Window::resizeObject(float change)
{
	if (object_num == 0)
		object1->resize(change);
	else if (object_num == 1)
		object2->resize(change);
	else if (object_num == 2)
		object3->resize(change);
}

void Window::translateObject(glm::vec3 transVec) 
{
	if (object_num == 0)
		object1->translate(transVec);
	else if (object_num == 1)
		object2->translate(transVec);
	else if (object_num == 2)
		object3->translate(transVec);
}

void Window::scaleObject(glm::vec3 transVec)
{
	if (object_num == 0)
		object1->scale(transVec);
	else if (object_num == 1)
		object2->scale(transVec);
	else if (object_num == 2)
		object3->scale(transVec);
}

void Window::orbitObject(float deg)
{
	if (object_num == 0)
		object1->orbit(deg);
	else if (object_num == 1)
		object2->orbit(deg);
	else if (object_num == 2)
		object3->orbit(deg);
}

void Window::resetObject()
{
	if (object_num == 0)
		object1->reset();
	else if (object_num == 1)
		object2->reset();
	else if (object_num == 2)
		object3->reset();
}

bool Window::getRenderingMode() {
	return rasterizeOn;
}

OBJObject * Window::getObject() {
	if (object_num == 0)
		return object1;
	else if (object_num == 1)
		return object2;
	else if (object_num == 2)
		return object3;
}

int Window::getHeight() {
	return height;
}

int Window::getWidth() {
	return width;
}