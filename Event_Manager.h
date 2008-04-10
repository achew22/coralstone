#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

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

#include <string>
#include <queue>
#include <map>

class Event_Manager
{
private:
    std::queue<Event*> eventQueue;
    std::map<int, Character*>* characterMap;    //This is only a pointer to a character map because it needs to use an up-to-the-ms
        //version when Polling events. Thus, the pointer.
public:
    Event_Manager(std::map<int, Character*>* pCharacterMap);
    ~Event_Manager();
    void AddEvent(std::string eventSerial);
	void AddEvent(Event* event);
	bool PeekEvent();
	bool PollEvent();
};

#endif