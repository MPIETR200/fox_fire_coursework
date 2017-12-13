/*
=================
cOrb.h
- Header file for class definition - SPECIFICATION
- Header file for the Crate class which is a child of cSprite class
=================
*/
#ifndef _CORB_H
#define _CORB_H
#include "cSprite.h"

class cOrb : public cSprite
{
private:
	SDL_Point orbVelocity;

public:
	cOrb();
	void update(double deltaTime);		
	void setOrbVelocity(SDL_Point orbVel);   
	SDL_Point getOrbVelocity();				
};
#endif