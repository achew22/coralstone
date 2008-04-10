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
#include "Graphics.h"
#include "Meter.h"

Meter::Meter()
{
    width = 0;
    height = 0;
    red = 0;
    blue = 0;
    green = 0;

    source = NULL;
}

Meter::Meter(int theWidth, int theHeight, int colorRed, int colorGreen, int colorBlue)
{
    width = theWidth;
    height = theHeight;
    red = colorRed;
    green = colorGreen;
    blue = colorBlue;

    source = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, SCREEN_BPP, NULL, NULL, NULL, NULL);
}

Meter::~Meter()
{
    if (source != NULL)
    {
        SDL_FreeSurface(source);
    }
}

void Meter::SetPercent(int newPercent)
{
    percent = newPercent;
}

void Meter::ApplyToSurface(int x, int y, SDL_Surface* theSurface)
{
    //First, update it so that it looks right
    if (source == NULL)
    {
        return;
    }
    SDL_FillRect(source, NULL, SDL_MapRGB(source->format, red/2.0, green/2.0, blue/2.0));
    SDL_Rect clip;
    clip.x = 0;
    clip.y = 0;
    clip.w = (percent/100.0) * width;
    clip.h = height;
    SDL_FillRect(source, &clip, SDL_MapRGB(source->format, red, green, blue));

    //Now apply it to the surface
    Graphics::ApplyImage(x, y, source, theSurface);
}
