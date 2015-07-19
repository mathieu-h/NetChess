#include "BoardGame.h"


BoardGame::BoardGame(void)
{
	backbround.allocate(100, 100, OF_IMAGE_COLOR);
	backbround.setColor(ofColor::white);
	backbround.update();
}


BoardGame::~BoardGame(void)
{
}


void BoardGame::draw()
{
	for(int i = 0; i < WIDTH; ++i) {
		for(int j = 0; j < WIDTH; ++j) {
			ofSetColor((i + j) % 2 ? ofColor::white : ofColor::black);
			backbround.draw(i * 100, j * 100);
		}
	}
}