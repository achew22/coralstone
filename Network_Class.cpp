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


#include "Network_Class.h"
#include <SDL/SDL_net.h>
#include <stdio.h>
#include <string>

//Init everything, create
Network_Class::Network_Class()
{
    if (SDLNet_Init() == -1)
    {
        printf("Error with init\n");
        return;
    }

    //We have a set this is used to check to do event based network communication
    set = SDLNet_AllocSocketSet(1);
    if (!set)
    {
        printf("Error with set creation\n");
        return;
    }
};

bool Network_Class::Connect(std::string hostname, int port)
{
    IPaddress ip;
    //Okay this threw me for a loop -- it turns out that this function = 0 when its successful. Why? Basically it's evil
    if (SDLNet_ResolveHost(&ip, hostname.c_str(), port) != 0)
    {
        printf("Error with resolve\n");
        //return false;
    }
    sock = SDLNet_TCP_Open(&ip);
    if (!sock)
    {
        printf("Unable to connect to %s on port %i\n", hostname.c_str(), port);
        //return false;
    }
    if (SDLNet_TCP_AddSocket(set, sock) == -1)
    {
        printf("Error with add socket\n");
        //return false;
    }

    // At this point we have a good connection to the server
    return true;
    /*	else
    	//{
    		std::string response;
    		printf("Connected Successfully");
    		printf("\nServer says: %s\n", Read(sock).c_str());

    		Send (sock, "characterId: 1");
    		printf("\nServer says: %s\n", Read(sock).c_str());
    	//}
    */
}

//If the class has something happening (i.e. data sent to it) this will return true
bool Network_Class::Ready()
{
    // This little trick will return true if we're ready or false if not
    return SDLNet_CheckSockets(set, 1);
}

//Read from the network class return string
std::string Network_Class::Read()
{
    char server_response[5000]; //Hopefully 5000 characters is enough to keep the whole throughput in there. This can be bumped up if necessary
    int count = SDLNet_TCP_Recv( sock, server_response, sizeof( server_response ) );
    if (count)
    {
        return (std::string)server_response;
    }
    //This sucks -- we didn't get any data from the server, give em nothing back
    return "";
}

//Pipe text into the server and return bool
bool Network_Class::Write(std::string message)
{
    message += "\r\n"; // Our messages terminate with \r\n
    return SDLNet_TCP_Send(sock, const_cast<char *>(message.c_str()), message.length() + 1); // this god awful hack is because SDLNET_TCP_Send doesn't accept a constant char * in linux but in windows it works, strange.
}
