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
#include "Event_Manager.h"
#include "Event.h"

#include <string>
#include <sstream>
#include <list>
#include <map>
#include <stdio.h>

Event_Manager::Event_Manager(std::map<int, Character*>* pCharacterMap)
{
    characterMap = pCharacterMap;
}

Event_Manager::~Event_Manager()
{
    while (!eventQueue.empty())
    {
        delete eventQueue.front();
        eventQueue.pop();
    }
}

//Use this version of the AddEvent function, it is safer from a memory perspective
void Event_Manager::AddEvent(std::string eventSerial)
{
	eventQueue.push(Event::Deserialize(eventSerial));
}

//This function is not safe, please avoid using it
void Event_Manager::AddEvent(Event* event)
{
	eventQueue.push(event);
}

//Returns true if there is an event to poll, false otherwise
bool Event_Manager::PeekEvent()
{
    return !eventQueue.empty();
}

//Process the next event in the queue. Returns false if there are no events to poll.
bool Event_Manager::PollEvent()
{
    if (eventQueue.empty())
    {
        return false;
    }
	Event* event = eventQueue.front();
	eventQueue.pop();

	//This is necessary, so that if a character is searched for, but they don't exist, we don't segfault
	std::map<int, Character*>::iterator whoIter = characterMap->find(event->who_id);
	if (whoIter == characterMap->end())
	{
	    if (DEBUG_SHOWALL || DEBUG_SHOWERRORS)
	    {
            printf("Polled an event for a character that does not exist - Character id %i\n", event->who_id);
	    }
	    delete event;
	    return true;
	}

    //Actually handle the event
    if (event->type == "Attack")
    {
		whoIter->second->Attack(event->info);	//These are all special functions that take in serialized data, then deserialize it and
        if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
        {
            printf("Character with id %i (client %i) Attacked!\n", event->who_id, whoIter->second->GetClientId());
        }
    }
    else if (event->type == "Move")
    {
		whoIter->second->Move(event->info);
        if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
        {
            printf("Character with id %i (client %i) Moved!\n", event->who_id, whoIter->second->GetClientId());
        }
    }
    else if (event->type == "StopMove")
    {
        whoIter->second->StopMove(event->info);
        if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
        {
            printf("Character with id %i (client %i) Stopped Moving!\n", event->who_id, whoIter->second->GetClientId());
        }
    }
    else if (event->type == "ChangeTarget")
    {
        std::stringstream inString;
        inString.str(event->info);
        std::string temp = "";
        inString >> temp;   //Should read "New_Target:"
        inString >> temp;   //Should be the new target's id number
        if (temp == "None")
        {
            whoIter->second->ChangeTarget(NULL);
        }
        else
        {
            whoIter->second->ChangeTarget(event->info, characterMap->find(Conversions::StringToInt(temp))->second);
        }
        if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
        {
            printf("Character with id %i (client %i) Changed targets to character with id %s!\n", event->who_id, whoIter->second->GetClientId(),  temp.c_str());
        }
    }
    else if (event->type == "Chat")
    {
//		whoIter->second->Chat(event->info);
        if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
        {
            printf("Character with id %i (client %i) Chatted!\n", event->who_id, whoIter->second->GetClientId());
        }
    }
    else if (event->type == "Defend")
    {
//		whoIter->second->Defend(event->info);
        if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
        {
            printf("Character with id %i (client %i) Defended!\n", event->who_id, whoIter->second->GetClientId());
        }
    }
    else if (event->type == "TakeDamage")
    {
		whoIter->second->TakeDamage(event->info);
        if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
        {
            printf("Character with id %i (client %i) Took Damage!\n", event->who_id, whoIter->second->GetClientId());
        }
    }
    else if (event->type == "UseMagic")
    {
        whoIter->second->UseMagic(event->info);
        if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
        {
            printf("Character with id %i (client %i) Used Magic!\n", event->who_id, whoIter->second->GetClientId());
        }
    }
    else if (event->type == "Die")
    {
//		whoIter->second->Die(event->info);
        if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
        {
            printf("Character with id %i (client %i) Died!\n", event->who_id, whoIter->second->GetClientId());
        }
    }
    else if (event->type == "Spawn")
    {
//		characterMap.insert(characterMap.begin(), std::pair<std::string, Character*>(event->who_id, event->info));
        if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
        {
            printf("Character with id %i (client %i) Spawned!\n", event->who_id, whoIter->second->GetClientId());
        }
    }
    else if (event->type == "Jump")
    {
        whoIter->second->Jump(event->info);
        if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
        {
            printf("Character with id %i (client %i) Jumped!\n", event->who_id, whoIter->second->GetClientId());
        }
    }
    else if (event->type == "StopJump")
    {
        whoIter->second->StopJump(event->info);
        if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
        {
            printf("Character with id %i (client %i) stopped jumping!\n", event->who_id, whoIter->second->GetClientId());
        }
    }
    else
    {
        if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS || DEBUG_SHOWERRORS)
        {
            printf("The event type is unknown - type is %s\n", event->type.c_str());
        }
    }

	delete event;	//This should be the only remaining pointer to this event, and it is deleted here to avoid memory leaks

	return true;
}
