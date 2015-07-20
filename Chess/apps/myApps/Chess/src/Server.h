#pragma once

#define WIN32_LEAN_AND_MEAN

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <vector>
#include <string>

#define BUFFER_MAX 1024
#define SPECTATOR_MAX 5


class Server
{
public:
	Server(int port);
	~Server(void);

	void Update();
	void SendMessageTo(SOCKET socket, const std::string message);
	void SendMove(bool chessmanTeam, int chessmanIndex, int x, int y);
	void SendMoveToSpectators(bool chessmanTeam, int chessmanIndex, int x, int y);
	void SendToNewSpectator(const std::string& _buffer);

	std::vector<std::string> buffer;
	std::vector<SOCKET> socketNewSpectators;
	bool opponentConnected;

private:
	std::string Receive(SOCKET socket);
	void OnUserConnected(SOCKET socket, const std::string& connectMode);
	void DisconnetClient(SOCKET socket, const std::string message);
	void Send(SOCKET socket, const std::string& _buffer);
	
	SOCKET socketServer;
	SOCKET socketOpponent;
	std::vector<SOCKET> socketSpectators;
};

