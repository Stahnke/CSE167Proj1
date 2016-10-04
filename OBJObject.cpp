#include "OBJObject.h"

OBJObject::OBJObject(const char *filepath) 
{
	toWorld = glm::mat4(1.0f);
	xScale = yScale = zScale = 1;
	translateToOrigin = glm::translate(glm::mat4(1.0f), { 0,0,0 });
	parse(filepath);
}

void OBJObject::parse(const char *filepath) 
{
	//TODO parse the OBJ file
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
	fp = fopen(filepath, "rb");  // make the file name configurable so you can load other files
	if (fp == NULL) { cerr << "error loading file:\n" << filepath << endl; exit(-1); }  // just in case the file can't be found or is corrupt

	int count_v = 0;
	int count_vn = 0;
	cout << filepath << endl;
	while ((c1 = fgetc(fp)) != EOF)
	{
		if ((c1 == 'v'))
		{
			c2 = fgetc(fp);
			if (c2 == ' ')
			{
				count_v++;
				fscanf(fp, "%f %f %f", &x, &y, &z);
				vertices.push_back({ x,y,z });
				//cout << count_v << "." << "x: " << x << " y: " << y << " z: " << z << endl;
			}

			else if (c2 == 'n')
			{
				c3 = fgetc(fp);
				if (c3 == ' ') 
				{
					count_vn++;
					fscanf(fp, "%f %f %f", &r, &g, &b);
					normals.push_back({ r,g,b });
					//cout << count_vn << "." << "r: " << r << " g: " << g << " b: " << b << endl;
				}
			}
		}
	}
	cout << endl;
	fclose(fp);   // make sure you don't forget to close the file when done
}

void OBJObject::draw() 
{
	glMatrixMode(GL_MODELVIEW);

	// Push a save state onto the matrix stack, and multiply in the toWorld matrix
	glPushMatrix();
	this->toWorld =  toWorldOrb * toWorldTrans * toWorldScale * toWorldRot;
	glMultMatrixf(&(toWorld[0][0]));

	glBegin(GL_POINTS);
	// Loop through all the vertices of this OBJ Object and render them
	for (unsigned int i = 0; i < vertices.size(); ++i) 
	{
		glColor3f((glm::normalize(normals[i].x) + 1) /2, (glm::normalize(normals[i].y) + 1) / 2, (glm::normalize(normals[i].z) + 1) / 2);
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
		//glNormal3f(normals[i].x, normals[i].y, normals[i].z);
	}
	glEnd();

	// Pop the save state off the matrix stack
	// This will undo the multiply we did earlier
	glPopMatrix();
}

void OBJObject::update()
{
	spin(1.0f);
}

void OBJObject::spin(float deg)
{
	this->angle += deg;
	if (this->angle > 360.0f || this->angle < -360.0f) this->angle = 0.0f;
	// This creates the matrix to rotate the object
	this->toWorldRot = glm::rotate(glm::mat4(1.0f), this->angle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
}

void OBJObject::translate(glm::vec3 transVec)
{
	//SOMEHOW NEED TO BE ABLE TO DO MULTIPLE TRANSFORMATIONS
	//setting this->toWorld deletes all other transformations before next render
	this->xPos += transVec.x;
	this->yPos += transVec.y;
	this->zPos += transVec.z;
	glm::vec3 tempVec(xPos, yPos, zPos);
	cout << "TRANSLATE BY: " << transVec.x << endl;
	this->toWorldTrans = glm::translate(glm::mat4(1.0f), tempVec);
}

void OBJObject::resize(float change)
{
	this->size += change;
	glPointSize(size);
}

void OBJObject::scale(glm::vec3 scaleVector)
{
	this->xScale *= scaleVector.x;
	if (this->xScale <= 0)
		xScale = 0.0125f;
	this->yScale *= scaleVector.y;
	if (this->yScale <= 0)
		yScale = 0.0125f;
	this->zScale *= scaleVector.z;
	if (this->zScale <= 0)
		zScale = 0.0125f;
	
	glm::vec3 tempVec(xScale, yScale, zScale);
	this->toWorldScale = glm::scale(glm::mat4(1.0f), tempVec);
}

void OBJObject::orbit(float deg)
{
	this->angleOrbit += deg;
	if (this->angleOrbit > 360.0f || this->angleOrbit < -360.0f) this->angleOrbit = 0.0f;
	// This creates the matrix to rotate the object
	this->toWorldOrb = glm::rotate(glm::mat4(1.0f), this->angleOrbit / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
}

void OBJObject::reset() 
{
	this->xPos = this->yPos = this->zPos = 0;
	this->xScale = this->yScale = this->zScale = 1;
	this->angle = 0;
	this->angleOrbit = 0;
	orbit(0);
	translate({ 0,0,0 });
	scale({ 1,1,1 });
	spin(0);
}