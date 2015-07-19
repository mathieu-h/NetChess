#include "Chessman.h"


Chessman::Chessman(int X, int Y, bool team_): x(X), y(Y), team(team_)
{
	isAlive = true;

	image1.allocate(80, 80, OF_IMAGE_COLOR);
	image1.setColor(ofColor::white);
	image1.update();

	image2.allocate(70, 70, OF_IMAGE_COLOR);
	image2.setColor(ofColor::white);
	image2.update();
}


Chessman::~Chessman(void)
{
}


void Chessman::draw()
{
	if (isAlive) {
		ofSetColor(team ? ofColor::black : ofColor::white);
		image1.draw(x * 100 + 10, y * 100 + 10);
		ofSetColor(team ? ofColor::white : ofColor::black);
		image2.draw(x *100 + 15, y * 100 + 15);
	}
}