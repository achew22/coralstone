#ifndef MAP_H
#define MAP_H

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

#include <vector>
#include <string>
#include "Sprite_Sheet.h"
#include "Point.h"

class Map
{
private:
    std::vector< std::vector<int> > spriteIds;
    Sprite_Sheet* spriteSheet;
    Point spriteDim;
    std::string indexFile;
    std::string picFile;
public:
    Map(std::string mapFile, std::string pictureFile);
    ~Map();
    void LoadFiles(std::string mapFile, std::string pictureFile);
    void ApplyMap(int x, int y, int h, int w, SDL_Surface* destination);
    std::string GetMapInfo();
    Point GetDim(); //Returns the dimensions of the map, in terms of how many sprites tall by how many sprites wide
};

#endif
