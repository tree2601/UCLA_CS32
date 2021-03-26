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
	bool isValid(int x, int y);
	
	int getPlayerX();
	int getPlayerY();
	bool Overlap(int x1, int y1, int x2, int y2);
	bool Intersect(int x1, int y1, int x2, int y2);
	bool LevelFinished();
	void FinishLevel();

private:
	Penelope* player; // added by me;
	vector<Actor*> objects ;
	int scores;
	bool level_finished;
	
	//vector<string> m_filenames;
};

#endif // STUDENTWORLD_H_
