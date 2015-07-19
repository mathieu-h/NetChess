#pragma once

#include "ofMain.h"

class Chessman
{
public:
	Chessman(int X, int Y, bool team_);
	~Chessman(void);

	void draw();

	int x;
	int y;
	bool team;
	bool isAlive;

private:
	ofImage image1;
	ofImage image2;

};

