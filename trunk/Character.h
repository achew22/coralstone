#ifndef CHARACTER_H
#define CHARACTER_H

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

#include "Animation.h"
#include "Slot.h"
#include "Point.h"
#include "Sector.h"
#include "Area.h"
#include "Meter.h"

#include <SDL/SDL.h>
#include <map>
#include <string>
#include <vector>
#include <queue>

//The class for any and all characters in the game.
    //Dynamically allocates memory for healthMeter and magicMeter
class Character
{
private:
    Point pos;  //Position
	Point dim;  //Dimensions
	Point vel;	//Velocity in pixels per second
	int id;  //The unique id of this character
	std::vector<Slot> inventory;    //The inventory

    int lastTime;   //The time (in ms) of the last time Update was called. Allows for the positions, etc. to be adjusted
        //independently of frame rate.
	SDL_Surface *destination; //A pointer to the surface where the sprites should be blitted to
	std::vector<Animation> animList;	//A list of the different possible animations
	std::vector<int> flagList;	//A list of all of the flags
    Animation* currentAnim; //A pointer to the current animation being used

    int currentSectorId;    //The current Sector
    int currentAreaId;      //The current Area

    int clientId;   //-1 means no client is assigned

    std::queue<std::string> eventQueue;  //Holds all of the events that this Character creates, so that they might be polled by
        //the Client

	enum
	{
		FLAG_FACING,	    //The flag indicating which way the character is facing
                                //1-left  2-right  3-up  4-down
		FLAG_AUTOPILOT,	    //The flag indicating whether or not the AI should control this character
                                //0-no  1-yes
		FLAG_JUMPING,       //The flag indicating whether or not the character is jumping
                                //0-no  1-yes
		ANIM_MOVELEFT = 0,  //The animations corresponding to moving left
		ANIM_MOVERIGHT,     //and right
		ANIM_MOVEUP,        //and up
		ANIM_MOVEDOWN,      //and down
		ANIM_JUMPLEFT,      //jumping animation, facing left
		ANIM_JUMPRIGHT,     //jumping animation, facing right
		ANIM_JUMPUP,        //jumping animation, facing up
		ANIM_JUMPDOWN,      //jumping animation, facing down
		ANIM_STILLLEFT,     //the standing still animation, facing left
		ANIM_STILLRIGHT,    //standing still, facing right
		ANIM_STILLUP,       //standing still, facing up
		ANIM_STILLDOWN,     //standing still, facing down
	};

	int totalMagic;
	int totalHealth;
	int remainingMagic;
	int remainingHealth;
	Meter* healthMeter;
	Meter* magicMeter;

	Character* target;
public:
	Character(int areaId, int locx, int locy, int width, int height, SDL_Surface *destinationSurface, int ID);
	Character(std::string serialized, SDL_Surface* destinationSurface);
	~Character();
	void AddAnimation(std::vector<int> animation, std::string filename = "images/miniDungeonCharSprites2x.png");
	void ChangeAnimation(Animation* animation);
    void UpdatePosition();  //Updates everything but the animation
	void UpdateAnimation();	//Updates the surface on which the character should be updated
	bool PeekEvent();   //Returns whether or not there are events to poll
	std::string PollEvent();    //Get the first event in the event queue, returns "NULL" otherwise
    Point GetPosition();
    static void CleanUp();  //Cleans up all of the dynamically allocated memory stored in characterList
	Point GetVelocity();	//Returns the velocity
	int GetId();    //Returns the id
	int GetClientId();	//Returns the client id number
	int GetAreaId();    //Returns the area id
	Character* GetTarget();    //Returns a pointer to the target
	void AssignClient(int theClientId);

	//Below are all of the functions corresponding to events. If a function is overloaded with the string
	//parameter "info," that is the function that will be called when and event with type "EVENT_TYPE_FUNCTIONNAME"
	//is polled
	void Jump(std::string info);
	void Jump(bool addEvent = true);
	void StopJump(std::string info);
	void StopJump(bool addEvent = true);
	void Move(std::string info);
	void MoveRight(bool addEvent = true);
	void MoveLeft(bool addEvent = true);
	void MoveUp(bool addEvent = true);
	void MoveDown(bool addEvent = true);
	void StopMove(std::string info);
	void StopMoveHoriz(bool addEvent = true);
	void StopMoveVert(bool addEvent = true);
	void ChangeTarget(Character* theTarget, bool addEvent = true);
    void ChangeTarget(std::string info, Character* theTarget);
	void Attack(bool addEvent = true);
	void Attack(std::string info);
	void Defend(bool addEvent = true);
	void Defend(std::string info);
	void TakeDamage(int amount, bool addEvent = true);
	void TakeDamage(std::string info);
	void UseMagic(int amount, bool addEvent = true);
	void UseMagic(std::string info);
	void Die(bool addEvent = true);
	void Die(std::string info);

	//Serialize this character, for transport
	std::string Serialize();
};

#endif
