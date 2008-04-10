#ifndef CLIENT_H
#define CLIENT_H

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

#include "Event_Manager.h"
#include "Character.h"
#include "Sector.h"
#include "Event.h"
#include "Area.h"
#include "Fake_Server.h"
#include "Graphics.h"
#include "Network_Class.h"

#include <SDL/SDL.h>
#include <map>
#include <string>
#include <vector>

class Client
{
private:
    int id;
    Network_Class* networkClass;
    Event_Manager* eventManager;
    std::map<int, Character*> characterMap;
    Fake_Server* server;
    Character* player;

//    SDL_Surface* screen;
//    SDL_Surface* dynamicLayer;
    Map* map;

    Graphics* graphics;

    //Key mappings
    SDLKey moveUp;
    SDLKey moveDown;
    SDLKey moveLeft;
    SDLKey moveRight;
    SDLKey attack;
    SDLKey changeTarget;
public:
    Client(Fake_Server* theServer, int clientId, /*SDL_Surface* theScreen, SDL_Surface* theDynamicLayer,*/ Graphics* theGraphics);
    ~Client();
    void LoadMap(std::string indexFile, std::string tileFile);
    bool Update();
    bool UpdateEvents();
    bool UpdatePositions();
    void SetKeys(SDLKey keyUp, SDLKey keyDown, SDLKey keyLeft, SDLKey keyRight, SDLKey keyAttack, SDLKey keyChangeTarget);
	int GetId();
	Character* GetPlayer();
    bool Connect(int characterId);  //Connects to the server using character with id characterId
    bool ConnectRemote(int characterId);    //Connect remotely
	void RegisterEvent(std::string event);  //Let the Client know that an event has occurred
	void SendEventToServer(std::string event);  //Send a serialized event to server - often occurs in tandem with the function above
	bool SendEventToRemoteServer(std::string event);
    bool PollEvent();   //Process the next event in eventManager. Returns false if there are no more events to poll
    bool PeekEvent();   //Return whether or not there is an event to Poll
    void HandleInput(SDL_Event SDLEvent);   //Handle the input from SDL, such as keypresses and mouse clicks
};

#endif
