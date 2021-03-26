#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
//added by me;

class StudentWorld;

class Actor: public	GraphObject{ 
public:		
	Actor(int ID, double level_x, double level_y, int direction, int depth, StudentWorld* p);
	// Action to perform for each tick.
	virtual void doSomething() = 0;
	virtual bool blocksMovement() const {
		return false;
	};
	virtual bool canBeInfected()const {
		return false;
	}
	virtual bool canBeDamaged() const {
		return false;
	}
	virtual bool triggersZombieVomit() const {
		return false;
	}
	virtual bool canBeVomited()const {
		return false;
	}
	virtual bool blocksFlame() const {
		return false;
	}
	virtual void beVomitedOnIfAppropriate() {
		return;
	}
	virtual bool beSmartZombie() {
		return false;
	}
	virtual bool beDumbZombie() {
		return false;
	}
	virtual bool canLeadCitizens() {
		return false;
	}
	virtual bool triggersOnlyActiveLandmines() {
		return false;
	}
	

	
	virtual bool moveTowards(int ch);
	virtual bool isAlive() const;
	 void setDead();

	//virtual bool isInfected() const;
	//virtual void move();
	
virtual StudentWorld* getWorld()const;

private: 
	StudentWorld* m_ptr;
	bool m_alive;
	

};
//------------------Agent-------------------
class Agent :public Actor {
public:
	Agent(int ID, double level_x, double level_y, int direction, int depth, StudentWorld* p);
	virtual bool canBeDamaged() const {
		return true;
	}
	virtual bool blocksMovement() const {
		return true;
	};
	virtual bool paralyzed() {
		return false;
	}
	virtual bool triggersOnlyActiveLandmines() {
		return true;
	}
	virtual void setRandomDirection();
	virtual bool tryMoveBy(int d);
	
private:

};
//-----------------Human---------------------------
class Human :public Agent {
public:
	Human(int ID, double level_x, double level_y, int direction, int depth, StudentWorld* p);
	virtual bool isInfected() const {
		return infected;
	}
	virtual bool triggersZombieVomit() const {
		return true;
	}
	virtual bool canBeVomited()const {
		return true;
	}
	
	
	virtual bool canBeInfected()const {
		return true;
	}
	virtual void beVomitedOnIfAppropriate() {
		infected = true;
	}
	virtual void heal() {
		infected = false;
	}
	virtual bool paralyzed() {
		return false;
	}
	virtual void doSomethingSpecific()=0;
	virtual void doSomething();
	virtual int infectionCount() {
		return infection_count;
	}
	virtual void incrementInfection() {
		infection_count++;
	}
private:
	bool infected;
	int infection_count;

};
//------------Citizen----------------------
class Citizen :public Human {
public:
	Citizen(double level_x, double level_y, StudentWorld* p);
	virtual bool paralyzed();
	virtual void doSomethingSpecific();
	virtual bool citizenTryMove(Direction d);
		
	
private:

};

//----------------Penelope----------------------
class Penelope : public Human {
public:
	Penelope(double level_x, double level_y, StudentWorld* p);
	 virtual void doSomethingSpecific();
	 void adjustLandmine(int n);
	 void adjustVaccine(int n);
	 void adjustFlame(int n);
	 virtual bool canLeadCitizens() {
		 return true;
	 }
	 bool useVaccine();
	 bool useFlame();
	 bool useLandmine();
	 
	 
	
		
private:
	
	
	int landmines;
	int flames;
	int vaccines;
	
};


//----------Zombie------------------------
class Zombie :public Agent {
public:
	Zombie(double level_x, double level_y, StudentWorld* p);
	virtual int getPlan()const;
	void getNewPlan();
	virtual bool paralyzed();
	virtual void doSomething();
	virtual void doSomethingSpecific()=0;
	virtual bool checkfront();

private:
	int plan_distance;
};
//-----------Dumb Zombie---------------------
class Dumb_Zombie :public Zombie {
public:
	Dumb_Zombie(double level_x, double level_y, StudentWorld* p);
	virtual void doSomethingSpecific();
	virtual bool beDumbZombie() {
		return true;
	}
	
private:
};
//---------Smart Zombie-------------------------
class Smart_Zombie :public Zombie {
public:
	Smart_Zombie(double  level_x, double  level_y, StudentWorld* p);
	virtual void doSomethingSpecific();
	virtual bool beSmartZombie() {
		return true;
	}
	
private:
};
//----------------------Wall--------------------
class Wall :public Actor {
public:

	Wall(double  level_x, double  level_y, StudentWorld* p);
	void doSomething();
	virtual bool blocksMovement() const {
		return true;
	};
	virtual bool blocksFlame() const {
		return true;
	}
	
private:
};
//----------------activatingObject------------------
class ActivatingObject :public Actor {
public:
	ActivatingObject(int ID, double  level_x, double  level_y, int direction, int depth, StudentWorld* p);
	virtual void IncrementTime();
	virtual int getTime();
private:
	int m_time;
};
//-------------Exit-----------------


class Exit :public ActivatingObject{
public:
	Exit(int level_x, int level_y, StudentWorld* p);
	//Exit(int level_x, int level_y, StudentWorld* p);
	virtual bool blocksFlame() const {
		return true;
	}
	virtual void doSomething();
private:

};
//--------------Pit---------------------------
class Pit :public ActivatingObject {
public:
	Pit(int level_x, int level_y, StudentWorld* p);
	virtual void doSomething();
private:
	
};






//--------------Flame-----------------------
class Flame : public ActivatingObject {
public:
	Flame(int level_x, int level_y, int dir,StudentWorld* p);
	virtual void doSomething();
private:
	
};
//---------------Vomit-----------------
class Vomit : public ActivatingObject {
public:
	Vomit(int level_x, int level_y, int dir, StudentWorld* p);
	virtual void doSomething();
private:
};
//--------------Landmine---------------
class Landmine : public ActivatingObject {
public:
	Landmine(int level_x, int level_y, StudentWorld* p);
	virtual bool canBeDamaged() const {
		return true;
	}
	int getTicks() {
		return m_ticks;
	}
	void decrementTicks() {
		m_ticks--;
	}
	bool isActive() {
		return active;
	}
	void setActive() {
		active = true;
	}
	virtual void doSomething();
private:
	int m_ticks;
	bool active;
};
//---------------Goodie----------------
class Goodie : public ActivatingObject {
public:
	Goodie(int ID,int level_x, int level_y, StudentWorld* p);
	virtual void doSomething();
	void bePickedUp();
	virtual void bePickedSpecific() = 0;
private:
};

//----------------Vaccine goodie---------------
class Vaccine_Goodie :public Goodie {
public:
	Vaccine_Goodie(int level_x, int level_y, StudentWorld* p);
	virtual void bePickedSpecific();
private:

};
//------------------Flame goodie-----------------
class Flame_Goodie :public Goodie {
public:
	Flame_Goodie(int level_x, int level_y, StudentWorld* p);
	virtual void bePickedSpecific();
private:
};
//-----------------Landmine goodie-------------
class Landmine_Goodie :public Goodie {
public:
	Landmine_Goodie(int level_x, int level_y, StudentWorld* p);
	virtual void bePickedSpecific();
private:
};
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
