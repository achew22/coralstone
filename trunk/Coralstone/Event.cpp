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

#include "Event.h"
#include "Character.h"
#include "Conversions.h"
#include "constants.h"

#include <string>
#include <list>
#include <sstream>
#include <stdio.h>

Event::Event()
{
	type = "";
	who_id = 0;
	info = "";
}

Event::Event(std::string eventType, int eventWho_id, std::string eventInfo)
{
	type = eventType;
	who_id = eventWho_id;
	info = eventInfo;
}

//Turns an event into a string for transportation
std::string Event::Serialize(Event* event)
{
	std::string serial = "";
    serial += "EventType: " + event->type + "\n";
    serial += "CharacterId: " + Conversions::IntToString(event->who_id) + "\n";
    serial += "Info: " + event->info;
	return serial;
}

//This is the preferred method of using serialize, though it isn't too important
std::string Event::Serialize(std::string eventType, int eventWho_id, std::string eventInfo)
{
    std::string serial = "";
    serial += "EventType: " + eventType + "\n";
    serial += "CharacterId: " + Conversions::IntToString(eventWho_id) + "\n";
    serial += "Info: " + eventInfo;
    return serial;
}

//Note: Whenever this function is called, it dynamically allocates memory, and that memory must be deleted
//The event looks like this: "EventType: type\nCharacterId: who_id\nInfo: info" Or equavalently:
//EventType: type
//CharacterId: who_id
//Info: info
//The '\n' is put in between each so that a simple getline() call will work, especially since info may be especially large
Event* Event::Deserialize(std::string eventSerial)
{
	Event* event = new Event();
	std::string temp;
	std::stringstream inStream;
	inStream.str(eventSerial);
	inStream >> temp;   //Pull in 'EventType:'
	inStream.get();      //Get the " "
	getline(inStream, temp);    //Pull in 'type'
	inStream.get();     //Get the '\n'
	event->type = temp;
	inStream >> temp;   //Pull in 'CharacterId:'
	inStream.get();      //Get the " "
	getline(inStream, temp);   //Pull in 'who_id'
	inStream.get();     //Get the '\n'
	event->who_id = Conversions::StringToInt(temp);
	inStream >> temp;   //Pull in 'Info:'
	inStream.get();      //Get the " "
	getline(inStream, temp);    //Pull in 'info'
	event->info = temp;
	return event;
}

bool Event::ShouldForward()
{
    //This needs to be changed so that only the right events are forwarded
    return true;
}
