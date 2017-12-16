#include "Spaceship.h"

Spaceship::Spaceship():ObjectModel("spaceship_clean.obj","Spaceship.bmp")
{
	printf( "\n Spaceship constructor");
	ObjectModel::getBuffers()->Create();
	ObjectModel::scaleBoundingBox(0.25f, 0.25f, 0.25f);
}
Spaceship::~Spaceship()
{}