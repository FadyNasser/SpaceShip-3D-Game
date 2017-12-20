#include "Spaceship.h"

Spaceship::Spaceship():ObjectModel("spaceship_clean.obj","Spaceship.bmp")
{
	printf( "\n Spaceship constructor \n");
	ObjectModel::getBuffers()->Create();
	ObjectBoundingBox = ObjectModel::getBuffers()->getBufferBoundingBox();

	ObjectModel::scaleBoundingBox(0.25f, 0.25f, 0.25f);
	
	Center.x = 0;
	Center.y = 0;
	Center.z = 0;
}
Spaceship::~Spaceship()
{}