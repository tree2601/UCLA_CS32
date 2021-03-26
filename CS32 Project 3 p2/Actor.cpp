#include "Actor.h"
#include "StudentWorld.h"
#include<algorithm>
#include<cmath>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


//-----------------Actor--------------------
StudentWorld* Actor::getWorld()const {
	return m_ptr;
}
Actor::Actor(int ID, double level_x, double level_y, int direction, int depth, StudentWorld* p)
:GraphObject(ID,  level_x, level_y, direction, depth),m_alive(true),m_ptr(p)
{}

bool Actor::isAlive()const {
	return m_alive;
}
void Actor::setDead() {
	m_alive = false;
}

bool Actor::moveTowards(int ch) {
	switch (ch) {
	case KEY_PRESS_LEFT:
		if (getWorld()->isValid(getX() - 4, getY(),this)) {
			moveTo(getX() - 4, getY());
			return true;
		}
		break;
	case KEY_PRESS_RIGHT:
		if (getWorld()->isValid(getX() + 4, getY(), this)) {
			moveTo(getX() + 4, getY());
			return true;
		}
		break;
	case KEY_PRESS_UP:
		if (getWorld()->isValid(getX(), getY() + 4, this)) {
			moveTo(getX(), getY() + 4);
			return true;
		}
		break;
	case KEY_PRESS_DOWN:
		if (getWorld()->isValid(getX(), getY() - 4, this)) {
			moveTo(getX(), getY() - 4);
			return true;
		}
		break;
	}
	return false;
}
//----------Agent--------------------------
Agent::Agent(int ID, double level_x, double level_y, int direction, int depth, StudentWorld* p)
:Actor(ID, level_x, level_y, direction, depth, p)
{

}
void Agent::setRandomDirection() {
	int a = randInt(1, 4);
	if (a == 1) {
		setDirection(up);
	}
	if (a == 2) {
		setDirection(down);
	}
	if (a == 3) {
		setDirection(left);
	}
	if (a == 4) {
		setDirection(right);
	}
 }

bool Agent::tryMoveBy(int d) {
	int a = getDirection();
	switch (a) {
	case left:
		if (getWorld()->isValid(getX() - d, getY(), this)) {
			moveTo(getX() - d, getY());
			return true;
		}
		break;
	case right:
		if (getWorld()->isValid(getX() + d, getY(), this)) {
			moveTo(getX() + d, getY());
			return true;
		}
		break;
	case up:
		if (getWorld()->isValid(getX() , getY()+d, this)) {
			moveTo(getX(), getY()+d);
			return true;
		}
		break;
	case down:
		if (getWorld()->isValid(getX(), getY() - d, this)) {
			moveTo(getX(), getY()-d);
			return true;
		}
		break;
	}
	return false;
}
//-----------Human---------------------
Human::Human(int ID, double level_x, double level_y, int direction, int depth, StudentWorld* p)
: Agent(ID, level_x, level_y, direction, depth, p),infected(false),infection_count(0) {

}
void Human::doSomething() {
	if (!isAlive()) {
		return;
	}
	if (isInfected()) {
		incrementInfection();
		if (infectionCount() >= 500) {
			setDead();
			getWorld()->decrementCitizens();
			getWorld()->addZombie(getX(), getY());
			return;
		}
	}
	if (paralyzed()) {
		return;
	}

	doSomethingSpecific();
}
//--------------Citizen--------------------
Citizen::Citizen(double level_x, double level_y, StudentWorld* p) : Human(IID_CITIZEN, level_x, level_y, right, 0, p)
{

};
bool Citizen::citizenTryMove(Direction d) {
	switch (d) {
	case left:
		if (getWorld()->isValid(getX() - 1, getY(), this)) {
			if (getWorld()->isValid(getX() - 2, getY(), this)) {
				setDirection(left);
				moveTo(getX() - 2, getY());
				return true;
			}
		}
		break;
	case right:

		if (getWorld()->isValid(getX() + 1, getY(), this)) {
			if (getWorld()->isValid(getX() + 2, getY(), this)) {
				setDirection(right);
				moveTo(getX() + 2, getY());
				return true;
			}
		}

		break;
	case up:
		if (getWorld()->isValid(getX(), getY() + 1, this)) {
			if (getWorld()->isValid(getX(), getY() + 2, this)) {
				setDirection(up);
				moveTo(getX(), getY() + 2);
				return true;
			}
		}
		break;
	case down:
		if (getWorld()->isValid(getX(), getY() - 1, this)) {
			if (getWorld()->isValid(getX(), getY() - 2, this)) {
				setDirection(down);
				moveTo(getX(), getY() - 2);
				return true;
			}
			break;

		}
	}
	return false;
}
void Citizen::doSomethingSpecific() {
	double dp = getWorld()->distance(getX(), getY(), getWorld()->PlayerX(), getWorld()->PlayerY());
	double xp = getWorld()->PlayerX();
	double yp = getWorld()->PlayerY();
	double xzmin = 100;
	double yzmin = 100;
	double dz = getWorld()->getClosestZombieDistance(getX(), getY(), xzmin, yzmin);
	if (dz == 10000 || dp < dz) {

		if (dp <= 80) {
			//int dir = getDirection();
			if (xp == getX()) {
				if (yp > getY()) {if (citizenTryMove(up)) {return;}}

				else if (yp < getY()) {if (citizenTryMove(down)) {return;}}
			}
			if (yp == getY()) {
				if (xp > getX()) {if (citizenTryMove(right)) {return;}}

				else if (xp < getX()) {if (citizenTryMove(left)) {return;}}
			}

			else {
				int r = randInt(1, 2);
				if (xp > getX() && yp > getY()) {
					switch (r) {
					case 1:
						if (citizenTryMove(right)) { return; }
						else if (citizenTryMove(up)) { return; }
						break;
					case 2:
						if (citizenTryMove(up)) { return; }
						else if (citizenTryMove(right)) { return; }
						break;
					}
				}
				else if (xp > getX() && yp < getY()) {
					switch (r) {
					case 1:
						if (citizenTryMove(right)) { return; }
						else if (citizenTryMove(down)) { return; }
						break;
					case 2:
						if (citizenTryMove(down)) { return; }
						else if (citizenTryMove(right)) { return; }
						break;
					}
				}
				else if (xp < getX() && yp < getY()) {
					switch (r) {
					case 1:
						if (citizenTryMove(left)) { return; }
						else if (citizenTryMove(down)) { return; }
						break;
					case 2:
						if (citizenTryMove(left)) { return; }
						else if (citizenTryMove(right)) { return; }
						break;
					}
				}
				else if (xp < getX() && yp > getY()) {
					switch (r) {
					case 1:
						if (citizenTryMove(left)) { return; }
						else if (citizenTryMove(up)) { return; }
						break;
					case 2:
						if (citizenTryMove(left)) { return; }
						else if (citizenTryMove(up)) { return; }
						break;
					}
				}
			}

		}
		
	}
	if (dz <= 80) {
		double xminr;
		double yminr;
		double dzl = -1;
		double dzr = -1;
		double dzu = -1;
		double dzd = -1;
		if (getWorld()->isValid(getX() + 1, getY(), this)) {
			if (getWorld()->isValid(getX() + 2, getY(), this)) {
				dzr = getWorld()->getClosestZombieDistance(getX() + 2, getY(), xminr, yminr);
			}
		}
		if (getWorld()->isValid(getX() - 1, getY(), this)) {
			if (getWorld()->isValid(getX() - 2, getY(), this)) {
				dzl = getWorld()->getClosestZombieDistance(getX() - 2, getY(), xminr, yminr);
			}
		}
		if (getWorld()->isValid(getX() , getY()+1, this)) {
			if (getWorld()->isValid(getX(), getY()+2, this)) {
				dzu = getWorld()->getClosestZombieDistance(getX() , getY()+2, xminr, yminr);
			}
		}
		if (getWorld()->isValid(getX(), getY()-1, this)) {
			if (getWorld()->isValid(getX(), getY()-2, this)) {
				dzd = getWorld()->getClosestZombieDistance(getX(), getY()-2, xminr, yminr);
			}
		}
		double dzn = dzl;
		if (dzr > dzn) {
			dzn = dzr;
		}
		if (dzu > dzn) {
			dzn = dzu;
		}
		if (dzd > dzn) {
			dzn = dzd;
		}
		if (dzn == -1 || dzn <= dz) {
			return;
		}
		else {
			if (dzn == dzl) {
				citizenTryMove(left);
				return;
			}
			if (dzn == dzr) {
				citizenTryMove(right);
				return;
			}
			if (dzn == dzu) {
				citizenTryMove(up);
				return;
			}
			if (dzn == dzd) {
				citizenTryMove(down);
				return;
			}
			
		}
	}

		
	return;
}
bool Citizen::paralyzed() {
	int a = getWorld()->getTicks();
	if (a % 2 == 0) {
		return true;
	}
	return false;
}
//-------------Penelope------------------

Penelope::Penelope(double level_x, double level_y, StudentWorld* p)
	:Human(IID_PLAYER, level_x, level_y, right, 0, p) {


	landmines = 0;
	flames = 0;
	vaccines = 0;

}
void Penelope::adjustLandmine(int n) {
	landmines += n;
}
void Penelope::adjustVaccine(int n) {
	vaccines += n;
}
void Penelope::adjustFlame(int n) {
	flames += n;
}
bool Penelope::useVaccine() {
	if (vaccines > 0) {
		vaccines--;
		heal();
		return true;
	}
	return false;
}
bool Penelope::useFlame() {
	if (flames > 0) {
		flames--;
		getWorld()->playSound(SOUND_PLAYER_FIRE);
		for (int i = 1; i <= 3; i++) {
			int d = getDirection();
			switch (d) {
			case up:
				if (!getWorld()->isFlameBlockedAt(getX(), getY() + i * SPRITE_HEIGHT)) {
					getWorld()->addFlame(getX(), getY() + i * SPRITE_HEIGHT, d);
				}
				else {
					return true;
				}
				break;
			case down:
				if (!getWorld()->isFlameBlockedAt(getX(), getY() - i * SPRITE_HEIGHT)) {
					getWorld()->addFlame(getX(), getY() - i * SPRITE_HEIGHT, d);
				}
				else {
					return true;
				}
				break;
			case left:
				if (!getWorld()->isFlameBlockedAt(getX() - i * SPRITE_HEIGHT, getY() )) {
					getWorld()->addFlame(getX() - i * SPRITE_HEIGHT, getY(), d);
				}
				else {
					return true;
				}
				break;
			case right:
				if (!getWorld()->isFlameBlockedAt(getX() + i * SPRITE_HEIGHT, getY())) {
					getWorld()->addFlame(getX() + i * SPRITE_HEIGHT, getY(), d);
				}
				else {
					return true;
				}
				break;
			}
		}
	}
	return false;
}
bool Penelope::useLandmine() {
	if (landmines > 0) {
		getWorld()->addLandMine(getX(), getY());
		landmines--;
		return true;
	}
	
	return false;
}


void Penelope::doSomethingSpecific() {
	
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
				useFlame();
				break;
			case KEY_PRESS_TAB:
				useLandmine();
				break;
			case KEY_PRESS_ENTER:
				useVaccine();
				break;
			}
		}
	
	return;
}

//---------------Zombie-------------------------
Zombie::Zombie(double level_x, double level_y, StudentWorld* p)
	: Agent(IID_ZOMBIE,  level_x,  level_y, right, 0,p) 
{
	plan_distance = 0;
};
int Zombie::getPlan()const {
	return plan_distance;
}
void Zombie::getNewPlan() {
	plan_distance = randInt(3, 10);
}

bool Zombie::paralyzed() {
	int a = getWorld()->getTicks();
	if (a % 2 == 0) {
		return true;
	}
	return false;
}
void Zombie::doSomething() {
	if (!isAlive()) {
		return;
	}
	if (paralyzed()) {
		return;
	}
	if (checkfront()) {
		return;
	}
	
	if (getPlan() == 0) {
		getNewPlan();
		doSomethingSpecific();
		
	}
	
	if (tryMoveBy(1)) {
		plan_distance--;
	}
	else {
		plan_distance = 0;
	}
	
}

bool Zombie::checkfront() {
	int a = getDirection();
	int b = randInt(1, 3);
	switch (a) {
	case left:
		if (getWorld()->triggerVomit(getX()-SPRITE_WIDTH, getY()) ){
			if (b == 1) {
				getWorld()->addVomit(getX() - SPRITE_WIDTH, getY(), left);
				return true;
			}
		 }
		break;
	case right:
		if (getWorld()->triggerVomit(getX() + SPRITE_WIDTH, getY())) {
			if (b == 1) {
				getWorld()->addVomit(getX() + SPRITE_WIDTH, getY(), right);
				return true;
			}

		}
		break;
	case up:
		if (getWorld()->triggerVomit(getX() , getY() + SPRITE_WIDTH)) {
			if (b == 1) {
				getWorld()->addVomit(getX(), getY() + SPRITE_WIDTH, up);
				return true;
			}

		}
		break;
	case down:
		if (getWorld()->triggerVomit(getX() , getY() - SPRITE_WIDTH)) {
			if (b == 1) { 
				getWorld()->addVomit(getX(), getY() - SPRITE_WIDTH, down);
				return true;
			}

		}
		break;

	}
	return false;
}

//-----------Dumb_Zombie--------------------------
Dumb_Zombie::Dumb_Zombie(double  level_x, double  level_y, StudentWorld* p) : Zombie(level_x, level_y, p)
{}
void Dumb_Zombie::doSomethingSpecific() {
	setRandomDirection();
	
}

//--------------Smart_Zombie------------------------
Smart_Zombie::Smart_Zombie(double level_x, double  level_y, StudentWorld* p) : Zombie(level_x, level_y, p)
{}
void Smart_Zombie::doSomethingSpecific() {
	
	double xmin = 100;
	double ymin = 100;
	double dmin=getWorld()->getClosestPersonDistance( getX(), getY(),  xmin,  ymin);
	if (dmin > 80) {
		setRandomDirection();
	}
	else {
		if (getX() == xmin) {
			if (getY() > ymin) {
				setDirection(down);
			}
			else {
				setDirection(up);
			}
		}
		else if (getY() == ymin) {
			if (getX() > xmin) {
				setDirection(left);
			}
			else {
				setDirection(right);
			}

		}
		else {
			int r = randInt(1, 2);
			if ((getX() > xmin) && (getY() > ymin))
			{
				if (r == 1) {
					setDirection(left);
				}
				else {
					setDirection(down);
				}
			}
			else if ((getX() > xmin) && (getY() < ymin)) {
				if (r == 1) {
					setDirection(left);
				}
				else {
					setDirection(up);
				}
			}
			else if ((getX() < xmin) && (getY() > ymin)) {
				if (r == 1) {
					setDirection(right);
				}
				else {
					setDirection(down);
				}
			}
			else if ((getX() <xmin) && (getY() < ymin)) {
				if (r == 1) {
					setDirection(right);
				}
				else {
					setDirection(up);
				}
			}
		}
	}
}




//---------------------Wall--------------------
Wall::Wall(double  level_x, double  level_y, StudentWorld* p)
:Actor(IID_WALL,level_x,level_y, right, 0, p)
{}

void Wall::doSomething() {
	return;
}//does nothing;


//------------activatingObject--------------
ActivatingObject::ActivatingObject(int ID, double  level_x, double  level_y, int direction, int depth, StudentWorld* p)
:Actor(ID, level_x, level_y, direction, depth, p) {
	m_time = 0;
}

void ActivatingObject::IncrementTime(){
	m_time++;
}
int ActivatingObject::getTime() {
	return m_time;
}
//---------------Exit--------------------

Exit::Exit(int level_x, int level_y, StudentWorld* p):ActivatingObject(IID_EXIT, level_x, level_y, right, 1, p) {
	
}

void Exit::doSomething() {
	getWorld()->checkExitPlayer(getX(), getY());
	getWorld()->checkExit(getX(), getY());
	return;

}
//--------------Pit---------------------------
Pit::Pit(int level_x, int level_y, StudentWorld* p): ActivatingObject(IID_PIT, level_x, level_y, right, 1, p)
{}
void Pit::doSomething() {
	getWorld()->checkFallinginPitOrBurning(getX(), getY());
	return;
}
//--------------Flame-----------------------
Flame::Flame(int level_x, int level_y,int dir, StudentWorld* p):ActivatingObject(IID_FLAME, level_x, level_y, dir,0,p)
{}
void Flame::doSomething() {
	if (!isAlive()) {
		return;
	}
	getWorld()->checkFallinginPitOrBurning(getX(), getY());
	IncrementTime();
	if (getTime() == 2) {
		setDead();
	}
 }




//---------------Vomit-----------------
Vomit::Vomit(int level_x, int level_y, int dir, StudentWorld* p) :ActivatingObject(IID_VOMIT, level_x, level_y, dir, 0, p)
{}
void Vomit::doSomething() {
	if (!isAlive()) {
		return;
	}
	getWorld()->checkInfecting(getX(), getY());
	IncrementTime();
	if (getTime() == 2) {
		setDead();
	}
	
}
//--------------Landmine---------------
Landmine::Landmine(int level_x, int level_y, StudentWorld* p) :ActivatingObject(IID_LANDMINE, level_x, level_y, right, 1, p) {
	active = false;
	m_ticks = 30;
	
}
void Landmine::doSomething() {
	if (!isAlive()) {
		return;
	}
	if (!isActive()) {
		decrementTicks();
		if (getTicks() == 0) {
			setActive();
		}
		return;
	}
	if (getWorld()->landmineBeTriggered(getX(), getY())) {
		setDead();
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= i; j++) {
				if (!getWorld()->isFlameBlockedAt(getX() + i * SPRITE_WIDTH, getY() + j * SPRITE_HEIGHT)) {
					getWorld()->addFlame(getX() + i * SPRITE_WIDTH, getY() + j * SPRITE_HEIGHT,up);
				}
			}
		}
		getWorld()->addPit(getX(), getY());
		getWorld()->playSound(SOUND_LANDMINE_EXPLODE);
	}


}
//---------------Goodie----------------
Goodie::Goodie(int ID,int level_x, int level_y, StudentWorld* p):ActivatingObject(ID,level_x,level_y,right,1,p) {

}
void Goodie::doSomething() {
	if (!isAlive()) {
		return;
	}
	bePickedUp();
}
void Goodie::bePickedUp() {
	
	if (getWorld()->Overlap(getWorld()->PlayerX(), getWorld()->PlayerY(),getX(),getY())) {
		bePickedSpecific();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(50);
		this->setDead();
	}
}

//---------------Vaccine_Goodie------------------------
Vaccine_Goodie::Vaccine_Goodie(int level_x, int level_y, StudentWorld* p):Goodie(IID_VACCINE_GOODIE,  level_x,  level_y,  p) {

}
void Vaccine_Goodie::bePickedSpecific() {
	getWorld()->playergetVaccine();
 }
//---------------Flame_Goodie-------------------------
Flame_Goodie::Flame_Goodie(int level_x, int level_y, StudentWorld* p) : Goodie(IID_GAS_CAN_GOODIE, level_x, level_y, p) {

}
void Flame_Goodie::bePickedSpecific() {
	getWorld()->playergetFlame();
}
//-------------Landmine_Goodie---------------------------
Landmine_Goodie::Landmine_Goodie(int level_x, int level_y, StudentWorld* p) : Goodie(IID_LANDMINE_GOODIE, level_x, level_y, p) {

}
void Landmine_Goodie::bePickedSpecific() {
	getWorld()->playergetLandmine();
}