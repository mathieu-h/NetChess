#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	SelectMode();
	InitChessmen();

	selectedChessman = -1;

	if (playerTurn)
		cout << "It's your turn" << endl;
	else
		cout << "Waiting opponent's move" << endl;
}

//--------------------------------------------------------------
void ofApp::update()
{
	if (isServer) {
		server->Update();
		for(int i = 0; i < server->buffer.size(); ++i)
			HandleBuffer(server->buffer[i]);
		server->buffer.clear();
	}
	else if (isClient) {
		client->Update();
		for(int i = 0; i < client->buffer.size(); ++i)
			HandleBuffer(client->buffer[i]);
		client->buffer.clear();
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	boardGame.draw();

	for (int i = 0; i < chessmenWhites.size(); ++i)
		chessmenWhites[i].draw();

	for (int i = 0; i < chessmenBlacks.size(); ++i)
		chessmenBlacks[i].draw();
/*
	if (selectedChessman > -1) {
		if (playerTeam)

		else
			
	}*/
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
	if (!isSpectator && playerTurn) {
		if (button == 0) {
			TransformScreenToGridPosition(&x, &y);
			if (selectedChessman == -1) {
				int chessmanIndex;
				bool chessmanTeam;
				if (GetTeamChessmanAtPosition(x, y, &chessmanTeam, &chessmanIndex)) {
					if (chessmanTeam == playerTeam)
						selectedChessman = chessmanIndex;
				}
			}
			else {
				MoveSelectedChessman(x, y);
				selectedChessman = -1;
			}
		}
		else if (button == 1) {
			selectedChessman = -1;
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{ 
}

//-------------------------------------------------------------
void ofApp::SelectMode()
{
	cout << "- 1 : Create game" << endl << "- 2 : Join game" << endl << "- 3 : Spectate game" << endl;
	int choice = 0;
	while (choice != 1 && choice != 2 && choice != 3) {
		cin >> choice;
	}
	switch(choice) {
	case 1:
		LaunchServer();
		break;
	case 2:
		JoinServer();
		break;
	case 3:
		SpectateGame();
		break;
	}
}

//-------------------------------------------------------------
void ofApp::LaunchServer()
{
	isServer = true;
	isClient = false;
	isSpectator = false;

	server = new Server();

	playerTeam = true;
	playerTurn = true;
}

//-------------------------------------------------------------
void ofApp::JoinServer()
{
	isServer = false;
	isClient = true;
	isSpectator = false;

	client = new Client(false);

	playerTeam = false;
	playerTurn = false;
}

//-------------------------------------------------------------
void ofApp::SpectateGame()
{
	isServer = false;
	isClient = true;
	isSpectator = true;

	client = new Client(true);
}


void ofApp::HandleBuffer(const std::string& _str)
{
	switch(_str[0])
	{
	case '0': // message
		break;
	case '1': // start game
		break;
	case '2': // move
		HandleMove(_str);
		break;
	case '3': // invalid move
		break;
	case '4': // player turn
		break;
	case '5': // end
		break;
	}
}


void ofApp::HandleMove(const std::string& _str)
{
	bool chessmanTeam;
	if (_str[1] == '0')
		chessmanTeam = true;
	else
		chessmanTeam = false;

	int chessmanIndex = _str[2];
	int x = _str[3];
	int y = _str[4];

	MoveChessman(chessmanTeam, chessmanIndex, x, y);
}


void ofApp::InitChessmen()
{
	for (int i = 0; i < 8; ++i) {
		chessmenWhites.push_back(Chessman(i, 7, true));
		chessmenBlacks.push_back(Chessman(i, 0, false));
	}
}


bool ofApp::TransformScreenToGridPosition(int* x, int* y)
{
	if(*x < 0 || *x > 800 || *y < 0 || *y > 800)
		return false;

	*x /= 100;
	*y /= 100;
	return true;
}


bool ofApp::GetChessmanAtPosition(int x, int y, bool* team, int* index)
{
	for (int i = 0; i < chessmenWhites.size(); ++i) {
		if (chessmenWhites[i].isAlive && chessmenWhites[i].x == x && chessmenWhites[i].y == y) {
			*team = chessmenWhites[i].team;
			*index = i;
			return true;
		}
	}

	for (int i = 0; i < chessmenBlacks.size(); ++i) {
		if (chessmenBlacks[i].isAlive && chessmenBlacks[i].x == x && chessmenBlacks[i].y == y) {
			*team = chessmenBlacks[i].team;
			*index = i;
			return true;
		}
	}

	return false;
}


bool ofApp::GetTeamChessmanAtPosition(int x, int y, bool* team, int* index)
{
	if (playerTeam) {
		for (int i = 0; i < chessmenWhites.size(); ++i) {
			if (chessmenWhites[i].isAlive && chessmenWhites[i].x == x && chessmenWhites[i].y == y) {
				*team = chessmenWhites[i].team;
				*index = i;
				return true;
			}
		}
	}
	else {
		for (int i = 0; i < chessmenBlacks.size(); ++i) {
			if (chessmenBlacks[i].isAlive && chessmenBlacks[i].x == x && chessmenBlacks[i].y == y) {
				*team = chessmenBlacks[i].team;
				*index = i;
				return true;
			}
		}
	}
	return false;
}


bool ofApp::MoveSelectedChessman(int x, int y)
{
	bool team;
	int index;
	if (GetChessmanAtPosition(x, y, &team, &index)) {
		if (team == playerTeam)
			return false;
	}

	MoveChessman(playerTeam, selectedChessman, x, y);

	if (isServer) {
		server->SendMove(playerTeam, selectedChessman, x, y);
	}
	else if (isClient) {
		client->SendMove(playerTeam, selectedChessman, x, y);
	}
	return true;
}


void ofApp::MoveChessman(bool team, int index, int x, int y)
{
	bool team_;
	int index_;
	if (GetChessmanAtPosition(x, y, &team_, &index_)) {
		if (team_) {
			chessmenWhites[index_].isAlive = false;
		}
		else {
			chessmenBlacks[index_].isAlive = false;
		}
	}

	if (team) {
		chessmenWhites[index].x = x;
		chessmenWhites[index].y = y;
	}
	else {
		chessmenBlacks[index].x = x;
		chessmenBlacks[index].y = y;
	}

	playerTurn = !(team == playerTeam);
	if (playerTurn)
		cout << "It's your turn" << endl;
	else
		cout << "Waiting opponent's move" << endl;
}

