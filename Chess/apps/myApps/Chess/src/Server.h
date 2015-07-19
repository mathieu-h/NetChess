#pragma once

#define WIN32_LEAN_AND_MEAN

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <vector>
#include <string>

#define DEFAULT_ADDRESS "127.0.0.1"
#define DEFAULT_PORT 27015
#define BUFFER_MAX 1024
#define SPECTATOR_MAX 5


class Server
{
public:
	Server(void);
	~Server(void);

	void Update();
	void SendMove(bool chessmanTeam, int chessmanIndex, int x, int y);

	std::vector<std::string> buffer;

private:
	std::string Receive(SOCKET socket);
	void OnUserConnected(SOCKET socket, const std::string& connectMode);
	void DisconnetClient(SOCKET socket, const std::string message);
	void Send(const std::string& _buffer);
	
	SOCKET socketServer;
	SOCKET socketOpponent;
	std::vector<SOCKET> socketSpectators;

};

