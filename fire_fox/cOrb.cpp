/*
=================
cOrb.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cOrb.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cOrb::cOrb() : cSprite()
{
	this->orbVelocity = { 0, 0 };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cOrb::update(double deltaTime)
{

	this->setSpriteRotAngle(this->getSpriteRotAngle() +(5.0f * deltaTime)); 
	if (this->getSpriteRotAngle() > 360)
	{
		this->setSpriteRotAngle(this->getSpriteRotAngle() -360);
	}

	SDL_Rect currentSpritePos = this->getSpritePos();

	// creates an illusion of spawning more objects
	if (currentSpritePos.y > (770 + rand() % 100))
	{
		currentSpritePos.x += this->getSpriteTranslation().x * deltaTime;
		currentSpritePos.y = -150 + (rand() % -200);
	}
	// adding movement in the y direction
	this->setSpritePos({ currentSpritePos.x , currentSpritePos.y + rand() % 10 });
	cout << "Orb position - x: " << this->getSpritePos().x << " y: " << this->getSpritePos().y << " deltaTime: " << deltaTime << endl;
	this->setBoundingRect(this->getSpritePos());
}
/*
=================================================================
  Sets the velocity for the Orbs
=================================================================
*/
void cOrb::setOrbVelocity(SDL_Point OrbVel)
{
	orbVelocity = OrbVel;
}
/*
=================================================================
  Gets the Orb velocity
=================================================================
*/
SDL_Point cOrb::getOrbVelocity()
{
	return orbVelocity;
}
