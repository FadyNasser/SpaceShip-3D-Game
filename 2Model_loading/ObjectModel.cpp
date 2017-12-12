#include "ObjectModel.h"

//
////	// Shall they be added in GLobal Header file? or passed to the function?

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
	//Stupid Solution
	/*int i=0; 
	while (name[i] != '\0')
	{
		ObjectName[i] = name[i]; 
		i++;
	}
	ObjectName[i] = '\0';*/
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
}
// Special Constructor for spaceship
ObjectModel::ObjectModel(char* ObjName, char *texName)
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
	this->Translation=translate(mat4(), Translation); 
	this->Scaling = scale(mat4(),Scaling); 
	//this->Rotation = eulerAngleX(xrotate) * eulerAngleY(yrotate);
	this->Rotation = eulerAngleYXZ(Rotation.y, Rotation.x, Rotation.z);
	this->ModelMatrix = this->Translation*this->Rotation*this->Scaling;
	translateBoundingBox(Translation.x, Translation.y, Translation.z); 
	scaleBoundingBox(Scaling.x, Scaling.y, Scaling.z);
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


Box& ObjectModel::getBoundingBox() {
	return ObjectBuffers->getBoundingBox();
}

Buffers* ObjectModel::getBuffers()
{
	return ObjectBuffers;
}

bool ObjectModel::detectCollision(Box b)
{
	Box a = getBoundingBox(); // my bounding box
	bool Xintersection = a.Xmin <= b.Xmax && a.Xmax >= b.Xmin;
	bool Yintersection = a.Ymin <= b.Ymax && a.Ymax >= b.Ymin;
	bool Zintersection = a.Zmin <= b.Zmax && a.Zmax >= b.Zmin;
	return (Xintersection && Yintersection && Zintersection);
}


int ObjectModel::getType()
{
	return Type;
}

void ObjectModel::translateBoundingBox(float x, float y, float z)
{
		ObjectBuffers->translateBoundingBox(x, y, z);

}
void ObjectModel::scaleBoundingBox(float x, float y, float z)
{
	ObjectBuffers->scaleBoundingBox(x, y, z);

}