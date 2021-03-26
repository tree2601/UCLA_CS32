#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"

#include <string>
#include"Actor.h" //added by me;
#include<vector> //added by me;

using namespace std;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
	~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	virtual int getTicks() const;
	
	bool isValid(double x, double y, Actor* it);

	void checkFallinginPitOrBurning(double x, double y);
	void checkExit(double x,double y);
	void checkExitPlayer(double x, double y);
	void checkInfecting(double x, double y);
	bool triggerVomit(double x, double y);
	bool intersectWithPenelope(double x, double y);
	bool isFlameBlockedAt(double x, double y) ;
	bool landmineBeTriggered(double x, double y);


	void addVomit(double x, double y, int direction);
	void addZombie(double x, double y);
	void addFlame(double x, double y, int direction);
	void addLandMine(double x, double y);
	void addPit(double x, double y);
	
	bool Overlap(double x1, double y1, double x2, double y2);
	bool Intersect(double x1, double y1, double x2, double y2);
	double PlayerX();
	double PlayerY();
	
	void playergetVaccine();
	void playergetFlame();
	void playergetLandmine();

	bool LevelFinished();
	void FinishLevel();
	void StartLevel();
	int getCitizens()const;
	void decrementCitizens();

	double distance(double x1, double y1, double x2, double y2);
	double getClosestPersonDistance(double x, double y, double &xmin, double &ymin);
	
	double getClosestZombieDistance(double x, double y, double &xmin, double &ymin);

private:
	Penelope* player; // added by me;
	vector<Actor*> objects ;
	
	bool level_finished;
	int m_ticks;
	int m_citizens;
	
	//vector<string> m_filenames;
	
};

#endif // STUDENTWORLD_H_
