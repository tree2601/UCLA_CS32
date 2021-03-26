#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
//added by me;

class StudentWorld;

class Actor: public	GraphObject{ 
public:		
	Actor(int ID, int level_x, int level_y, int direction, int depth, StudentWorld* p);
virtual ~Actor()
{}

virtual void doSomething() = 0;
virtual bool isPlayer() const;
virtual bool isWall()const;
virtual StudentWorld* getWorld()const;
private: 
	StudentWorld* m_ptr;

};

//----------------Penelope----------------------
class Penelope : public Actor {
public:
	Penelope(int level_x, int level_y, StudentWorld* p);
	virtual void doSomething();
	virtual void move();
	virtual bool isAlive() const;
	virtual bool isInfected() const;
	virtual void setDead();
	virtual bool isPlayer() const;
	bool moveTowards(int ch);
	
	
		
private:
	bool alive;
	bool infected;
	int infection_count;
	int landmine;
	int flame;
	int vaccines;
	
};

//----------------------Wall--------------------
class Wall :public Actor {
public:

	Wall(int level_x, int level_y, StudentWorld* p);
	virtual void doSomething();
	virtual bool isWall()const;
	
private:
};
//-------------Exit-----------------
class Exit :public Actor {
public:
	Exit(int level_x, int level_y, StudentWorld* p);
	//Exit(int level_x, int level_y, StudentWorld* p);
	virtual void doSomething();
private:

};



// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
