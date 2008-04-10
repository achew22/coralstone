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

#include "Animation.h"

#include <vector>
#include <SDL/SDL.h>
#include <math.h>

//The constructor. Needs the name of the file from which to get the sprites, the width and height of the sprites, the
//indexes of all of the sprites (ie which sprites to use in which order), the clip time, and the color for the alpha mask
Animation::Animation(std::string file, int spriteW, int spriteH, std::vector<int> spriteIndexes, int clipTime /*default: 120*/,
    int red /*default: 0*/, int green /*default: 255*/, int blue /*default: 255*/)
{
    spriteSheet = Sprite_Sheet::FindSheet(file);    //See if this animation already exists in loaded memory
    if (spriteSheet == NULL)    //If it doesn't, load up a new one
    {
        spriteSheet = new Sprite_Sheet(spriteW, spriteH, file, red, green, blue);
    }

    sprites.resize(spriteIndexes.size());
    for (unsigned int i = 0; i < sprites.size(); i++)
    {
        sprites[i] = spriteIndexes[i];
    }
    clipLength = clipTime;
    currentSprite = sprites.begin();
    lastAnimTime = SDL_GetTicks();
}

//Applys whatever the current sprite of the animation is to the surface in destination at the coordinates x, y
void Animation::ApplyCurrentSprite(int x, int y, SDL_Surface* destination)
{
    if (SDL_GetTicks() - lastAnimTime > (unsigned int)clipLength) //If enough time has passed
    {
        currentSprite ++;   //Go to the next sprite
        if (currentSprite == sprites.end()) //If you are at the end, go back to the beginning
        {
            currentSprite = sprites.begin();
        }
        lastAnimTime = SDL_GetTicks();
    }
    spriteSheet->ApplySprite(x, y, (*currentSprite), destination);  //Apply sprite
}

//What it says
void Animation::ApplyFirstSprite(int x, int y, SDL_Surface* destination)
{
    spriteSheet->ApplySprite(x, y, (*(sprites.begin())), destination);
}

//What it says
void Animation::ApplyLastSprite(int x, int y, SDL_Surface* destination)
{
    spriteSheet->ApplySprite(x, y, (*(sprites.rbegin())), destination);
}

//Begin the animation (ie reset the animation to the beginning
void Animation::Begin()
{
    lastAnimTime = SDL_GetTicks();
    currentSprite = sprites.begin();
}
