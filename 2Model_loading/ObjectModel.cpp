#include "ObjectModel.h"
#include <iostream>
using namespace std;
float BHorientation = 3.14159f / 2.0f;

ObjectModel::ObjectModel()
{
	printf("\n Default Constructor");
}

glm::mat4 ObjectModel::GetModelTranslation()
{
	return this->Translation;
}
glm::mat4 ObjectModel::GetModelScaling()
{
	return this->Scaling;
}
glm::mat4 ObjectModel::GetModelRotation()
{
	return this->Rotation;
}

ObjectModel::ObjectModel(char* texName, Buffers* buffers , int type)
{
	ObjectBuffers = buffers;
	int i = 0;
	while (texName[i] != '\0')
	{
		TextureName[i] = texName[i];
		i++;
	}
	TextureName[i] = '\0';

	TextureIndex = loadBMP_custom(TextureName);
	Type = type; 
	ObjectBoundingBox = getBuffers()->getBufferBoundingBox();
	if (Type == 2 || Type == 4)
	{
		//RotateBoundingBox();
		//ObjectBoundingBox.Xmax += 0.25;
		//ObjectBoundingBox.Xmin -= 0.25;
		//ObjectBoundingBox.Ymax += 0.25;
		//ObjectBoundingBox.Ymin -= 0.25;
		//ObjectBoundingBox.Zmax += 0.25;
		//ObjectBoundingBox.Zmin -= 0.25;
	}
	xLength = ObjectModel::ObjectBoundingBox.Xmax - ObjectModel::ObjectBoundingBox.Xmin;
	yLength = ObjectModel::ObjectBoundingBox.Ymax - ObjectModel::ObjectBoundingBox.Ymin;
	zLength = ObjectModel::ObjectBoundingBox.Zmax - ObjectModel::ObjectBoundingBox.Zmin;
}

ObjectModel::ObjectModel(char* ObjName, char *texName) // Special Constructor for spaceship
{
	ObjectBuffers = new Buffers(ObjName); 
	int i = 0;
	while (texName[i] != '\0')
	{
		TextureName[i] = texName[i];
		i++;
	}
	TextureName[i] = '\0';

	TextureIndex = loadBMP_custom(TextureName);
	Type = 0;
}

ObjectModel::~ObjectModel()
{
//	glDeleteBuffers(1, ObjectBuffers->getVertexBuffer);
//	glDeleteBuffers(1, ObjectBuffers->getNormalBuffer);
//	glDeleteBuffers(1, ObjectBuffers->getUVBuffer);
}


bool ObjectModel::Draw(GLuint programID, GLuint MatrixID, GLuint vertexPosition_modelspaceID, GLuint vertexUVID, GLuint TextureID)
{

	glEnableVertexAttribArray(vertexPosition_modelspaceID);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectBuffers->getVertexBuffer());
	glVertexAttribPointer
		(
			vertexPosition_modelspaceID,  // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
			);
	glEnableVertexAttribArray(vertexUVID);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectBuffers->getUVBuffer());
	glVertexAttribPointer
	(
			vertexUVID,                   // The attribute we want to configure
			2,                            // size : U+V => 2
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
	);
	computeMatricesFromInputs();
	glm::mat4 SSMVP = getProjectionMatrix() * getViewMatrix() * ModelMatrix;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &SSMVP[0][0]);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, TextureIndex);
	glUniform1i(TextureID, 10);
	glDrawArrays(GL_TRIANGLES, 0, ObjectBuffers->getVerticesVector().size());
	return true; 
}

bool ObjectModel::setModelMatrix(glm::mat4 model)
{ 
	ModelMatrix = model; 
	return true; 
}

void ObjectModel::constructModelMatrix( glm::vec3 Translation, glm::vec3 Scaling, glm::vec3 Rotation)
{
	this->Translation = translate(mat4(), Translation); 
	this->Scaling = scale(mat4(),Scaling); 
	this->Rotation = eulerAngleYXZ(Rotation.y, Rotation.x, Rotation.z);
	this->ModelMatrix = this->Translation*this->Rotation*this->Scaling;
	// ORDER MATTERS : Scale BoundingBox then translate it ...NEVER THE OPPOSITE 
	scaleBoundingBox(Scaling.x, Scaling.y, Scaling.z); 
	translateBoundingBox(Translation.x, Translation.y, Translation.z); 
}

bool ObjectModel::setTexture(char texName[])
{
	int i = 0;
	while (texName[i] != '\0')
	{
		TextureName[i] = texName[i];
		i++;
	}
	TextureName[i] = '\0';
	TextureIndex = loadBMP_custom(TextureName);
	return true; 
}

Box& ObjectModel::getBoundingBox() 
{
	return ObjectBoundingBox;
}

Buffers* ObjectModel::getBuffers()
{
	return ObjectBuffers;
}

bool ObjectModel::detectCollision(Box b)
{
	//Box a = getBoundingBox(); // my bounding box
	bool Xintersection = (ObjectBoundingBox.Xmin <= b.Xmax && ObjectBoundingBox.Xmax >= b.Xmin) || (ObjectBoundingBox.Xmax <= b.Xmin && ObjectBoundingBox.Xmin >= b.Xmax);
	bool Yintersection = (ObjectBoundingBox.Ymin <= b.Ymax && ObjectBoundingBox.Ymax >= b.Ymin) || (ObjectBoundingBox.Ymax <= b.Ymin && ObjectBoundingBox.Ymin >= b.Ymax);
	bool Zintersection = (ObjectBoundingBox.Zmin <= b.Zmax && ObjectBoundingBox.Zmax >= b.Zmin) || (ObjectBoundingBox.Zmax <= b.Zmin && ObjectBoundingBox.Zmin >= b.Zmax);
	if (Xintersection && Yintersection && Zintersection)
	{
		cout << "Collision Happened " << Type << endl;
	}
	return (Xintersection && Yintersection && Zintersection);
}

int ObjectModel::getType()
{
	return Type;
}

void ObjectModel::translateBoundingBox(float x, float y, float z) // changes the position of xMin,yMin,Zmin based on the scaled Xlength and yLength 
{
	ObjectBoundingBox.Xmax = x + (xLength / 2);
	ObjectBoundingBox.Xmin = x - (xLength / 2);
	ObjectBoundingBox.Ymax = y + (yLength / 2);
	ObjectBoundingBox.Ymin = y - (yLength / 2);
	ObjectBoundingBox.Zmax = z + (zLength / 2);
	ObjectBoundingBox.Zmin = z - (zLength / 2);
}

void ObjectModel::scaleBoundingBox(float x, float y, float z) // update xLength , yLength , zLength 
{
	struct Box myBox = ObjectBuffers->getBufferBoundingBox();
	xLength = myBox.Xmax - myBox.Xmin;
	yLength = myBox.Ymax - myBox.Ymin;
	zLength = myBox.Zmax - myBox.Zmin;
	xLength *= x;
	yLength *= y;
	zLength *= z;
}

void ObjectModel::rotateObject(glm::mat4 newRotation)
{
	this->Rotation = newRotation; 
	ModelMatrix = Translation*Rotation*Scaling;
}

void ObjectModel::RotateBoundingBox()
{
	//y' = y*cos q - z*sin q
	//z' = y*sin q + z*cos q
	//x' = x
	ObjectBoundingBox.Ymax = ObjectBoundingBox.Ymax * cos(BHorientation) - ObjectBoundingBox.Zmax * sin(BHorientation);
	ObjectBoundingBox.Ymin = ObjectBoundingBox.Ymin * cos(BHorientation) - ObjectBoundingBox.Zmin * sin(BHorientation);
	ObjectBoundingBox.Zmax = ObjectBoundingBox.Ymax * sin(BHorientation) - ObjectBoundingBox.Zmax * cos(BHorientation);
	ObjectBoundingBox.Zmin = ObjectBoundingBox.Ymin * sin(BHorientation) - ObjectBoundingBox.Zmin * cos(BHorientation);
}