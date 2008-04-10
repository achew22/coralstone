#ifndef NETWORK_CLASS_INCLUDED
#define NETWORK_CLASS_INCLUDED

/*
Copyright 2007, 2008 Andrew Allen and Brian Shourd

This file is part of Coralstone.

AutoRPG is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

AutoRPG is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with AutoRPG (Called LICENSE.txt).  If not, see
<http://www.gnu.org/licenses/>.
*/

#include <SDL/SDL_net.h>
#include <string>

class Network_Class
{
    public:
        Network_Class();
        bool Connect(std::string, int);
        bool Write(std::string);
        bool Ready();
        std::string Read();
    private:
        TCPsocket sock;
        SDLNet_SocketSet set;
};

#endif // NETWORK_CLASS_INCLUDED
