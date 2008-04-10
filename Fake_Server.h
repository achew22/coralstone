#ifndef FAKE_SERVER_H
#define FAKE_SERVER_H

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
#include "Event_Manager.h"
#include "Event.h"
#include "Quest.h"

#include <map>
#include <string>

class Client;

//Returns everything as a string, since everything ought to be serialized. This means that it
//will have to go through the parser before it can be used.
class Fake_Server
{
private:
    World* world;
    Event_Manager* eventManager;
	std::map<int, Client*> clientMap;	//Contains the ids of all of the registered clients
    std::map<int, Quest*> questMap;
public:
    Fake_Server(std::string filename);
    ~Fake_Server();
    bool Update();
    Quest* GetQuest(int id);
    void RegisterEvent(std::string event, int clientId);
	void SendEventToClient(int clientId, std::string event);
    std::string RegisterClient(Client* theClient, int characterId);
};

#endif
