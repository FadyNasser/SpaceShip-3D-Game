#include "Spaceship.h"

Spaceship::Spaceship():ObjectModel("spaceship_clean.obj","Spaceship.bmp")
{
   printf( "\n Spaceship constructor");
	ObjectModel::getBuffers()->Create();
	// scale spaceship 
	ObjectModel::scaleBoundingBox(0.25f, 0.25f, 0.25f);
	//ObjectModel::setTexture("Diffusion.bmp");

}
Spaceship::~Spaceship()
{}

