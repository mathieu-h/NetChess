#include "Chessman.h"


Chessman::Chessman(int type_, int X, int Y, bool team_): type(type_), x(X), y(Y), team(team_)
{
	if (type < 0 || type > 5)
		type = 0;

	isAlive = true;

	switch (type)
	{
	case 0:
		image.loadImage("pion.png");
		imageBorder.loadImage("pionBorder.png");
		break;
	case 1:
		image.loadImage("roi.png");
		imageBorder.loadImage("roiBorder.png");
		break;
	case 2:
		image.loadImage("reine.png");
		imageBorder.loadImage("reineBorder.png");
		break;
	case 3:
		image.loadImage("tour.png");
		imageBorder.loadImage("tourBorder.png");
		break;
	case 4:
		image.loadImage("cavalier.png");
		imageBorder.loadImage("cavalierBorder.png");
		break;
	case 5:
		image.loadImage("fou.png");
		imageBorder.loadImage("fouBorder.png");
		break;
	}
}


Chessman::~Chessman(void)
{
}


void Chessman::draw()
{
	if (isAlive) {
		ofSetColor(team ? ofColor::black : ofColor::white);
		imageBorder.draw(x * 100, y * 100);
		ofSetColor(team ? ofColor::white : ofColor::black);
		image.draw(x *100, y * 100);
	}
}


bool Chessman::CheckMove(int X, int Y, vector<Chessman>* chessmenWhites, vector<Chessman>* chessmenBlacks)
{
	if (X == x && Y == y
		|| X < 0
		|| X > 7
		|| Y < 0
		|| Y > 7)
	{
		return false;
	}

	int deltaX = X - x;
	int deltaY = Y - y;
	switch (type)
	{
	// handle pion move
	case 0:
		if (deltaX < -1 || deltaX > 1 )
			return false;
		if (deltaX != 0) {
			if (team && deltaY == -1 || !team && deltaY == 1) {
				bool teamAtPos;
				if (GetChessmanTeamAtPosition(X, Y, chessmenWhites, chessmenBlacks, &teamAtPos)) {
					if (teamAtPos != team)
						return true;
				}
			}
			return false;
		}
		
		if (team) {
			if (y == 6) {
				if (deltaY != -2 && deltaY != -1)
					return false;
			}
			else if (deltaY != -1)
				return false;
		}
		else {
			if (y == 1) {
				if (deltaY != 2 && deltaY != 1)
					return false;
			}
			else if (deltaY != 1)
				return false;
		}
		
		if (deltaY == 2 || deltaY == -2) {
			if (CheckChessmanAtPosition(X, y + deltaY/2, chessmenWhites, chessmenBlacks))
				return false;
		}
		if (CheckChessmanAtPosition(X, Y, chessmenWhites, chessmenBlacks))
			return false;
		return true;
	// ------------------------------

	// handle roi move
	case 1:
		if (deltaX > 1 || deltaX < -1)
			return false;
		if (deltaY > 1 || deltaY < -1)
			return false;
		return true;
	// ------------------------------

	// handle reine move
	case 2:
		if (deltaX != 0 && deltaY != 0 && deltaX != deltaY && deltaX != -deltaY)
			return false;
		break;
	// ------------------------------

	// handle tour move
	case 3:
		if (deltaX != 0 && deltaY != 0)
			return false;
		break;
	// ------------------------------

	// handle cavalier move
	case 4:
		if ((deltaX == 2 || deltaX == -2) && (deltaY == 1 || deltaY == -1)
			|| (deltaX == 1 || deltaX == -1) && (deltaY == 2 || deltaY == -2))
			return true;
		return false;
	// ------------------------------

	// handle fou move
	case 5:
		if (deltaX != deltaY && deltaX != -deltaY)
			return false;
		break;
	}
	// ------------------------------

	int X_;
	if (deltaX > 0) X_ = 1;
	else if (deltaX < 0) X_ = -1;
	else X_ = 0;

	int Y_;
	if (deltaY > 0) Y_ = 1;
	else if (deltaY < 0) Y_ = -1;
	else Y_ = 0;

	int x_ = x + X_;
	int y_ = y + Y_;
	while (x_ != X || y_ != Y ) {
		if (CheckChessmanAtPosition(x_, y_, chessmenWhites, chessmenBlacks))
			return false;
		x_ += X_;
		y_ += Y_;
	}

	return true;
}


bool Chessman::CheckChessmanAtPosition(int x, int y, vector<Chessman>* chessmenWhites, vector<Chessman>* chessmenBlacks)
{
	for (int i = 0; i < chessmenWhites->size(); ++i) {
		if (chessmenWhites->at(i).isAlive && chessmenWhites->at(i).x == x && chessmenWhites->at(i).y == y) {
			return true;
		}
	}

	for (int i = 0; i < chessmenBlacks->size(); ++i) {
		if (chessmenBlacks->at(i).isAlive && chessmenBlacks->at(i).x == x && chessmenBlacks->at(i).y == y) {
			return true;
		}
	}

	return false;
}


bool Chessman::GetChessmanTeamAtPosition(int x, int y, vector<Chessman>* chessmenWhites, vector<Chessman>* chessmenBlacks, bool* team)
{
	for (int i = 0; i < chessmenWhites->size(); ++i) {
		if (chessmenWhites->at(i).isAlive && chessmenWhites->at(i).x == x && chessmenWhites->at(i).y == y) {
			*team = chessmenWhites->at(i).team;
			return true;
		}
	}

	for (int i = 0; i < chessmenBlacks->size(); ++i) {
		if (chessmenBlacks->at(i).isAlive && chessmenBlacks->at(i).x == x && chessmenBlacks->at(i).y == y) {
			*team = chessmenBlacks->at(i).team;
			return true;
		}
	}

	return false;
}