#pragma once

#include "ofMain.h"

class Chessman
{
public:
	Chessman(int type_, int X, int Y, bool team_);
	~Chessman(void);

	void draw();
	bool CheckMove(int X, int Y, vector<Chessman>* chessmenWhites, vector<Chessman>* chessmenBlacks);

	int x;
	int y;
	bool team;
	bool isAlive;

	int type;

	// 0 : pion
	// 1 : roi
	// 2 : reine
	// 3 : tour
	// 4 : cavalier
	// 5 : fou

private:
	bool CheckChessmanAtPosition(int x, int y, vector<Chessman>* chessmenWhites, vector<Chessman>* chessmenBlacks);
	bool GetChessmanTeamAtPosition(int x, int y, vector<Chessman>* chessmenWhites, vector<Chessman>* chessmenBlacks, bool* team);
	
	ofImage image;
	ofImage imageBorder;

};

