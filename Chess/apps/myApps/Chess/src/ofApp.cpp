#include "ofApp.h"

//-------------------------------------------------------------
void ofApp::SetNetwork(int mode, string ip, int port)
{
	switch(mode) {
	case 1:
		LaunchServer(port);
		break;
	case 2:
		JoinServer(ip, port);
		break;
	case 3:
		SpectateGame(ip, port);
		break;
	}
}

//-------------------------------------------------------------
void ofApp::LaunchServer(int port)
{
	isServer = true;
	isClient = false;
	isSpectator = false;

	server = new Server(port);

	playerTeam = true;
	gameStart = false;
}

//-------------------------------------------------------------
void ofApp::JoinServer(string ip, int port)
{
	isServer = false;
	isClient = true;
	isSpectator = false;

	client = new Client(false, ip, port);

	playerTeam = false;
	gameStart = true;
}

//-------------------------------------------------------------
void ofApp::SpectateGame(string ip, int port)
{
	isServer = false;
	isClient = true;
	isSpectator = true;

	client = new Client(true, ip, port);
}

//--------------------------------------------------------------
void ofApp::setup()
{
	playerTurn = false;

	InitChessmen();

	selectedChessman = -1;

	if (!isSpectator && !playerTurn)
		cout << "Waiting opponent" << endl;

	selectionImage.loadImage("selection.png");
}

//--------------------------------------------------------------
void ofApp::update()
{
	if (isServer) {
		server->Update();
		for(int i = 0; i < server->buffer.size(); ++i)
			HandleBuffer(server->buffer[i]);
		server->buffer.clear();
		if (!gameStart && server->opponentConnected) {
			playerTurn = true;
			gameStart = true;
		}
		if (server->socketNewSpectators.size())
			SendGameStateToNewSpectator();
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

	if (selectedChessman > -1) {
		ofSetColor(ofColor::white);
		if (playerTeam)
			selectionImage.draw(chessmenWhites[selectedChessman].x * 100, chessmenWhites[selectedChessman].y * 100);
		else
			selectionImage.draw(chessmenBlacks[selectedChessman].x * 100, chessmenBlacks[selectedChessman].y * 100);
	}
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
				if (!MoveSelectedChessman(x, y))
					cout << "invalid move" << endl;
				selectedChessman = -1;
			}
		}
		else if (button == 2) {
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


void ofApp::HandleBuffer(std::string& _str)
{
	switch(_str[0])
	{
	case '0': // message
		HandleMessage(_str);
		break;
	case '2': // move
		HandleMove(_str);
		break;
	case '5': // end
		break;
	case '6': // set game state
		SetGameState(_str);
		break;
	}
}


void ofApp::HandleMessage(std::string& _str)
{
	_str[0] = ' ';
	cout << _str << endl;
}


void ofApp::HandleMove(std::string& _str)
{
	bool chessmanTeam;
	if (_str[1] == '0')
		chessmanTeam = true;
	else
		chessmanTeam = false;

	int chessmanIndex = _str[2] - 1;
	int x = _str[3] - 1;
	int y = _str[4] - 1;

	MoveChessman(chessmanTeam, chessmanIndex, x, y);
}


void ofApp::InitChessmen()
{
	chessmenWhites.push_back(Chessman(1, 4, 7, true));
	chessmenWhites.push_back(Chessman(2, 3, 7, true));
	chessmenWhites.push_back(Chessman(3, 0, 7, true));
	chessmenWhites.push_back(Chessman(3, 7, 7, true));
	chessmenWhites.push_back(Chessman(4, 1, 7, true));
	chessmenWhites.push_back(Chessman(4, 6, 7, true));
	chessmenWhites.push_back(Chessman(5, 2, 7, true));
	chessmenWhites.push_back(Chessman(5, 5, 7, true));

	chessmenBlacks.push_back(Chessman(1, 4, 0, false));
	chessmenBlacks.push_back(Chessman(2, 3, 0, false));
	chessmenBlacks.push_back(Chessman(3, 0, 0, false));
	chessmenBlacks.push_back(Chessman(3, 7, 0, false));
	chessmenBlacks.push_back(Chessman(4, 1, 0, false));
	chessmenBlacks.push_back(Chessman(4, 6, 0, false));
	chessmenBlacks.push_back(Chessman(5, 2, 0, false));
	chessmenBlacks.push_back(Chessman(5, 5, 0, false));

	for (int i = 0; i < 8; ++i) {
		chessmenWhites.push_back(Chessman(0, i, 6, true));
		chessmenBlacks.push_back(Chessman(0, i, 1, false));
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
	if (playerTeam) {
		if (!chessmenWhites[selectedChessman].CheckMove(x, y, &chessmenWhites, &chessmenBlacks))
			return false;
	}
	else {
		if (!chessmenBlacks[selectedChessman].CheckMove(x, y, &chessmenWhites, &chessmenBlacks))
			return false;
	}

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

	if (isServer)
		server->SendMoveToSpectators(team, index, x, y);

	bool end = false;

	if (team) {
		if (!chessmenBlacks[0].isAlive) {
			end = true;
			cout << "Whites win" << endl;
		}
	}
	else {
		if (!chessmenWhites[0].isAlive) {
			end = true;
			cout << "Blacks win" << endl;
		}
	}
	if (!end) {
		if (CheckCheck(true)) 
			cout << "Whites is check" << endl;
		if (CheckCheck(false)) 
			cout << "Blacks is check" << endl;

		if (!isSpectator) {
			playerTurn = !(team == playerTeam);
			if (playerTurn)
				cout << "It's your turn" << endl;
			else
				cout << "Waiting opponent's move" << endl;
		}
		else
			playerTurn = false;
	}
}


void ofApp::SendGameStateToNewSpectator()
{
	string str = "6";
	for (int i = 0; i < chessmenWhites.size(); ++i) {
		if (chessmenWhites[i].isAlive) {
			if (chessmenWhites[i].team)
				str += "0";
			else
				str += "1";
	
			str += i + 1;
			str += chessmenWhites[i].x + 1;
			str += chessmenWhites[i].y + 1;
		}
	}
	for (int i = 0; i < chessmenBlacks.size(); ++i) {
		if (chessmenBlacks[i].isAlive) {
			if (chessmenBlacks[i].team)
				str += "0";
			else
				str += "1";
	
			str += i + 1;
			str += chessmenBlacks[i].x + 1;
			str += chessmenBlacks[i].y + 1;
		}
	}
	server->SendToNewSpectator(str);
}


void ofApp::SetGameState(std::string& _str)
{
	for (int i = 0; i < chessmenWhites.size(); ++i)
		chessmenWhites[i].isAlive = false;

	for (int i = 0; i < chessmenBlacks.size(); ++i)
		chessmenBlacks[i].isAlive = false;

	int dataSize = (_str.size() - 1) / 4;
	for (int i = 0; i < dataSize; ++i) {
		int index = 1 + i * 4;
		bool chessmanTeam;
		if (_str[index] == '0')
			chessmanTeam = true;
		else 
			chessmanTeam = false;

		int chessmanIndex = _str[index+1] - 1;
		int x = _str[index+2] - 1;
		int y = _str[index+3] - 1;

		SetChessman(chessmanTeam, chessmanIndex, x, y);
	}
}


void ofApp::SetChessman(bool team, int index, int x, int y)
{
	if (team) {
		chessmenWhites[index].isAlive = true;
		chessmenWhites[index].x = x;
		chessmenWhites[index].y = y;
	}
	else {
		chessmenBlacks[index].isAlive = true;
		chessmenBlacks[index].x = x;
		chessmenBlacks[index].y = y;
	}
}


bool ofApp::CheckCheck(bool roiTeam)
{
	if (roiTeam) {
		for (int i = 0; i < chessmenBlacks.size(); ++i) {
			if (chessmenBlacks[i].isAlive && chessmenBlacks[i].CheckMove(chessmenWhites[0].x, chessmenWhites[0].y, &chessmenWhites, &chessmenBlacks))
				return true;
		}
	}
	else {
		for (int i = 0; i < chessmenWhites.size(); ++i) {
			if (chessmenWhites[i].isAlive && chessmenWhites[i].CheckMove(chessmenBlacks[0].x, chessmenBlacks[0].y, &chessmenWhites, &chessmenBlacks))
				return true;
		}
	}
	return false;
}
