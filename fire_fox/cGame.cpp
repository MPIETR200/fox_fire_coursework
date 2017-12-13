/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();


/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	
	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();

	// the code for saving the game? added from lab 10
	// Set filename
	theFile.setFileName("Data/usermap.dat");
	// Check file is available
	if (!theFile.openFile(ios::in)) //open file for input output
	{
	cout << "Could not open specified file '" << theFile.getFileName() << "'. Error " << SDL_GetError() << endl;
	fileAvailable = false;
	}
	else
	{
	cout << "File '" << theFile.getFileName() << "' opened for input!" << endl;
	fileAvailable = true;
	}

	
	// Store the textures
	textureName = { "orb1", "orb2", "orb3", "orb4","theCrate","theBackground", "theMenuBkg", "EndScreen"};
	texturesToUse = { "Images\\norm.png", "Images\\lit.png", "Images\\norm.png", "Images\\lit.png", "Images\\Crate1.png", "Images\\MainGame.png", "Images\\MenuScreen.png", "Images\\EndScreen.png" };
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}
	// part of the code for buttons

	btnNameList = { "exit_btn", "instructions_btn", "load_btn", "menu_btn", "play_btn", "save_btn", "settings_btn" };
	btnTexturesToUse = { "Images/Buttons/button_exit.png", "Images/Buttons/button_instructions.png", "Images/Buttons/button_load.png", "Images/Buttons/button_menu.png", "Images/Buttons/button_play.png", "Images/Buttons/button_save.png", "Images/Buttons/button_settings.png" };
	btnPos = { { 900, 675 }, { 400, 300 }, { 400, 300 }, { 500, 500 }, { 900, 600 }, { 740, 500 }, { 400, 300 } };
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
	theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
	cButton * newBtn = new cButton();
	newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
	newBtn->setSpritePos(btnPos[bCount]);
	newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
	theButtonMgr->add(btnNameList[bCount], newBtn);
	}
	theGameState = MENU;
	theBtnType = EXIT;

	
	// Create textures for Game Dialogue (text)
	fontList = { "asian",  "fox" };
	fontsToUse = { "Fonts/Asian Delight.ttf", "Fonts/Brown Fox.ttf" };
	
	for (int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 56);
	}
	string scoreString = to_string(score);
	LPCSTR scoreLPCSTR = scoreString.c_str();

	gameTextNames = { "Title", "keys", "Game Over", "Score", "scoreVal",};
	gameTextList = { "KITSUNE BI", "MOVEMENT KEYS:", "GAME OVER", "SCORE: ", scoreLPCSTR };
	for (int text = 0; text < gameTextNames.size(); text++)
	{
		theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("asian")->createTextTexture(theRenderer, gameTextList[text], SOLID, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }));

	}
	
	
	// Load game sounds
	soundList = { "theme", "bell", "click" };
	soundTypes = { MUSIC, SFX, SFX };
	soundsToUse = { "Audio/j_harp.wav", "Audio/chime.wav", "Audio/ClickOn.wav" };
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	theSoundMgr->getSnd("theme")->play(-1);

	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	// set sprite for menu screen

	spriteBkgd1.setSpritePos({ 0, 0 });
	spriteBkgd1.setTexture(theTextureMgr->getTexture("theMenuBkg"));
	spriteBkgd1.setSpriteDimensions(theTextureMgr->getTexture("theMenuBkg")->getTWidth(), theTextureMgr->getTexture("theMenuBkg")->getTHeight());

	spriteBkgd2.setSpritePos({ 0, 0 });
	spriteBkgd2.setTexture(theTextureMgr->getTexture("EndScreen"));
	spriteBkgd2.setSpriteDimensions(theTextureMgr->getTexture("EndScreen")->getTWidth(), theTextureMgr->getTexture("EndScreen")->getTHeight());

	// changed the sprite possition to the bottom of the screen
	theCrate.setSpritePos({ 700, 650 });
	theCrate.setTexture(theTextureMgr->getTexture("theCrate"));
	theCrate.setSpriteDimensions(theTextureMgr->getTexture("theCrate")->getTWidth(), theTextureMgr->getTexture("theCrate")->getTHeight());
	theCrate.setCrateVelocity({ 0, 0 });

	// Create vector array of textures

	for (int orbs = 0; orbs < 10; orbs++)
	{
		theOrbs.push_back(new cOrb);
		
		// spawn the orbs at the top of the screen
		theOrbs[orbs]->setSpritePos({ 100 * (rand() % 6 + 1), 0 });
		theOrbs[orbs]->setSpriteTranslation({ (0), (rand() % 8 + 1) });
		
		int randOrb = rand() % 4;
		theOrbs[orbs]->setTexture(theTextureMgr->getTexture(textureName[randOrb]));
		theOrbs[orbs]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randOrb])->getTWidth(), theTextureMgr->getTexture(textureName[randOrb])->getTHeight());
		theOrbs[orbs]->setOrbVelocity({ 3, 3 });
		theOrbs[orbs]->setActive(true);

	}

}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	bool loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	// added from lab 10
	switch (theGameState)
	{
	case MENU:
	{
		spriteBkgd1.render(theRenderer, NULL, NULL, spriteBkgd1.getSpriteScale());
		// Render the Title
		tempTextTexture = theTextureMgr->getTexture("Title");
		pos = { 350, 250, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		tempTextTexture = theTextureMgr->getTexture("keys");
		pos = { 250, 570, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		scale = { 1, 1 };
		// Render Button
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("play_btn")->setSpritePos({ 900, 600 });
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 900, 675 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
	}
	break;
	case PLAYING:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		// Render each orb in the vector array
		for (int draw = 0; draw < theOrbs.size(); draw++)
		{
			theOrbs[draw]->render(theRenderer, &theOrbs[draw]->getSpriteDimensions(), &theOrbs[draw]->getSpritePos(), theOrbs[draw]->getSpriteRotAngle(), &theOrbs[draw]->getSpriteCentre(), theOrbs[draw]->getSpriteScale());
		}
		tempTextTexture = theTextureMgr->getTexture("Score");
		pos = { 600, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		tempTextTexture = theTextureMgr->getTexture("scoreVal");
		pos = { 780, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 270, 10 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		theButtonMgr->getBtn("load_btn")->setSpritePos({ 140, 10 });
		theButtonMgr->getBtn("load_btn")->render(theRenderer, &theButtonMgr->getBtn("load_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("load_btn")->getSpritePos(), theButtonMgr->getBtn("load_btn")->getSpriteScale());
		theButtonMgr->getBtn("save_btn")->setSpritePos({ 10, 10 });
		theButtonMgr->getBtn("save_btn")->render(theRenderer, &theButtonMgr->getBtn("save_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("save_btn")->getSpritePos(), theButtonMgr->getBtn("save_btn")->getSpriteScale());
	
		// render the Crate
		theCrate.render(theRenderer, &theCrate.getSpriteDimensions(), &theCrate.getSpritePos(), theCrate.getSpriteRotAngle(), &theCrate.getSpriteCentre(), theCrate.getSpriteScale());
		SDL_RenderPresent(theRenderer);
		
	}
	break;
	case END:
	{
		spriteBkgd2.render(theRenderer, NULL, NULL, spriteBkgd2.getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("Game Over");
		pos = { 350, 300, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("Score");
		pos = { 350, 400, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("scoreVal");
		pos = { 550, 400, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 400, 600 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		theButtonMgr->getBtn("play_btn")->setSpritePos({ 550, 600 });
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
	}
	break;
	case QUIT:
	{
		loop = false;
	}
	break;
	default:
		break;
	}
	SDL_RenderPresent(theRenderer);

	;
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{

}

void cGame::update(double deltaTime)
{
	switch (theGameState)
	{
		case MENU:
		{
			theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
			theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, PLAYING, theAreaClicked);
		}
		break;
		case PLAYING:
		{
			theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, END, theAreaClicked);
			//theGameState = theButtonMgr->getBtn("load_btn")->update(theGameState, LOADMAP, theAreaClicked);
			
			if (fileAvailable && theGameState == LOADMAP)
			{

				theGameState = PLAYING;
				theAreaClicked = { 0, 0 };
			}
			//theGameState = theButtonMgr->getBtn("save_btn")->update(theGameState, SAVEMAP, theAreaClicked);
			if (theGameState == SAVEMAP)
			{
				// Check file is available
				if (!theFile.openFile(ios::out)) //open file for output
				{
				cout << "Could not open specified file '" << theFile.getFileName() << "'. Error " << SDL_GetError() << endl;
				}
				else
				{
				cout << "File '" << theFile.getFileName() << "' opened for output!" << endl;
				//theTileMap.writeMapDataToFile(&theFile);
				}

				//theTileMap.writeMapDataToFile(&theFile);
				theGameState = PLAYING;
				theAreaClicked = { 0, 0 };
			}
			// Update the visibility and position of each orb

			vector<cOrb*>::iterator orbIterator = theOrbs.begin();
			while (orbIterator != theOrbs.end())
			{
				if ((*orbIterator)->isActive() == false)
				{
					orbIterator = theOrbs.erase(orbIterator);

				}
				else
				{
					(*orbIterator)->update(deltaTime);
					++orbIterator;
				}
			}

			/*
			/*
			==============================================================
			| Check for collisions
			==============================================================
			*/

			// check for collision between the crate and orbs

			for (vector<cOrb*>::iterator orbIterator = theOrbs.begin(); orbIterator != theOrbs.end(); ++orbIterator)
			{
				if ((*orbIterator)->collidedWith(&(*orbIterator)->getBoundingRect(), &theCrate.getBoundingRect()))
				{
					// if a collision set the orb to false
					(*orbIterator)->setActive(false);
					score += 1;
					theSoundMgr->getSnd("bell")->play(0);
				}
			}


		}
	break;
	case END:
	{
		theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, PLAYING, theAreaClicked); 
		theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, MENU, theAreaClicked);
	}
	break;
	case QUIT:
	{
	
	}
	break;
	default:
		break;
	}
	
	// Update the crate position
	theCrate.update(deltaTime);
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
					//added from lab10
					theAreaClicked = { event.motion.x, event.motion.y };
					theSoundMgr->getSnd("click")->play(0);
				
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
					//added from lab 10
					if (theGameState == PLAYING)
					{
						theAreaClicked = { event.motion.x, event.motion.y };
						theSoundMgr->getSnd("click")->play(0);
					}
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEMOTION:
			break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					theLoop = false;
					break;

					// change the movement to simple left/right movement
				case SDLK_DOWN:
				{
				}
				break;

				case SDLK_UP:
				{
				}
				break;
				case SDLK_RIGHT:
				{
					// this part does not seem to work
					if (theCrate.getSpritePos().x < (renderWidth - theCrate.getSpritePos().w))
					{
					
					theCrate.setCrateVelocity({ 5, 0 });
					}
					
				}
				break;

				case SDLK_LEFT:
				{
					if (theCrate.getSpritePos().x > 0)
					{
					
					theCrate.setCrateVelocity({ -5, 0 });
					}
				}
				
				break;
				default:
					break;
				}

			default:
				break;
		}

	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

