/*
Copyright 2007, 2008 Andrew Allen and Brian Shourd

This file is part of Coralstone.

Coralstone is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Coralstone is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Coralstone (Called LICENSE.txt).  If not, see
<http://www.gnu.org/licenses/>.
*/

#include "Graphics.h"
#include "constants.h"
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

Graphics::Graphics()
{
    screen = NULL;
    dynamicLayer = NULL;
    player = NULL;
    map = NULL;

    fpsTicks = fpsFrames = lastTime = 0;
}

Graphics::~Graphics()
{
    CleanUp();
}

//Load the image at filename with the color specified by red, green, and blue as the transparent color. The default
//transparency color is bright blue, so it needn't be specified. Returns a pointer to this created surface
SDL_Surface* Graphics::LoadImage(std::string filename, int red, int green, int blue)
{
	SDL_Surface *loadedImage = NULL;
	SDL_Surface *optimizedImage = NULL;
	loadedImage = IMG_Load(filename.c_str());
	if (loadedImage != NULL)
	{
		optimizedImage = SDL_DisplayFormat(loadedImage);
		SDL_FreeSurface(loadedImage);
		if (optimizedImage != NULL)
		{
			Uint32 colorKey = SDL_MapRGB(optimizedImage->format, red, green, blue);
			SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, colorKey);
		}
	}
	return optimizedImage;
}

//Apply the surface from source to destination, at the position x, y and with the clip defined by clip, which defaults to the entire surface
void Graphics::ApplyImage(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip)
{
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(source, clip, destination, &offset);
}

//Initialize the SDL libraries to be used, returns false if it fails
bool Graphics::Init()
{
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_HWSURFACE | SDL_DOUBLEBUF/* | SDL_NOFRAME*/);
    if (screen == NULL) {return false;}
    dynamicLayer = SDL_CreateRGBSurface(SDL_HWSURFACE, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, NULL, NULL, NULL, NULL);
    if (dynamicLayer == NULL) {return false;}
    return true;
}

void Graphics::SetPlayer(Character* thePlayer)
{
    player = thePlayer;
}

void Graphics::SetMap(Map* theMap)
{
    map = theMap;
}

SDL_Surface* Graphics::GetScreen()
{
    return screen;
}

SDL_Surface* Graphics::GetDynamicLayer()
{
    return dynamicLayer;
}

//Updates the main screen, this way everything is hunky-dory and the screen only gets flipped once per frame
bool Graphics::Update()
{
    //First, sort the characters by layer, so that they can be updated in proper relation to each other
        //We only do one pass to save time, so we assume that not much changes in between frames
    for (unsigned int i = 0; i < characterArray.size() - 1; i++)
    {
        if (characterArray[i]->GetPosition().y >characterArray[i + 1]->GetPosition().y)
        {
            Character* temp = characterArray[i];
            characterArray[i] = characterArray[i + 1];
            characterArray[i + 1] = temp;
        }
    }

    //Now clear the screen
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

    //Set the camera here
  	SDL_Rect screenLocation;	//Defines essentially where the camera is at
	screenLocation.h = SCREEN_HEIGHT;
	screenLocation.w = SCREEN_WIDTH;
	screenLocation.y = 0;

	//Keep the screenLocation relative to the player's position
	if (player != NULL)
	{
		Point pos = player->GetPosition();
		screenLocation.x = pos.x - (SCREEN_WIDTH - 48)/2;
		screenLocation.y = pos.y - (SCREEN_HEIGHT - 64)/2;
	}
	else
	{
		screenLocation.x = 0;
	}

	//Apply the map to the screen
	if (map != NULL)
	{
        map->ApplyMap(screenLocation.x, screenLocation.y, SCREEN_HEIGHT, SCREEN_WIDTH, dynamicLayer);
	}

	//Whoever is targeted, make a square underneath them so we know
	SDL_Rect clip;
	Character* target = NULL;
	if (player != NULL)
	{
        target = player->GetTarget();
	}
	if (target != NULL)
	{
        clip.x = target->GetPosition().x + 2;
        clip.w = CHARACTER_WIDTH - 4;
        clip.y = target->GetPosition().y + 54;
        clip.h = 20;
        SDL_FillRect(dynamicLayer, &clip, SDL_MapRGB(dynamicLayer->format, 0, 255, 255));
	}

    //Frames per second calculator
    fpsFrames++;
    if (SDL_GetTicks() - fpsTicks > 1000)
    {
        if (DEBUG_SHOWALL || DEBUG_SHOWFPS)
        {
            printf("Frames per second is %f\n", fpsFrames/((SDL_GetTicks() - fpsTicks)/1000.0));
        }
        fpsTicks = SDL_GetTicks();
        fpsFrames = 0;
    }

    //Lastly, draw the characters
    for (unsigned int i = 0; i < characterArray.size(); i++)
    {
        characterArray[i]->UpdatePosition();
        characterArray[i]->UpdateAnimation();
    }

    //Apply the dynamicLayer to the screen
	ApplyImage(0, 0, dynamicLayer, screen, &screenLocation);

	//Flip the buffers and return false if this fails
	if (SDL_Flip(screen) == -1) {return false;}
    return true;
}

void Graphics::AddCharacter(Character* character)
{
    characterArray.push_back(character);
}

void Graphics::CleanUp()
{
    if (dynamicLayer != NULL)
    {
        SDL_FreeSurface(dynamicLayer);
    }
    SDL_Quit();
}
