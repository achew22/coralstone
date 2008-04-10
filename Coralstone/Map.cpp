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

#include "Map.h"
#include "Point.h"
#include "Conversions.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

Map::Map(std::string mapFile, std::string pictureFile)
{
    indexFile = mapFile;
    picFile = pictureFile;

    //Read in the indexes from mapFile
    std::fstream fileIn(mapFile.c_str());
    std::string line = "";   //the current line
    getline(fileIn, line);
    std::stringstream stringIn;
    stringIn.str(line);
    std::string temp = "";
    stringIn >> temp;
    spriteDim.x = Conversions::StringToInt(temp);
    stringIn >> temp;
    spriteDim.y = Conversions::StringToInt(temp);

    while (!fileIn.eof())
    {
        getline(fileIn, line);
        line += " END__";
        std::string buffer = "";
        std::stringstream stringIn;
        stringIn.str(line);
        std::vector<int> tempLine;  //Holds a line of indexes from the mapfile
        stringIn >> buffer;
        while (buffer != "END__")
        {
            tempLine.push_back(Conversions::StringToInt(buffer));
            stringIn >> buffer;
        }
        spriteIds.push_back(tempLine);
    }
    fileIn.close();

    //Get the spriteSheet from picturefile
    spriteSheet = Sprite_Sheet::FindSheet(pictureFile); //First, attempt to find this spriteSheet if it has already been
        //created in memory. If not, returns NULL
    if (spriteSheet == NULL)
    {
        spriteSheet = new Sprite_Sheet(spriteDim.x, spriteDim.y, pictureFile);
    }
    else
    {
        spriteSheet->AddUser(); //This function is for memory management. This particular instance of spriteSheet will always
            //keep track of how many users are using it. If that number drops to zero, it removes itself.
    }
}

Map::~Map()
{
    if (spriteSheet != NULL)
    {
        spriteSheet->RemoveUser();  //This function is for memory management. This particular instance of spriteSheet will always
            //keep track of how many users are using it. If that number drops to zero, it deletes itself.
    }
}

void Map::LoadFiles(std::string mapFile, std::string pictureFile)
{
    //First, get rid of all of the stuff that was already created
    if (spriteSheet != NULL)
    {
        spriteSheet->RemoveUser();  //This function is for memory management. This particular instance of spriteSheet will always
            //keep track of how many users are using it. If that number drops to zero, it deletes itself.
    }
    for (unsigned int i = 0; i < spriteIds.size(); i++)
    {
        spriteIds[i].resize(0); //Delete all of the current indexes
    }
    spriteIds.resize(0);

    indexFile = mapFile;
    picFile = pictureFile;

    //Now load up the new files
    std::fstream fileIn(mapFile.c_str());
    std::string line = "";   //the current line
    getline(fileIn, line);
    std::stringstream stringIn;
    stringIn.str(line);
    std::string temp = "";
    stringIn >> temp;
    spriteDim.x = Conversions::StringToInt(temp);
    stringIn >> temp;
    spriteDim.y = Conversions::StringToInt(temp);

    while (!fileIn.eof())
    {
        getline(fileIn, line);
        line += " END__";   //So we know when to stop easily
        std::string buffer = "";
        std::stringstream stringIn;
        stringIn.str(line);
        std::vector<int> tempLine;  //Holds a line of indexes from the mapfile
        stringIn >> buffer;
        while (buffer != "END__")
        {
            tempLine.push_back(Conversions::StringToInt(buffer));
            stringIn >> buffer;
        }
        spriteIds.push_back(tempLine);
    }
    fileIn.close();

    spriteSheet = Sprite_Sheet::FindSheet(pictureFile); //First, attempt to find this spriteSheet if it has already been
        //created in memory. If not, returns NULL
    if (spriteSheet == NULL)
    {
        spriteSheet = new Sprite_Sheet(spriteDim.x, spriteDim.y, pictureFile);
    }
    else
    {
        spriteSheet->AddUser(); //This function is for memory management. This particular instance of spriteSheet will always
            //keep track of how many users are using it. If that number drops to zero, it removes itself.
    }
}

void Map::ApplyMap(int x, int y, int w, int h, SDL_Surface* destination)
{
    if (spriteSheet == NULL)
    {
        return;
    }
    Point start, numberTiles;
    start.x = (int)(x/spriteDim.x);
    start.y = (int)(y/spriteDim.y);
    numberTiles.x = (int)(w/spriteDim.x) + 1;
    numberTiles.y = (int)(h/spriteDim.y) + 1;
    if ((start.x + numberTiles.x >= spriteIds.size()) || (start.y + numberTiles.y >= spriteIds[0].size()))
    {
        return;
    }
    for (int i = start.x * spriteDim.x; i <= (start.x + numberTiles.x) * spriteDim.x; i += spriteDim.x)
    {
        for (int j = start.y * spriteDim.y; j <= (start.y + numberTiles.y) * spriteDim.y; j += spriteDim.y)
        {
            if ((i >= 0) && (j >= 0))
            {
                spriteSheet->ApplySprite(i, j, spriteIds[i/spriteDim.x][j/spriteDim.y], destination);
            }
        }
    }
}

std::string Map::GetMapInfo()
{
    return indexFile + "\n" + picFile;
}

//Returns the dimensions of the map, in terms of how many sprites tall by how many sprites wide
Point Map::GetDim()
{
    return Point(spriteIds.size(), spriteIds[0].size());
}
