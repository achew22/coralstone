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

#include "Sprite_Sheet.h"
#include "constants.h"
#include "Point.h"
#include "Graphics.h"

#include <string>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

std::list<Sprite_Sheet*> Sprite_Sheet::spriteSheetList;

Sprite_Sheet::Sprite_Sheet(int wOfSprite, int hOfSprite, std::string file, int red, int green, int blue)
{
    filename = file;

    //Loads the file to the surface screen, with the alphamask indicated by red, green, and blue
    SDL_Surface *loadedImage = NULL;
	SDL_Surface *sheet = NULL;

	loadedImage = IMG_Load(filename.c_str());
	if (loadedImage != NULL)
	{
		sheet = SDL_DisplayFormat(loadedImage);
		SDL_FreeSurface(loadedImage);   //Free this memory
	}

    if (sheet == NULL && (DEBUG_SHOWALL || DEBUG_SHOWERRORS))
    {
        printf("Sheet is not initialized for some reason\n");
    }

    int numx = (sheet->w)/wOfSprite;  //Number of sprites in the x direction
    int numy = (sheet->h)/hOfSprite;  //Number of sprites in the y direction

    //This part sets up the vector of sprites to empty surfaces
    sprites.resize(numx*numy);
    for (int i = 0; i < numx*numy; i++)
    {
        sprites[i] = SDL_CreateRGBSurface(SDL_HWSURFACE, wOfSprite, hOfSprite, SCREEN_BPP, NULL, NULL, NULL, NULL);
    }
    SDL_Rect clip;
    clip.w = wOfSprite;
    clip.h = hOfSprite;

    //Now set every one of those surfaces to be the individual sprite, with the proper alpha
    for (int i = 0; i < numy; i++)
    {
        clip.y = i * hOfSprite;
        for (int j = 0; j < numx; j++)
        {
            clip.x = j * wOfSprite;
            SDL_SetColorKey(sprites[i*numx + j], SDL_SRCCOLORKEY, SDL_MapRGB(sprites[i*numx + j]->format, red, green, blue));

            SDL_BlitSurface(sheet, &clip, sprites[i*numx + j], NULL);
        }
    }

    spriteDim.x = wOfSprite;    //Set the spriteDimentions
    spriteDim.y = hOfSprite;

    SDL_FreeSurface(sheet); //Free this memory

    spriteSheetList.push_back(this);

    numUsers = 1;   //We are assuming that this is being created for use by ONE class at this time. Use AddUser() to add more.
}

Sprite_Sheet::~Sprite_Sheet()
{
    for (unsigned int i = 0; i < sprites.size(); i++)
    {
        if (sprites[i] != NULL)
        {
            SDL_FreeSurface(sprites[i]);
        }
    }
    spriteSheetList.remove(this);
}

void Sprite_Sheet::ApplySprite(int x, int y, int spriteNum, SDL_Surface* destination)
{
    if ((unsigned int)spriteNum >= sprites.size() || spriteNum < 0)
    {
        if (DEBUG_SHOWALL || DEBUG_SHOWERRORS)
        {
            printf("ApplySprite had a sprite out of range\n");
        }
        return;
    }
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(sprites[spriteNum], NULL, destination, &offset);
}

bool Sprite_Sheet::operator ==(Sprite_Sheet* compare)
{
    return (filename == compare->filename);
}

Point Sprite_Sheet::GetSpriteDimension()
{
    return spriteDim;
}

//Used for memory management in conjunction with the function below
void Sprite_Sheet::AddUser()
{
    numUsers++;
}

//This function is for memory management. This particular instance of spriteSheet will always
//keep track of how many users are using it. If that number drops to zero, it deletes itself.
void Sprite_Sheet::RemoveUser()
{
    numUsers--;
    if (numUsers <= 0)
    {
        delete this;
    }
}

Sprite_Sheet* Sprite_Sheet::FindSheet(std::string file)
{
    for (std::list<Sprite_Sheet*>::iterator i = spriteSheetList.begin(); i != spriteSheetList.end(); i++)
    {
        if ((*i)->filename == file)
        {
            return (*i);
        }
    }
    return NULL;
}

void Sprite_Sheet::CleanUp()
{
    for (std::list<Sprite_Sheet*>::iterator i = spriteSheetList.begin(); i != spriteSheetList.end(); i++)
    {
        if ((*i) != NULL)
        {
            delete (*i);
            i = spriteSheetList.begin();
        }
    }
}

void Sprite_Sheet::PrintList()
{
    int j = 0;
    printf("\nPrinting spriteList:\n");
    for (std::list<Sprite_Sheet*>::iterator i = spriteSheetList.begin(); i != spriteSheetList.end(); i++)
    {
        printf("    List value %i is %i\n", j, (int)(*i));
        j++;
    }
}
