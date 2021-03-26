#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>

#include"Actor.h" // added by me;
#include"Level.h"// added by me;
#include<iostream>
#include<sstream>
#include<cmath>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath),player(nullptr), level_finished(false)
{
	
}
StudentWorld::~StudentWorld() {
	cleanUp();
}
int StudentWorld::init()
{
	
	Level lev(assetPath());
	string levelFile = "level02.txt";
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
					   break;
				   case Level::smart_zombie:
					   break;
				   case Level::citizen:
					   break;
				   case Level::wall:
					   objects.push_back(new Wall(SPRITE_WIDTH *i, SPRITE_WIDTH *j, this));
					   break;
				   case Level::pit:
					   break;
				   case Level::vaccine_goodie:
					   break;
				   case Level::gas_can_goodie:
					   break;
				   case Level::landmine_goodie:
					   break;

				   }
			   }
		   }
		   
			return GWSTATUS_CONTINUE_GAME;
	}
}


int StudentWorld::move()
{
	player->doSomething();
	
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->doSomething();
		if (!player->isAlive()) {
			decLives();
			return  GWSTATUS_PLAYER_DIED;
		}
		else if (LevelFinished()) {
			return GWSTATUS_PLAYER_WON;
		}
	}
		
     
		
		
	
	
	// This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    return  GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::isValid(int x, int y) {
	
	for (int i= 0; i<objects.size(); i++) {
		if (objects[i]->isWall()) {
		  if (Intersect(x,y,objects[i]->getX(),objects[i]->getY())) {
			  return false;
		  }
		}
	}
		return true;
}
int StudentWorld::getPlayerX() {
	return player->getX();
}
int StudentWorld::getPlayerY() {
	return player->getY();
}

bool StudentWorld::Intersect(int x1, int y1, int x2, int y2) {
	if (x1 + SPRITE_WIDTH - 1 <= x2 || x2 + SPRITE_WIDTH <= x1) {
		return false;
	}
	if (y1 + SPRITE_WIDTH - 1 <= y2 || y2 + SPRITE_WIDTH - 1 <= y1) {
		return false;
	}
	return true;
}
bool StudentWorld::Overlap(int x1, int y1, int x2, int y2) {
	if (((x1 - x2)*(x1 - x2)) + ((y1 - y2)*(y1 - y2)) > 100) {
		return false;
	}
	return true;
}

bool StudentWorld::LevelFinished() {
	return level_finished;
}
void StudentWorld::FinishLevel() {
	level_finished = true;
}

void StudentWorld::cleanUp()
{
	delete player;
	for (int i = 0; i < objects.size(); i++) {
		delete objects[i];
	}
}
