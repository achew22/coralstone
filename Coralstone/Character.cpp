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

#include "Character.h"
#include "Graphics.h"
#include "Conversions.h"
#include "Event.h"

#include <sstream>

Character::Character(int areaId, int locx, int locy, int width, int height, SDL_Surface *destinationSurface, int ID)
{
	flagList.push_back(1);	//Facing flag: 1-left, 2-right, 3-up, 4-down
	flagList.push_back(0);	//AutoPilot flag: 0-off, 1-on
	flagList.push_back(0);  //Jumping flag: 0-not jumping, 1-jumping

    currentAreaId = areaId;
    pos.x = locx;
	pos.y = locy;
	dim.x = width;
	dim.y = height;
	vel.x = 0;
	vel.y = 0;

	id = ID;
	clientId = -1;  //This means that no client is yet assigned

	destination = destinationSurface;
	lastTime = SDL_GetTicks();

	totalMagic = 100;
	totalHealth = 100;
	remainingMagic = 100;
	remainingHealth = 100;

	healthMeter = new Meter(40, 4, 255, 0, 0);
	healthMeter->SetPercent((remainingHealth * 100.0)/totalHealth);
	magicMeter = new Meter(40, 4, 0, 255, 0);
	magicMeter->SetPercent((remainingMagic * 100.0)/totalMagic);

	target = NULL;

	animList.clear();	//Just make sure that these are all clear
	//flagList.clear();

	//Set up the animations, based on the current layout of the spritesheets. Should be within the character class
	//since all character spritesheets should be set up in the same way (so says I!)
    //Walking
	std::vector<int> animLeft;
	animLeft.push_back(19);
	animLeft.push_back(18);
	animLeft.push_back(19);
	animLeft.push_back(20);
	AddAnimation(animLeft);

    std::vector<int> animRight;
	animRight.push_back(7);
	animRight.push_back(6);
	animRight.push_back(7);
	animRight.push_back(8);
	AddAnimation(animRight);

    std::vector<int> animUp;
	animUp.push_back(1);
	animUp.push_back(0);
	animUp.push_back(1);
	animUp.push_back(2);
	AddAnimation(animUp);

    std::vector<int> animDown;
	animDown.push_back(13);
	animDown.push_back(12);
	animDown.push_back(13);
	animDown.push_back(14);
	AddAnimation(animDown);

	//Jumping
    std::vector<int> animJumpLeft;
	animJumpLeft.push_back(19);
	AddAnimation(animJumpLeft);

    std::vector<int> animJumpRight;
	animJumpRight.push_back(7);
	AddAnimation(animJumpRight);

    std::vector<int> animJumpUp;
	animJumpUp.push_back(1);
	AddAnimation(animJumpUp);

    std::vector<int> animJumpDown;
	animJumpDown.push_back(13);
	AddAnimation(animJumpDown);

	//Standing still
    std::vector<int> animStillLeft;
	animStillLeft.push_back(19);
	AddAnimation(animStillLeft);

    std::vector<int> animStillRight;
	animStillRight.push_back(7);
	AddAnimation(animStillRight);

    std::vector<int> animStillUp;
	animStillUp.push_back(1);
	AddAnimation(animStillUp);

    std::vector<int> animStillDown;
	animStillDown.push_back(13);
	AddAnimation(animStillDown);

    ChangeAnimation(&animList[ANIM_STILLRIGHT]);   //Always begin facing right, although this should be changed almost immediately
        //in almost all circumstances
}

Character::Character(std::string serialized, SDL_Surface* destinationSurface)
{
	flagList.push_back(1);	//Facing flag: 1-left, 2-right, 3-up, 4-down
	flagList.push_back(0);	//AutoPilot flag: 0-off, 1-on
	flagList.push_back(0);  //Jumping flag: 0-not jumping, 1-jumping

    std::stringstream inString;
    inString.str(serialized);

    std::string temp;
    inString >> temp;   //Should be 'Pos.x:'
    inString >> temp;
    pos.x = Conversions::StringToInt(temp);
    inString >> temp;   //Should be 'Pos.y:'
    inString >> temp;
	pos.y = Conversions::StringToInt(temp);
    inString >> temp;   //Should be 'Dim.x:'
    inString >> temp;
	dim.x = Conversions::StringToInt(temp);
    inString >> temp;   //Should be 'Dim.y:'
    inString >> temp;
	dim.y = Conversions::StringToInt(temp);
    inString >> temp;   //Should be 'Vel.x:'
    inString >> temp;
	vel.x = Conversions::StringToInt(temp);
    inString >> temp;   //Should be 'Vel.y:'
    inString >> temp;
	vel.y = Conversions::StringToInt(temp);
    inString >> temp;   //Should be 'Id:'
    inString >> temp;
	id = Conversions::StringToInt(temp);
	inString >> temp;   //Should be 'CurrentSectorId:'
	inString >> temp;
	currentSectorId = Conversions::StringToInt(temp);
	inString >> temp;   //Should be 'CurrentAreaId:'
	inString >> temp;
	currentAreaId = Conversions::StringToInt(temp);
	clientId = -1;

	destination = destinationSurface;
	lastTime = SDL_GetTicks();

	totalMagic = 100;
	totalHealth = 100;
	remainingMagic = 100;
	remainingHealth = 100;

	healthMeter = new Meter(40, 4, 255, 0, 0);
	healthMeter->SetPercent((remainingHealth * 100.0)/totalHealth);
	magicMeter = new Meter(40, 4, 0, 255, 0);
	magicMeter->SetPercent((remainingMagic * 100.0)/totalMagic);

	target = NULL;

	animList.clear();	//Just make sure that these are all clear

	//Set up the animations, based on the current layout of the spritesheets. Should be within the character class
	//since all character spritesheets should be set up in the same way (so says I!)
    //Walking
	std::vector<int> animLeft;
	animLeft.push_back(19);
	animLeft.push_back(18);
	animLeft.push_back(19);
	animLeft.push_back(20);
	AddAnimation(animLeft);

    std::vector<int> animRight;
	animRight.push_back(7);
	animRight.push_back(6);
	animRight.push_back(7);
	animRight.push_back(8);
	AddAnimation(animRight);

    std::vector<int> animUp;
	animUp.push_back(1);
	animUp.push_back(0);
	animUp.push_back(1);
	animUp.push_back(2);
	AddAnimation(animUp);

    std::vector<int> animDown;
	animDown.push_back(13);
	animDown.push_back(12);
	animDown.push_back(13);
	animDown.push_back(14);
	AddAnimation(animDown);

	//Jumping
    std::vector<int> animJumpLeft;
	animJumpLeft.push_back(19);
	AddAnimation(animJumpLeft);

    std::vector<int> animJumpRight;
	animJumpRight.push_back(7);
	AddAnimation(animJumpRight);

    std::vector<int> animJumpUp;
	animJumpUp.push_back(1);
	AddAnimation(animJumpUp);

    std::vector<int> animJumpDown;
	animJumpDown.push_back(13);
	AddAnimation(animJumpDown);

	//Standing still
    std::vector<int> animStillLeft;
	animStillLeft.push_back(19);
	AddAnimation(animStillLeft);

    std::vector<int> animStillRight;
	animStillRight.push_back(7);
	AddAnimation(animStillRight);

    std::vector<int> animStillUp;
	animStillUp.push_back(1);
	AddAnimation(animStillUp);

    std::vector<int> animStillDown;
	animStillDown.push_back(13);
	AddAnimation(animStillDown);

    ChangeAnimation(&animList[ANIM_STILLRIGHT]);   //Always begin facing right, although this should be changed almost immediately
        //in almost all circumstances
}

Character::~Character()
{
    if (healthMeter != NULL)
    {
        delete healthMeter;
    }
    if (magicMeter != NULL)
    {
        delete magicMeter;
    }
}

void Character::AddAnimation(std::vector<int> animation, std::string filename)
{
    animList.push_back(Animation(filename, 48, 64, animation, 120, 255, 0, 0));
}

//Switch to a new animation
void Character::ChangeAnimation(Animation* animation)
{
    currentAnim = animation;
    currentAnim->Begin();
}

//Updates everything about the character that isn't the animation, ie does not blit the image to the screen or worry about
//what animation it ought to be on. Used primarily for the server
void Character::UpdatePosition()
{
    double secsPassed = (SDL_GetTicks() - lastTime)/1000.0;
	pos.x += vel.x * secsPassed;
	pos.y += vel.y * secsPassed;

	//Keep the position within the level, currently 4000 pixels by 400 pixels
	if (pos.x < 0)
	{
		pos.x = 0;
	}
	else if (pos.x + dim.x > SCREEN_WIDTH)
	{
		pos.x = SCREEN_WIDTH - dim.x;
	}
    if (pos.y < 0)
	{
	    pos.y = 0;
	}
	else if (pos.y + dim.y > SCREEN_HEIGHT)
	{
	    pos.y = SCREEN_HEIGHT - dim.y;
	}

	lastTime = SDL_GetTicks();  //Update the last time
}

//Call only when the character is going to be drawn to the screen, that's all this does
void Character::UpdateAnimation()
{
    currentAnim->ApplyCurrentSprite(pos.x, pos.y, destination);

    healthMeter->ApplyToSurface(pos.x + 4, pos.y - 8, destination);
    magicMeter->ApplyToSurface(pos.x + 4, pos.y - 4, destination);

	lastTime = SDL_GetTicks();  //Update the lastTime function
}

//Returns true if there are events to be processed, false otherwise
bool Character::PeekEvent()
{
    return !eventQueue.empty();
}

//Returns the next serialized event, which is an event this character created, or "NULL" if there are no events
std::string Character::PollEvent()
{
    if (eventQueue.empty())
    {
        return "NULL";
    }
    std::string toReturn = eventQueue.front();
    eventQueue.pop();
    return toReturn;
}

Point Character::GetPosition()
{
    return pos;
}

int Character::GetId()
{
    return id;
}

int Character::GetClientId()
{
	return clientId;
}

int Character::GetAreaId()
{
    return currentAreaId;
}

Character* Character::GetTarget()
{
    return target;
}

void Character::AssignClient(int theClientId)
{
    clientId = theClientId;
}

//The parameter info can be anything, it is disregarded here
void Character::Jump(std::string info)
{
    Jump(false);
}

//Cause the character to Jump, which currently is deprecated
//If addEvent is true (it is by default), then this event will be added to the event Queue
void Character::Jump(bool addEvent /*=true*/)
{
    if (flagList[FLAG_JUMPING] == 0)    //If not already jumping
    {
        switch (flagList[FLAG_FACING])
        {
        case 1: //If facing left
            ChangeAnimation(&animList[ANIM_JUMPLEFT]);  //Set the appropriate animation
            break;
        case 2: //If facing right
            ChangeAnimation(&animList[ANIM_JUMPRIGHT]);
            break;
        case 3: //If facing up
            ChangeAnimation(&animList[ANIM_JUMPUP]);
            break;
        case 4: //If facing down
            ChangeAnimation(&animList[ANIM_JUMPDOWN]);
            break;
        default:    //If none of these - signifies error
            ChangeAnimation(&animList[ANIM_JUMPLEFT]);
            break;
        }
        vel.y = -400;
        flagList[FLAG_JUMPING] = 1;
    }

    if (addEvent && (clientId != -1))
    {
        eventQueue.push(Event::Serialize("Jump", id, "NULL"));
    }
}

//Here the parameter info is useless, could be anything
void Character::StopJump(std::string info)
{
    StopJump(false);
}

//Cease jumping. If addEvent is true (it is by default), this event will be added to the eventQueue
void Character::StopJump(bool addEvent /*=true*/)
{
    if (vel.x < 0)  //If moving left
    {
        ChangeAnimation(&animList[ANIM_MOVELEFT]);
    }
    else if (vel.x > 0) //If moving right
    {
        ChangeAnimation(&animList[ANIM_MOVERIGHT]);
    }
    else if (vel.y < 0) //If moving up
    {
        ChangeAnimation(&animList[ANIM_MOVEUP]);
    }
    else if (vel.y > 0) //If moving down
    {
        ChangeAnimation(&animList[ANIM_MOVEDOWN]);
    }
    else    //If not moving
    {
        if (flagList[FLAG_FACING] == 1)  //If facing left
        {
            ChangeAnimation(&animList[ANIM_STILLLEFT]);
        }
        else if (flagList[FLAG_FACING] == 2)   //If facing right
        {
            ChangeAnimation(&animList[ANIM_STILLRIGHT]);
        }
        else if (flagList[FLAG_FACING] == 3)
        {
            ChangeAnimation(&animList[ANIM_STILLUP]);
        }
        else if (flagList[FLAG_FACING] == 4)
        {
            ChangeAnimation(&animList[ANIM_STILLDOWN]);
        }
    }
    vel.y = 0;
    flagList[FLAG_JUMPING] = 0;

    if (addEvent && (clientId != -1))
    {
        eventQueue.push(Event::Serialize("StopJump", id, "NULL"));
    }
}

//Info here looks like the following:
//"Direction: direc xPos: x yPos: y"
void Character::Move(std::string info)
{
    if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
    {
        printf("Character %i (client %i) was told to move via an event. Character is now at position ", id, clientId);
    }
    std::stringstream inStream;
    inStream.str(info);
    std::string inString;
    inStream >> inString;   //Pull in 'Direction:'
    inStream >> inString;   //Pull in 'direc'
    std::string direc = inString;
    inStream >> inString;   //Pull in 'xPos:'
    inStream >> inString;   //Pull in 'x'
    if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
    {
        printf("%s, ", inString.c_str());
    }
    pos.x = Conversions::StringToInt(inString);
    inStream >> inString;   //Pull in 'yPos:'
    inStream >> inString;   //Pull in 'y'
    if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
    {
        printf("%s. Movement is in the ", inString.c_str());
    }
    pos.y = Conversions::StringToInt(inString);

    //Handle which direction to move
    if (direc == "Up")
    {
        MoveUp(false);
        if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
        {
            printf("Up direction.\n\n");
        }
    }
    else if (direc == "Down")
    {
        MoveDown(false);
        if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
        {
            printf("Down direction.\n\n");
        }
    }
    else if (direc == "Left")
    {
        MoveLeft(false);
        if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
        {
            printf("Left direction.\n\n");
        }
    }
    else if (direc == "Right")
    {
        MoveRight(false);
        if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
        {
            printf("Right direction.\n\n");
        }
    }
    else
    {
        if (DEBUG_SHOWALL || DEBUG_SHOWERRORS)
        {
            printf("\n--------An error occurred: Move -- the info parameter - %s - is not a proper format\n\n", info.c_str());
        }
    }
}

//Moves the character left. If addEvent is true (it is by default), an event showing that this character moved left will be added to the eventQueue.
void Character::MoveLeft(bool addEvent /*=true*/)
{
    if (flagList[FLAG_JUMPING] == 1)    //If you are jumping
    {
        ChangeAnimation(&animList[ANIM_JUMPLEFT]); //Set the left jumping animation
    }
    else
    {
        ChangeAnimation(&animList[ANIM_MOVELEFT]);
    }
    flagList[FLAG_FACING] = 1;
    vel.x = -80;   //800 pixels per second, give or take

    if (addEvent && (clientId != -1))
    {
        eventQueue.push(Event::Serialize("Move", id, "Direction: Left xPos: " + Conversions::IntToString(pos.x) + " yPos: " + Conversions::IntToString(pos.y)));
    }
}

//If addEvent is true (it is by default) this will be added to the eventQueue as well
void Character::MoveRight(bool addEvent /*=true*/)
{
    if (flagList[FLAG_JUMPING] == 1)    //If you are jumping
    {
        ChangeAnimation(&animList[ANIM_JUMPRIGHT]); //Set the right jumping animation
    }
    else
    {
        ChangeAnimation(&animList[ANIM_MOVERIGHT]);
    }
    flagList[FLAG_FACING] = 2;
    vel.x = 80;    //800 pixels per second, give or take

    if (addEvent && (clientId != -1))
    {
        eventQueue.push(Event::Serialize("Move", id, "Direction: Right xPos: " + Conversions::IntToString(pos.x) + " yPos: " + Conversions::IntToString(pos.y)));
    }
}

//If addEvent is true (it is by default) this will be added to the eventQueue as well
void Character::MoveUp(bool addEvent /*=true*/)
{
    ChangeAnimation(&animList[ANIM_MOVEUP]);
    flagList[FLAG_FACING] = 3;
    vel.y = -80;

    if (addEvent && (clientId != -1))
    {
        eventQueue.push(Event::Serialize("Move", id, "Direction: Up xPos: " + Conversions::IntToString(pos.x) + " yPos: " + Conversions::IntToString(pos.y)));
    }
}

//If addEvent is true (it is by default) this will be added to the eventQueue as well
void Character::MoveDown(bool addEvent /*=true*/)
{
    ChangeAnimation(&animList[ANIM_MOVEDOWN]);
    flagList[FLAG_FACING] = 4;
    vel.y = 80;

    if (addEvent && (clientId != -1))
    {
        eventQueue.push(Event::Serialize("Move", id, "Direction: Down xPos: " + Conversions::IntToString(pos.x) + " yPos: " + Conversions::IntToString(pos.y)));
    }
}

//info should be of the following form: "Direction: direc xPos: x yPos: y"
void Character::StopMove(std::string info)
{
    if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
    {
        printf("Character %i (client %i) was told to stop moving via an event. Character is now at position ", id, clientId);
    }
    std::stringstream inStream;
    inStream.str(info);
    std::string inString;
    inStream >> inString;   //Pull in 'Direction:'
    inStream >> inString;   //Pull in 'direc'
    std::string direc = inString;
    inStream >> inString;   //Pull in 'xPos:'
    inStream >> inString;   //Pull in 'x'
    if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
    {
        printf("%s, ", inString.c_str());
    }
    pos.x = Conversions::StringToInt(inString);
    inStream >> inString;   //Pull in 'yPos:'
    inStream >> inString;   //Pull in 'y'
    if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
    {
        printf("%s. Movement is in the ", inString.c_str());
    }
    pos.y = Conversions::StringToInt(inString);

    if (direc == "Horiz")
    {
        StopMoveHoriz(false);
        if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
        {
            printf("Horizontal direction.\n\n");
        }
    }
    else if (direc == "Vert")
    {
        StopMoveVert(false);
        if (DEBUG_SHOWALL || DEBUG_SHOWEVENTS)
        {
            printf("Vertical direction.\n\n");
        }
    }
    else
    {
        if (DEBUG_SHOWALL || DEBUG_SHOWERRORS)
        {
            printf("\n----------An error occurred: StopMove -- the info string - %s - is not of the proper format\n\n", info.c_str());
        }
    }
}

//if addEvent is true (it is by default), this event will be added to eventQueue
void Character::StopMoveHoriz(bool addEvent /*=true*/)
{
    if (vel.y < 0)  //If moving up
    {
        ChangeAnimation(&animList[ANIM_MOVEUP]);
        flagList[FLAG_FACING] = 3;
    }
    else if (vel.y > 0)   //If moving down
    {
        ChangeAnimation(&animList[ANIM_MOVEDOWN]);
        flagList[FLAG_FACING] = 4;
    }
    else if (flagList[FLAG_FACING] == 1)    //If facing left
    {
        ChangeAnimation(&animList[ANIM_STILLLEFT]);
    }
    else if (flagList[FLAG_FACING] == 2)    //If facing right
    {
        ChangeAnimation(&animList[ANIM_STILLRIGHT]);
    }
    vel.x = 0;

    if (addEvent && (clientId != -1))
    {
        eventQueue.push(Event::Serialize("StopMove", id, "Direction: Horiz xPos: " + Conversions::IntToString(pos.x) + " yPos: " + Conversions::IntToString(pos.y)));
    }
}

//If addEvent is true (it is by default), this event will be added to the eventQueue
void Character::StopMoveVert(bool addEvent /*=true*/)
{
    if (vel.x < 0)  //If moving left
    {
        ChangeAnimation(&animList[ANIM_MOVELEFT]);
        flagList[FLAG_FACING] = 1;
    }
    else if (vel.x > 0)   //If moving right
    {
        ChangeAnimation(&animList[ANIM_MOVERIGHT]);
        flagList[FLAG_FACING] = 2;
    }
    else if (flagList[FLAG_FACING] == 3)    //If facing up
    {
        ChangeAnimation(&animList[ANIM_STILLUP]);
    }
    else if (flagList[FLAG_FACING] == 4)    //If facing down
    {
        ChangeAnimation(&animList[ANIM_STILLDOWN]);
    }
    vel.y = 0;

    if (addEvent && (clientId != -1))
    {
        eventQueue.push(Event::Serialize("StopMove", id, "Direction: Vert xPos: " + Conversions::IntToString(pos.x) + " yPos: " + Conversions::IntToString(pos.y)));
    }
}

void Character::ChangeTarget(Character* theTarget, bool addEvent /* = true */)
{
    target = theTarget;

    if (addEvent && (clientId != -1))
    {
        if (target != NULL)
        {
            eventQueue.push(Event::Serialize("ChangeTarget", id, "New_Target: " + Conversions::IntToString(target->id)));
        }
        else
        {
            eventQueue.push(Event::Serialize("ChangeTarget", id, "New_Target: None"));
        }
    }
}

void Character::ChangeTarget(std::string info, Character* theTarget)
{
    ChangeTarget(theTarget, false);
}

void Character::Attack(bool addEvent /* = true */)
{
    //For now, this is overly simplified, just do 10 damage
    if (target == NULL)
    {
        if (DEBUG_SHOWALL || DEBUG_SHOWERRORS)
        {
            printf("Error: Character with id %i (client %i) attempted to attack a NULL\n", id, clientId);
        }
        return;
    }

    UseMagic(5, addEvent);
    target->TakeDamage(10, addEvent);

    if (addEvent && (clientId != -1))
    {
        eventQueue.push(Event::Serialize("Attack", id, "NULL"));
    }
}

void Character::Attack(std::string info)
{
    Attack(false);
}

void Character::Defend(bool addEvent /* = true */)
{
}

void Character::Defend(std::string info)
{
}

//These functions should probably be moved to private, and only be accessed by the Defend function
    //but I'll do that later, when I feel like it
void Character::TakeDamage(int amount, bool addEvent /* = true */)
{
    remainingHealth -= amount;
    if (remainingHealth <= 0)
    {
        Die(addEvent);
        remainingHealth = 0;
    }
    healthMeter->SetPercent((remainingHealth * 100)/totalHealth);

    if (addEvent && (clientId != -1))
    {
        eventQueue.push(Event::Serialize("TakeDamage", id, "Amount: " + Conversions::IntToString(amount)));
    }
}

void Character::TakeDamage(std::string info)
{
    std::stringstream inString;
    inString.str(info);
    std::string temp = "";
    inString >> temp;   //Should read "Amount:"
    inString >> temp;   //Should be the amount
    TakeDamage(Conversions::StringToInt(temp), false);
}

void Character::UseMagic(int amount, bool addEvent /* = true */)
{
    remainingMagic -= amount;
    if (remainingMagic < 0)
    {
        remainingMagic = 0;
    }
    magicMeter->SetPercent((remainingMagic * 100)/totalMagic);

    if (addEvent && (clientId != -1))
    {
        eventQueue.push(Event::Serialize("UseMagic", id, "Amount: " + Conversions::IntToString(amount)));
    }
}

void Character::UseMagic(std::string info)
{
    std::stringstream inString;
    inString.str(info);
    std::string temp = "";
    inString >> temp;   //Should read "Amount:"
    inString >> temp;   //Should be the amount
    UseMagic(Conversions::StringToInt(temp), false);
}

//Note: this does not yet do anything at all
void Character::Die(bool addEvent /* = true */)
{
    if (addEvent)
    {
        eventQueue.push(Event::Serialize("Die", id, "NULL"));
    }
}

//Note: neither does this
void Character::Die(std::string info)
{
    Die(false);
}

Point Character::GetVelocity()
{
    return vel;
}

std::string Character::Serialize()
{
    std::string serialized = "";
    serialized += "Pos.x: " + Conversions::IntToString(pos.x) + " ";
    serialized += "Pos.y: " + Conversions::IntToString(pos.y) + " ";
    serialized += "Dim.x: " + Conversions::IntToString(dim.x) + " ";
    serialized += "Dim.y: " + Conversions::IntToString(dim.y) + " ";
    serialized += "Vel.x: " + Conversions::IntToString(vel.x) + " ";
    serialized += "Vel.y: " + Conversions::IntToString(vel.y) + " ";
    serialized += "Id: " + Conversions::IntToString(id) + " ";
    serialized += "CurrentSectorId: " + Conversions::IntToString(currentSectorId) + " ";
    serialized += "CurrentAreaId: " + Conversions::IntToString(currentAreaId) + " ";
    return serialized;
}

void Character::CleanUp()
{
}
