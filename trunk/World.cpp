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

#include "World.h"
#include "Area.h"

#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <stdio.h>

World::World(std::string filename)
{
    file = filename;

    std::ifstream fileIn(filename.c_str());
    std::string line = "";   //the current line
    getline(fileIn, line);  //Should read "Areas:"
    getline(fileIn, line);

    while (!fileIn.eof())
    {
        Area* tempArea = new Area(line);
        areaMap.insert(std::pair<int, Area*>(tempArea->GetId(), tempArea));
        getline(fileIn, line);
    }

    fileIn.close();
}

World::~World()
{
    for (std::map<int, Character*>::iterator i = characterMap.begin(); i != characterMap.end(); i++)
    {
        delete i->second;
    }
    for (std::map<int, Area*>::iterator i = areaMap.begin(); i != areaMap.end(); i++)
    {
        delete i->second;
    }
}

void World::AddCharacter(Character* character)
{
    characterMap.insert(std::pair<int, Character*>(character->GetId(), character));
}

void World::RemoveCharacter(int id)
{
    characterMap.erase(id);
}

Character* World::GetCharacter(int id)
{
    std::map<int, Character*>::iterator i = characterMap.find(id);
    if (i != characterMap.end())
    {
        return i->second;
    }
    else
    {
        return NULL;
    }
}

std::map<int, Character*>* World::GetCharacterMap()
{
    return &characterMap;
}

Area* World::GetArea(int id)
{
    std::map<int, Area*>::iterator i = areaMap.find(id);
    if (i != areaMap.end())
    {
        return i->second;
    }
    else
    {
        return NULL;
    }
}
