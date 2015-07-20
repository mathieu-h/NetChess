#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define BUFFER_MAX 1024


class Client
{
public:
	Client(bool isSpectator, std::string ip, int port);
	~Client(void);

	void Update();
	void SendMove(bool chessmanTeam, int chessmanIndex, int x, int y);

	std::vector<std::string> buffer;

private:
	std::string Receive();
	void Send(const std::string& buffer);

	SOCKET socketClient;

};

