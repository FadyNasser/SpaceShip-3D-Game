#include "Spaceship.h"

Spaceship::Spaceship():ObjectModel("spaceship_clean.obj","Spaceship.bmp")
{
	printf( "\n Spaceship constructor");
	ObjectModel::getBuffers()->Create();
	ObjectBoundingBox = ObjectModel::getBuffers()->getBufferBoundingBox();
	ObjectModel::scaleBoundingBox(0.25f, 0.25f, 0.25f);
    
// Fuel Calculation
	Fuel = 10000; 
	Velocity = 0; 

}
Spaceship::~Spaceship()
{}

