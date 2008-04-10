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

#include "Client.h"
#include "Graphics.h"
#include "Conversions.h"

#include <string>
#include <sstream>
#include <SDL/SDL.h>

Client::Client(Fake_Server* theServer, int clientId, Graphics* theGraphics)
{
    id = clientId;
    server = theServer;
    eventManager = new Event_Manager(&characterMap);
    player = NULL;
    networkClass = new Network_Class();

    map = NULL;

    graphics = theGraphics;

    moveUp = SDLK_UP;
    moveDown = SDLK_DOWN;
    moveLeft = SDLK_LEFT;
    moveRight = SDLK_RIGHT;
    attack = SDLK_SPACE;
    changeTarget = SDLK_TAB;
}

Client::~Client()
{
    SendEventToRemoteServer("exit");
    delete eventManager;
    delete networkClass;
    if (map != NULL)
    {
        delete map;
    }
}

void Client::LoadMap(std::string indexFile, std::string tileFile)
{
    if (map != NULL)
    {
        delete map;
    }
    map = new Map(indexFile, tileFile);
    //map->AssignSpriteSheet(50, 50, tileFile);
}

bool Client::Update()
{
    return UpdateEvents() && UpdatePositions();
}

//This function will only handle events coming in and going out. Seperation is needed to determine the difference between
    //the clients that may not need to update graphics (ie testing multiple clients on one computer). Basically, except for
    //testing purposes, just use Update()
bool Client::UpdateEvents()
{
    //First, send all of the events that the character did to the server
    while (player->PeekEvent())
    {
        std::string event = player->PollEvent();
        if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
        {
            printf("Client %i sent an event with string '%s' to the server!\n", id, event.c_str());
        }
        SendEventToServer(event);
    }

    //Now process all of the events that the server sent to the client
    while (PeekEvent())
    {
        if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
        {
            printf("Client %i is polling an event that occurred\n", id);
        }
        PollEvent();
    }

    return true;
}

//Only update the positions of all of the characters. THis is used for testing purposes only, if you wish
    //to perform an update of the client, use Update() instead.
    //Used in conjunction with UpdateEvents()
bool Client::UpdatePositions()
{
    for (std::map<int, Character*>::iterator i = characterMap.begin(); i != characterMap.end(); i++)
    {
        i->second->UpdatePosition();
    }
    return true;
}

void Client::SetKeys(SDLKey keyUp, SDLKey keyDown, SDLKey keyLeft, SDLKey keyRight, SDLKey keyAttack, SDLKey keyChangeTarget)
{
    moveUp = keyUp;
    moveDown = keyDown;
    moveLeft = keyLeft;
    moveRight = keyRight;
    attack = keyAttack;
    changeTarget = keyChangeTarget;
}

int Client::GetId()
{
	return id;
}

Character* Client::GetPlayer()
{
    return player;
}

//Attempt to connect to the server and take control of the character with id characterId
bool Client::Connect(int characterId)
{
    std::string setupString = server->RegisterClient(this, characterId);
    if (setupString == "FAILED")
    {
        return false;
    }
    std::stringstream setupStream;
    setupStream.str(setupString);
    std::string line = "", mapfile = "", picfile = "";
    getline(setupStream, line); //Should read 'Map:'
    getline(setupStream, mapfile); //Should read 'mapfile.txt'
    getline(setupStream, picfile); //Should read 'picturefile.png'
    getline(setupStream, line); //Should read 'END_'
    map = new Map(mapfile, picfile);
    if (graphics != NULL)
    {
        graphics->SetMap(map);
    }
    getline(setupStream, line); //Shoud read 'Characters:'
    getline(setupStream, line); //Should be the serialized version of the first character
    SDL_Surface* dynamicLayer = NULL;
    if (graphics != NULL)
    {
        dynamicLayer = graphics->GetDynamicLayer();
    }
    while (line != "END_")
    {
        Character* tempChar = new Character(line, dynamicLayer);
        characterMap.insert(std::pair<int, Character*>(tempChar->GetId(), tempChar));
        if (graphics != NULL)
        {
            graphics->AddCharacter(tempChar);
        }
        getline(setupStream, line);
    }
    player = characterMap.find(characterId)->second;
    player->AssignClient(id);
    if (graphics != NULL)
    {
        graphics->SetPlayer(player);
    }
    return true;
}

bool Client::ConnectRemote(int characterId)
{
    printf("Attempting to connect\n");
    networkClass->Connect(ADDRESS, PORT);
    while (!networkClass->Ready()) {}   //Wait
    printf("%s\n", networkClass->Read().c_str());
    networkClass->Write("characterId: " + Conversions::IntToString(characterId));
    while (!networkClass->Ready()) {}   //Wait

    /*std::string setupString = networkClass->Read();
    if (setupString == "FAILED")
    {
        return false;
    }
    std::stringstream setupStream;
    setupStream.str(setupString);
    std::string line = "", mapfile = "", picfile = "";
    getline(setupStream, line); //Should read 'Map:'
    getline(setupStream, mapfile); //Should read 'mapfile.txt'
    getline(setupStream, picfile); //Should read 'picturefile.png'
    getline(setupStream, line); //Should read 'END_'
    map = new Map(mapfile, picfile);
    if (graphics != NULL)
    {
        graphics->SetMap(map);
    }
    getline(setupStream, line); //Shoud read 'Characters:'
    getline(setupStream, line); //Should be the serialized version of the first character
    SDL_Surface* dynamicLayer = NULL;
    if (graphics != NULL)
    {
        dynamicLayer = graphics->GetDynamicLayer();
    }
    while (line != "END_")
    {
        Character* tempChar = new Character(line, dynamicLayer);
        characterMap.insert(std::pair<int, Character*>(tempChar->GetId(), tempChar));
        if (graphics != NULL)
        {
            graphics->AddCharacter(tempChar);
        }
        getline(setupStream, line);
    }
    player = characterMap.find(characterId)->second;
    player->AssignClient(id);
    if (graphics != NULL)
    {
        graphics->SetPlayer(player);
    }*/

    printf("Client recieved string!\n%s\n", networkClass->Read().c_str());

    return true;
}

void Client::RegisterEvent(std::string event)
{
	eventManager->AddEvent(event);
}

void Client::SendEventToServer(std::string event)
{
    server->RegisterEvent(event, id);
}

bool Client::SendEventToRemoteServer(std::string event)
{
    return networkClass->Write(event);
}

//Poll an event
bool Client::PollEvent()
{
    return eventManager->PollEvent();
}

//Check if there is an event to Poll
bool Client::PeekEvent()
{
    return eventManager->PeekEvent();
}

void Client::HandleInput(SDL_Event SDLEvent)
{
    if (SDLEvent.type == SDL_KEYDOWN)
    {
        SDLKey keyPressed = SDLEvent.key.keysym.sym;
        if (keyPressed == moveRight)    //Right button pressed
        {
            player->MoveRight();
        }
        else if (keyPressed == moveLeft)    //Left button pressed
        {
            player->MoveLeft();
        }
        else if (keyPressed == moveDown)    //Down button pressed
        {
            player->MoveDown();
        }
        else if (keyPressed == moveUp)    //Up button pressed
        {
            player->MoveUp();
        }
        else if (keyPressed == attack)
        {
            player->Attack();
        }
        else if (keyPressed == changeTarget)
        {
            Character* closest = NULL;
            for (std::map<int, Character*>::iterator i = characterMap.begin(); i != characterMap.end(); i++)
            {
                if ((closest == NULL) || ((abs(i->second->GetPosition().x - player->GetPosition().x) + abs(i->second->GetPosition().y - player->GetPosition().y) < abs(closest->GetPosition().x - player->GetPosition().x) + abs(closest->GetPosition().y - player->GetPosition().y))))
                {
                    if (i->second != player && i->second != player->GetTarget())    //Don't let them target themselves this way, or their current target
                    {
                        closest = i->second;
                    }
                }
            }
            player->ChangeTarget(closest);
        }
    }
    else if (SDLEvent.type == SDL_KEYUP)
    {
        SDLKey keyUp = SDLEvent.key.keysym.sym;
        if (keyUp == moveRight)    //Right button released
        {
            if (player->GetVelocity().x > 0)   //If you were moving right
            {
                player->StopMoveHoriz();    //Stop moving right
            }
        }
        else if (keyUp == moveLeft)     //Left button released
        {
            if (player->GetVelocity().x < 0)   //If you were moving left
            {
                player->StopMoveHoriz();    //Stop moving left
            }
        }
        else if (keyUp == moveUp)     //Up button released
        {
            if (player->GetVelocity().y < 0)   //If you were moving up
            {
                player->StopMoveVert();    //Stop moving up
            }
        }
        else if (keyUp == moveDown) //Down button released
        {
            if (player->GetVelocity().y > 0) //If you were moving down
            {
                player->StopMoveVert();  //Stop moving down
            }
        }
    }
}


