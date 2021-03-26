#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>

#include"Actor.h" // added by me;
#include"Level.h"// added by me;
#include<iostream>
#include<sstream>
#include<cmath>
#include<vector>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath),player(nullptr), level_finished(false),m_ticks(0), m_citizens(0)
{
	
}
StudentWorld::~StudentWorld() {
	//cleanUp();
}
int StudentWorld::init()
{
	
	Level lev(assetPath());
	string levelFile = "level01.txt";
	Level::LoadResult result = lev.loadLevel(levelFile);
	if (result == Level::load_fail_file_not_found) {
		return GWSTATUS_LEVEL_ERROR;
	}
	else if (result == Level::load_fail_bad_format) {
		return GWSTATUS_LEVEL_ERROR;
	}
	else if (result == Level::load_success) {
		   Level::MazeEntry ge;
		   for (int i = 0; i < LEVEL_WIDTH; i++) {
			   for (int j = 0; j < LEVEL_HEIGHT; j++) {
				   ge = lev.getContentsOf(i, j);
				   switch (ge) {
				   case Level::empty:
					   break;
				   case Level::exit:
					   objects.push_back(new Exit(SPRITE_WIDTH *i, SPRITE_WIDTH *j, this));
					   break;
				   case Level::player:
					   player = new Penelope(SPRITE_WIDTH *i, SPRITE_WIDTH *j, this);
					   break;
				   case Level::dumb_zombie:
					   objects.push_back(new Dumb_Zombie(SPRITE_WIDTH *i, SPRITE_WIDTH *j, this));
					   break;
				   case Level::smart_zombie:
					   objects.push_back(new Smart_Zombie(SPRITE_WIDTH *i, SPRITE_WIDTH *j, this));
					   break;
				   case Level::citizen:
					   objects.push_back(new Citizen(SPRITE_WIDTH *i, SPRITE_WIDTH *j, this));
					   m_citizens++;
					   break;
				   case Level::wall:
					   objects.push_back(new Wall(SPRITE_WIDTH *i, SPRITE_WIDTH *j, this));
					   break;
				   case Level::pit:
					   objects.push_back(new Pit(SPRITE_WIDTH *i, SPRITE_WIDTH *j, this));
					   break;
				   case Level::vaccine_goodie:
					   objects.push_back(new Vaccine_Goodie(SPRITE_WIDTH *i, SPRITE_WIDTH *j, this));
					   break;
				   case Level::gas_can_goodie:
					   objects.push_back(new Flame_Goodie(SPRITE_WIDTH *i, SPRITE_WIDTH *j, this));
					   break;
				   case Level::landmine_goodie:
					   objects.push_back(new Landmine_Goodie(SPRITE_WIDTH *i, SPRITE_WIDTH *j, this));
					   break;

				   }
			   }
		   }
		  //objects.push_back(new Vomit(SPRITE_WIDTH *5, SPRITE_WIDTH *5,90, this));
			return GWSTATUS_CONTINUE_GAME;
	}
}


int StudentWorld::move()
{
	StartLevel();
	m_ticks++;
	player->doSomething();
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i]->isAlive()) {
			objects[i]->doSomething();
			if (!player->isAlive()) {
				decLives();
				playSound(SOUND_PLAYER_DIE);
				return  GWSTATUS_PLAYER_DIED;
			}
		   if (LevelFinished()) {
			   playSound(SOUND_LEVEL_FINISHED);
				return  GWSTATUS_FINISHED_LEVEL;
			}
		}
	}
    
	for (vector<Actor*>::iterator it = objects.begin(); it != objects.end();)  
		if ((*it)->isAlive()==false) { 
			delete *it;
			it = objects.erase(it); }
		else {
			++it;
		}
	// This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    return  GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::isValid(double x, double y, Actor* it) {
	if (it != player) {
		if (intersectWithPenelope(x, y)) {
			return false;
		}
	}
	for (int i= 0; i<objects.size(); i++) {
		if (objects[i]->blocksMovement()) {
			if (objects[i] != it) {
				if (Intersect(x, y, objects[i]->getX(), objects[i]->getY())) {
					return false;
				}
			}
		}
	}
		return true;
}
bool StudentWorld::intersectWithPenelope(double x, double y) {
	return Intersect(x, y, PlayerX(), PlayerY());
}

bool StudentWorld::Intersect(double x1, double y1, double x2, double y2) {
	
	if (x1 + SPRITE_WIDTH - 1 <= x2 || x2 + SPRITE_WIDTH <= x1) {
		return false;
	}
	if (y1 + SPRITE_WIDTH - 1 <= y2 || y2 + SPRITE_WIDTH - 1 <= y1) {
		return false;
	}
	return true;
}
bool StudentWorld::Overlap(double x1, double y1, double x2, double y2) {
	if (((x1 - x2)*(x1 - x2)) + ((y1 - y2)*(y1 - y2)) > 100) {
		return false;
	}
	return true;
}



void StudentWorld::cleanUp()
{
	delete player;
	player = nullptr;
	vector<Actor*> ::iterator m_it;
	while (!objects.empty()) {
		delete *objects.begin();
		objects.erase(objects.begin());
	}
}
	
void StudentWorld::StartLevel() {
	level_finished = false;
}
bool StudentWorld::LevelFinished() {
	return level_finished;
}
void StudentWorld::FinishLevel() {
	level_finished = true;
}

void StudentWorld::checkFallinginPitOrBurning(double x, double y) {
	if (Overlap(x, y, player->getX(), player->getY())) {
	   player->setDead();

	}
	for(int i = 0; i < objects.size(); i++) {
		if(objects[i]->isAlive()&&objects[i]->canBeDamaged())
			if (Overlap(x, y, objects[i]->getX(), objects[i]->getY())) {
				objects[i]->setDead();
				if(objects[i]->canBeInfected()){ 
					m_citizens--;
					playSound(SOUND_CITIZEN_DIE);
					increaseScore(-1000);
				}
				if (objects[i]->beDumbZombie()) {
					playSound(SOUND_ZOMBIE_DIE);
					increaseScore(1000);
				}
				if (objects[i]->beSmartZombie()) {
					playSound(SOUND_ZOMBIE_DIE);
					increaseScore(2000);
				}
				
			}
	}

}
void StudentWorld::checkInfecting(double x, double y) {
	if (Overlap(x, y, player->getX(), player->getY())) {
		player->beVomitedOnIfAppropriate();
		

	}
	for (int i = 0; i < objects.size(); i++) {
		if (Overlap(x, y, objects[i]->getX(), objects[i]->getY())) {
			if (objects[i]->canBeInfected()) {
				objects[i]->beVomitedOnIfAppropriate();
				playSound(SOUND_CITIZEN_INFECTED);
			}
		}
	}

}

void StudentWorld::checkExit(double x, double y) {
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i]->isAlive() && objects[i]->canBeDamaged()&&objects[i]->canBeInfected())
			if (Overlap(x, y, objects[i]->getX(), objects[i]->getY())) {
				objects[i]->setDead();
				playSound(SOUND_CITIZEN_SAVED);
				increaseScore(1000);
				m_citizens--;
			}
	}
}

void StudentWorld::checkExitPlayer(double x, double y) {
	if (Overlap(player->getX(), player->getY(), x, y)) {
		if (getCitizens() == 0) {
			FinishLevel();
		}
	}
}
int StudentWorld::getTicks() const {
	return m_ticks;
}
int StudentWorld::getCitizens()const {
	return  m_citizens;
}
void StudentWorld::decrementCitizens() {
	m_citizens--;
}
double StudentWorld::PlayerX() {
	return player->getX();
}
double  StudentWorld::PlayerY() {
	return player->getY();
}

void StudentWorld::playergetVaccine() {
	player->adjustVaccine(1);
}
void StudentWorld::playergetFlame() {
	player->adjustFlame(5);
}
void StudentWorld::playergetLandmine() {
	player->adjustLandmine(2);
}

bool StudentWorld::triggerVomit(double x, double y) {
	if (Overlap(player->getX(), player->getY(), x, y)) {
		return true;
	}
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i]->canBeInfected()) {
			if (Overlap(objects[i]->getX(), objects[i]->getY(), x, y)) {
				return true;
			}
		}
	}
	return false;
}

void  StudentWorld::addVomit(double x, double y,int direction) {
	
	objects.push_back(new Vomit(x, y, direction, this));
	playSound(SOUND_ZOMBIE_VOMIT);
}
void StudentWorld::addFlame(double x, double y, int direction) {
	objects.push_back(new Flame(x, y, direction, this));
}
void StudentWorld::addLandMine(double x, double y) {
	objects.push_back(new Landmine(x, y, this));
}
void StudentWorld::addPit(double x, double y) {
	objects.push_back(new Pit(x, y, this));
}

double  StudentWorld::distance(double x1, double y1, double x2, double y2) {
	double d = (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
	return sqrt(d);
}
double StudentWorld::getClosestPersonDistance(double x, double y, double &xmin, double &ymin) {//no penelope
	double min = 10000;
	min = distance(x, y, PlayerX(), PlayerY());
	xmin = PlayerX();
	ymin = PlayerY();
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i]->canBeInfected()) {
			if (min>distance(x, y, objects[i]->getX(), objects[i]->getY())) {
				min = distance(x, y, objects[i]->getX(), objects[i]->getY());
				xmin = objects[i]->getX();
				ymin = objects[i]->getY();
			}
		}
	}
	return min;
}


double StudentWorld::getClosestZombieDistance(double x, double y, double &xmin, double &ymin) {
	double min = 10000;
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i]->beDumbZombie()||objects[i]->beSmartZombie()) {
			if (min > distance(x, y, objects[i]->getX(), objects[i]->getY())) {
				min = distance(x, y, objects[i]->getX(), objects[i]->getY());
				xmin = objects[i]->getX();
				ymin = objects[i]->getY();
			}
		}
	}
	return min;
}
void  StudentWorld::addZombie(double x, double y) {
	int r = randInt(1, 10);
	if(r<=3)
	{ 
		objects.push_back(new Smart_Zombie(x, y, this)); }
	else {
		objects.push_back(new Dumb_Zombie(x, y, this));
	}
	playSound(SOUND_ZOMBIE_BORN);
	
}

bool StudentWorld::isFlameBlockedAt(double x, double y)  {
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i]->blocksFlame()) {
			if (Overlap(x, y, objects[i]->getX(), objects[i]->getY())) {
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::landmineBeTriggered(double x, double y) {
	if(Overlap(x, y, player->getX(), player->getY())) {
		return true;

	}
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i]->triggersOnlyActiveLandmines()) {
			if (Overlap(x, y, objects[i]->getX(), objects[i]->getY())) {
				return true;
			}
		}
	}
	return false;
}