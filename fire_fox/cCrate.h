/*
=================
cCrate.h
- Header file for class definition - SPECIFICATION
- Header file for the Crate class which is a child of cSprite class
=================
*/
#ifndef _CCRATE_H
#define _CCRATE_H
#include "cSprite.h"

class cCrate : public cSprite
{
private:
	SDL_Point crateVelocity;

public:
	cCrate();
	void update(double deltaTime);		
	void setCrateVelocity(SDL_Point crateVel);  
	SDL_Point getCrateVelocity();				 
};
#endif