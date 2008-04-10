#ifndef METER_H
#define METER_H

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

class Meter
{
private:
    int percent;    //This should be a number between 1 and 100
    int width;
    int height;
    int red;
    int blue;
    int green;

    SDL_Surface* source;
public:
    Meter();
    Meter(int theWidth, int theHeight, int colorRed, int colorGreen, int colorBlue);
    ~Meter();
    void SetPercent(int newPercent);
    void ApplyToSurface(int x, int y, SDL_Surface* theSurface);
};

#endif
