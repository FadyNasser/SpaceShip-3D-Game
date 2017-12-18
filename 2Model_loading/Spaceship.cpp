#include "Spaceship.h"

Spaceship::Spaceship():ObjectModel("spaceship_clean.obj","Spaceship.bmp")
{
	printf( "\n Spaceship constructor");
	ObjectModel::getBuffers()->Create();
	ObjectBoundingBox = ObjectModel::getBuffers()->getBufferBoundingBox();
	ObjectModel::scaleBoundingBox(0.25f, 0.25f, 0.25f);
	xLength = ObjectBoundingBox.Xmax - ObjectBoundingBox.Xmin;
	yLength = ObjectBoundingBox.Ymax - ObjectBoundingBox.Ymin;
	zLength = ObjectBoundingBox.Zmax - ObjectBoundingBox.Zmin;
}
Spaceship::~Spaceship()
{}

