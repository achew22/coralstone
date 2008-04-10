#ifndef GRAPHICS_H
#define GRAPHICS_H

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

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <string>

#include "Character.h"
#include "Map.h"

//Should contain everything that has graphics that need to be updated. Also, whenever Update()
    //is called, it should update those things, at least graphics-wise.
    //Note that Graphics only contains pointers to these things, and does not declare any
    //memory dynamically, except for screen and dynamicLayer, which are both taken care of in CleanUp()
    //as well as the destructor.
class Graphics
{
private:
    SDL_Surface* screen;    //The main screen for everything!
    SDL_Surface* dynamicLayer;  //The dynamicLayer
    Character* player;  //The player
    Map* map;   //The current map
    std::vector<Character*> characterArray; //Sorted by vertical position of character
	double fpsTicks, fpsFrames, lastTime; //Used for calculating framerate
public:
    Graphics();
    ~Graphics();
    static void ApplyImage(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip = NULL);
		//Apply an image at from the source to the destination, at the position x and y, and with the clip defined in clip (which defaults to the entire surface)
	static SDL_Surface* LoadImage(std::string filename, int red = 0, int green = 255, int blue = 255);
    bool Init();	//Initialize the whole graphics layer
    void SetPlayer(Character* thePlayer);
    void SetMap(Map* theMap);
	SDL_Surface* GetScreen();
	SDL_Surface* GetDynamicLayer();
	bool Update();
    void AddCharacter(Character* character);
	void CleanUp();
};

#endif
