#ifndef SECTOR_H
#define SECTOR_H

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

class Character;    //Forward declaration to avoid declaration rings because the Sector class needs to know the Character class,
    //but the Character class needs to know the Sector class, so neither can be made...

#include <vector>
#include <map>
#include <string>

class Sector
{
private:
    std::map<int, Character*> characterMap;
public:
    Sector();
    void MoveChar(int id, Sector* destination);
};

#endif
