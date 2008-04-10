#ifndef ANIMATION_H
#define ANIMATION_H

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

#include "constants.h"
#include "Sprite_Sheet.h"

#include <vector>
#include <SDL/SDL.h>
#include <string>
//Defines the Animation Class. An animation class is just a convenient way to store and manage
//a series of clips.
class Animation
{
private:
    Sprite_Sheet* spriteSheet;  //A pointer to the Sprite_Sheet class instance containing
        //all of the sprites for this animation
    std::vector<int> sprites;
    std::vector<int>::iterator currentSprite;
    int lastAnimTime;
    int clipLength;
public:
	Animation(std::string file, int spriteW, int spriteH, std::vector<int> spriteIndexes, int clipTime = 120,
        int red = 0, int green = 255, int blue = 255);  //Pass in the name
        //of the file containing the sprites for this animation, a vector of which sprites belong
        //in the animation (in order), the width and height of the sprites, the time between sprites (in ms),
        //and the color of the alpha mask
	void ApplyCurrentSprite(int x, int y, SDL_Surface* destination);
	void ApplyFirstSprite(int x, int y, SDL_Surface* destination);
	void ApplyLastSprite(int x, int y, SDL_Surface* destination);
	void Begin();
};

#endif
