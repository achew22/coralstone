#ifndef INCLUDE_H
#define INCLUDE_H

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

//Debug options for choosing whether or not to show some debug messages

#include <string>

const bool DEBUG_SHOWALL = false;
const bool DEBUG_SHOWFPS = true;
const bool DEBUG_SHOWEVENTS = false;
const bool DEBUG_SHOWERRORS = true;

//Globals.
static const int SCREEN_WIDTH = 500;	//Screen width
static const int SCREEN_HEIGHT = 500;	//Screen height
static const int SCREEN_BPP = 32;	//Screen BPP
const int PORT = 2000;  //Port
const std::string ADDRESS = "138.86.201.218"; //host address

const int CHARACTER_WIDTH = 48;
const int CHARACTER_HEIGHT = 64;

#endif
