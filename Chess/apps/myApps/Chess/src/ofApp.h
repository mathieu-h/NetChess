#pragma once

#include "ofMain.h"
#include "Server.h"
#include "Client.h"
#include "BoardGame.h"
#include "Chessman.h"


class ofApp : public ofBaseApp{

	public:
		void SetNetwork(int mode, string ip, int port);
		void LaunchServer(int port);
		void JoinServer(string ip, int port);
		void SpectateGame(string ip, int port);

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void HandleBuffer(std::string& _str);
		void HandleMessage(std::string& _str);
		void HandleMove(std::string& _str);
		void SetGameState(std::string& _str);

		void InitChessmen();

		bool TransformScreenToGridPosition(int* x, int* y);
		bool GetChessmanAtPosition(int x, int y, bool* team, int* index);
		bool GetTeamChessmanAtPosition(int x, int y, bool* team, int* index);
		bool MoveSelectedChessman(int x, int y);
		void MoveChessman(bool team, int index, int x, int y);
		void SendGameStateToNewSpectator();
		void SetChessman(bool team, int index, int x, int y);
		bool CheckCheck(bool roiTeam);

		bool isServer;
		bool isClient;
		bool isSpectator;

		Server* server;
		Client* client;

		BoardGame boardGame;
		vector<Chessman> chessmenWhites;
		vector<Chessman> chessmenBlacks;

		bool playerTeam;

		bool playerTurn;
		bool gameStart;

		int selectedChessman;

		ofImage selectionImage;
};
