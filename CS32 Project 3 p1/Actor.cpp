#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


//-----------------Actor--------------------
StudentWorld* Actor::getWorld()const {
	return m_ptr;
}
Actor::Actor(int ID, int level_x, int level_y, int direction, int depth, StudentWorld* p)
:GraphObject(ID,  level_x, level_y, direction, depth)
{
	m_ptr = p;
}

bool Actor::isPlayer() const{
	return false;
}

bool Actor::isWall() const{
	return false;
}
//-------------Penelope------------------

Penelope::Penelope(int level_x, int level_y, StudentWorld* p) 
	:Actor(IID_PLAYER, level_x, level_y, right, 0, p) {
	alive = true;
	infected = false;
	infection_count = 0;
	landmine = 0;
	flame = 0;
	vaccines = 0;

}

bool Penelope::moveTowards(int ch) {
	switch (ch) {
	case KEY_PRESS_LEFT:
		if (getWorld()->isValid(getX() - 4, getY())) {
			moveTo(getX()-4,getY());
			return true;
		}
		break;
	case KEY_PRESS_RIGHT:
		if (getWorld()->isValid(getX() + 4, getY())) {
			moveTo(getX()+4,getY());
			return true;
		}
		break;
	case KEY_PRESS_UP:
		if (getWorld()->isValid(getX() , getY()+4)) {
			moveTo(getX(), getY()+4);
			return true;
		}
		break;
	case KEY_PRESS_DOWN:
		if (getWorld()->isValid(getX() , getY()-4)) {
			moveTo(getX(), getY()-4);
			return true;
		}
			break;
	}
	return false;
}
 void Penelope::doSomething() {
	 if (isAlive()) {
		 if (isInfected()) {
			 infection_count++;
			 if (infection_count >= 500) {
				 setDead();
				 return;
			 }
		 }
		 int ch;
		 if (getWorld()->getKey(ch)) {
			 switch (ch) {
//-------------direction-------------------
			 case KEY_PRESS_LEFT:
				 moveTowards(ch);
				 setDirection(left);
				 break;
			 case KEY_PRESS_RIGHT:
				 moveTowards(ch);
				 setDirection(right);
				 break;
			 case KEY_PRESS_UP:
				 moveTowards(ch);
				 setDirection(up);
				 break;
			 case KEY_PRESS_DOWN:
				 moveTowards(ch);
				 setDirection(down);
				 break;
//----------------use goodies-------------------
			 case KEY_PRESS_SPACE:
				 break;
			 case KEY_PRESS_TAB:
				 break;
			 case KEY_PRESS_ENTER:
				 break;
			 }
		 }
	 }
	return;
}
	


void Penelope::move() {

}
bool Penelope::isAlive()const {
	return alive;
}
bool Penelope::isInfected()const {
	return infected;
}
void Penelope::setDead() {
	alive = false;
}
bool Penelope::isPlayer() const {
	return true;
 }

//---------------------Wall--------------------
Wall::Wall(int level_x, int level_y, StudentWorld* p)
:Actor(IID_WALL,level_x,level_y, right, 0, p)
{}

void Wall::doSomething() {
	return;
}//does nothing;

bool Wall::isWall() const {
	return true;
}
//---------Exit------------

Exit::Exit(int level_x, int level_y, StudentWorld* p):Actor(IID_EXIT, level_x, level_y, right, 1, p) {
	
}

void Exit::doSomething() {
	if (getWorld()->Overlap(getX(), getY(), getWorld()->getPlayerX(), getWorld()->getPlayerY())) {
		getWorld()->increaseScore(500);
		getWorld()->FinishLevel();
	}

}