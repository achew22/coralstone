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
#include "Character.h"
#include "Graphics.h"
#include "Conversions.h"
#include "Fake_Server.h"
#include "Client.h"

#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_net.h>

SDLNet_SocketSet set;
TCPsocket sock;

bool Init();

int main(int argc, char* argv[])
{
    if (!Init()) {return 1;}
    Graphics graphics;
   	if (!graphics.Init()) {printf("Init failed\n"); return 1;}

    Fake_Server server("server/server1.txt");
    Client client1(&server, 1, &graphics);
    graphics.SetPlayer(client1.GetPlayer());
    client1.LoadMap("maps/map1.txt", "maps/map1.gif");
    Client client2(&server, 2, NULL);
    if (!client1.Connect(1)) {return 1;}
    if (!client2.Connect(2)) {return 1;}
    client2.SetKeys(SDLK_w, SDLK_s, SDLK_a, SDLK_d, SDLK_q, SDLK_e);

    //This area reserved for testing experiments
    //client1.ConnectRemote(1);

	SDL_Event SDLEvent; //The main event for polling and what-not
    bool quit = false;
	while (quit != true)
	{
		if (SDL_PollEvent(&SDLEvent))
		{
		    client1.HandleInput(SDLEvent);
		    client2.HandleInput(SDLEvent);
			if (SDLEvent.type == SDL_KEYDOWN)
			{
				switch (SDLEvent.key.keysym.sym)
				{
				case SDLK_ESCAPE:   //Escape pressed
					quit = true;
					break;
                default:
                    break;
				}
			}
			else if (SDLEvent.type == SDL_QUIT)
			{
				quit = true;    //If the person "X"'s out of the window
			}
		}
		if (!(client2.UpdateEvents() && client2.UpdatePositions())) {return 1;}      //Update
		if (!client1.Update()) {return 1;}      //Update
		if (!server.Update()) {return 1;}       //Update
		if (!graphics.Update()) {return 1;}     //Update
	}

	graphics.CleanUp();
	Sprite_Sheet::CleanUp();

	return 0;
}

bool Init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {return false;}
    SDL_WM_SetCaption("Coralstone - Development", NULL);
    return true;
}
