#ifndef EVENT_H
#define EVENT_H

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

#include <string>
#include <list>

struct Event
{
public:
	std::string type;
	int who_id;
	std::string info;	//This info is a serialized version of the information to be passed with the function that ought to be called by the event

	Event();
	Event(std::string eventType, int eventWho_id, std::string eventInfo);
	static std::string Serialize(Event* event);
    static std::string Serialize(std::string eventType, int eventWho_id, std::string eventInfo);
	static Event* Deserialize(std::string eventSerial);
    bool ShouldForward();    //All events should be send client->server, but most need not be forwarded back from server->client
};

#endif
