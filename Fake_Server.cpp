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

#include "Fake_Server.h"
#include "Event.h"
#include "Client.h"
#include "Event_Manager.h"
#include "constants.h"
#include "Conversions.h"

#include <string>
#include <fstream>
#include <sstream>

Fake_Server::Fake_Server(std::string filename)
{
    std::ifstream inFile;
    inFile.open(filename.c_str());
    std::string line = "";
    getline(inFile, line);
    world = new World(line);
    int i = 0;
    while (!inFile.eof())
    {
        getline(inFile, line);
        Character* newCharacter = new Character(line, NULL);
        world->AddCharacter(newCharacter);
        i++;
    }
    eventManager = new Event_Manager(world->GetCharacterMap());
    inFile.close();
}

Fake_Server::~Fake_Server()
{
    delete eventManager;
    delete world;
}

bool Fake_Server::Update()
{
    //First perform all events
    while (eventManager->PollEvent()) {}

    //Now update the positions of all characters
    for (std::map<int, Character*>::iterator i = world->GetCharacterMap()->begin(); i != world->GetCharacterMap()->end(); i++)
    {
        i->second->UpdatePosition();
    }

    //Maybe more should go here, but I'm not sure

    return true;
}

Quest* Fake_Server::GetQuest(int id)
{
    return questMap[id];
}

void Fake_Server::RegisterEvent(std::string event, int clientId)
{
    eventManager->AddEvent(event);
	Event* eventptr = Event::Deserialize(event);
    if (eventptr->ShouldForward())	//Do not forward events if they should not go from the server to the client
	{
		//Currently, sends the event to every other player in the world. I will fix this when I get the chance
		for (std::map<int, Character*>::iterator i = world->GetCharacterMap()->begin(); i != world->GetCharacterMap()->end(); i++)
		{
		    int tempId = 0;
			if ( clientId != (tempId = i->second->GetClientId() ) && tempId != -1 )
			{
				SendEventToClient(tempId, event);
			}
		}
	}

	delete eventptr;    //Free this memory that was allocated
}

void Fake_Server::SendEventToClient(int clientId, std::string event)
{
    std::map<int, Client*>::iterator i = clientMap.find(clientId);
    if (i != clientMap.end())
    {
        if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
        {
            printf("Sending event from server to client. Event is '%s' and client has id %i\n", event.c_str(), clientId);
        }
        i->second->RegisterEvent(event);
    }
    else
    {
        if (DEBUG_SHOWALL || DEBUG_SHOWERRORS)
        {
            printf("An error occurred: Fake Server tried to send an event to client %i - doesn't exist\n", clientId);
        }
    }
}

std::string Fake_Server::RegisterClient(Client* theClient, int characterId)
{
    if (world->GetCharacter(characterId) == NULL)
    {
        return "FAILED";
    }
    if (world->GetCharacter(characterId)->GetClientId() != -1)
    {
        return "FAILED";
    }
    world->GetCharacter(characterId)->AssignClient(theClient->GetId());

	clientMap.insert(std::pair<int, Client*>(theClient->GetId(), theClient));

    std::string toSend = "";	//toSend will contain all of the information that needs to be sent to the client:
		//This includes the characters around the client, the area information, the map information, and maybe more
	//Currently, this just sends info on all characters that exist at this point in time
	toSend += "Map:\n";
	if (world->GetArea(world->GetCharacter(characterId)->GetAreaId()) != NULL)
	{
	    toSend += world->GetArea(world->GetCharacter(characterId)->GetAreaId())->GetMapInfo();
	}
	else
	{
	    if (DEBUG_SHOWALL || DEBUG_SHOWERRORS)
	    {
	        printf("There was an error, tried to access an area whose id did not exist. Id was %i\n", world->GetCharacter(characterId)->GetAreaId());
	    }
	    toSend += "Error\nError";
	}
	toSend += "\nEND_\n";
	toSend += "Characters:\n";
    for (std::map<int, Character*>::iterator i = world->GetCharacterMap()->begin(); i != world->GetCharacterMap()->end(); i++)
    {
        toSend += i->second->Serialize() + "\n";
    }
    toSend += "END_";
    return toSend;
}
