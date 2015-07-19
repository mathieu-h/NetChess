#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define DEFAULT_ADDRESS "127.0.0.1"
#define DEFAULT_PORT "27015"
#define BUFFER_MAX 1024


class Client
{
public:
	Client(bool isSpectator);
	~Client(void);

	void Update();
	void SendMove(bool chessmanTeam, int chessmanIndex, int x, int y);

	std::vector<std::string> buffer;

private:
	std::string Receive();
	void Send(const std::string& buffer);

	SOCKET socketClient;

};

