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

#include "Conversions.h"

#include <string>
#include <cmath>
#include <sstream>

std::string Conversions::IntToString(int toConvert)
{
    if (toConvert == 0)
    {
        return "0";
    }
    std::string toReturn = "";
    bool negative = false;
    if (toConvert < 0)
    {
        negative = true;
        toConvert *= -1;
    }
    for (int i = 0; pow(10.0, i) < 10*toConvert; i++)
    {
        toReturn = (char)((toConvert%(int)pow(10.0, i+1))/(pow(10,i)) + '0') + toReturn;
        toConvert -= toConvert%(int)pow(10.0, i+1);
    }
    if (negative)
    {
        toReturn = "-" + toReturn;
    }
    return toReturn;
}

int Conversions::StringToInt(std::string toConvert)
{
    int toReturn = 0;
    if (toConvert.size() == 0)
    {
        return 0;
    }
    //If the string is a negative number
    if (toConvert[0] == '-')
    {
        for (unsigned int i = 0; i < toConvert.size() - 1; i++)
        {
            toReturn += (int)((toConvert[toConvert.size()-i-1]-'0')*pow(10.0, i));
        }
        return toReturn * -1;
    }
    //If the string is a positive number
    for (unsigned int i = 0; i < toConvert.size(); i++)
    {
        toReturn += (int)((toConvert[toConvert.size()-i-1]-'0')*pow(10.0, i));
    }
    return toReturn;
}
