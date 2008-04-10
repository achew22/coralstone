#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

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
#include <vector>
#include <list>
#include <string>
#include "Point.h"

//A class to handle sprite sheets. Note: all of the sprites in the sheet must be the same size.
//By setting this up when the area is loaded (which should take under a second), we save a lot
//of blitting time between frames (which is where it counts).
class Sprite_Sheet
{
private:
    std::vector<SDL_Surface*> sprites;
    static std::list<Sprite_Sheet*> spriteSheetList;
    std::string filename;
    Point spriteDim;
    int numUsers;
public:
    Sprite_Sheet(int wOfSprite, int hOfSprite, std::string file, int red = 0, int green = 255, int blue = 255);
    ~Sprite_Sheet();    //The destructor
    void ApplySprite(int x, int y, int spriteNum, SDL_Surface* destination);    //Apply the sprite at index spriteNum
        //to the surface destination at the coordinates x, y
    bool operator ==(Sprite_Sheet* compare); //The obvious, compare two Sprite_Sheets
    Point GetSpriteDimension(); //Returns the width and height of the sprites
    void AddUser();     //Used in conjunction with the function below
    void RemoveUser();  //This function is for memory management. This particular instance of spriteSheet will always
        //keep track of how many users are using it. If that number drops to zero, it deletes itself.
    static Sprite_Sheet* FindSheet(std::string file);   //Returns a pointer to the spriteSheet that was already
        //created with the file specified by "file". If none yet exists, returns NULL. Use this to prevent creating
        //multiple identical Sprite_Sheets, thereby wasting memory.
    static void CleanUp();  //Call when you are done with the program, deletes all Sprite_Sheets stored in
        //spriteSheetList
    static void PrintList(); //A debugging function to print out spriteSheetList
};

#endif
