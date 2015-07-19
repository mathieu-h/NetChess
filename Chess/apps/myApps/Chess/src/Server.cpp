#include "Server.h"


Server::Server(void)
{
	socketServer = INVALID_SOCKET;
	socketOpponent = INVALID_SOCKET;

	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
		printf("WSAStartup failed: %d\n", result);

	socketServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketServer == INVALID_SOCKET)
		printf("Init server socket failed: %d\n", result);

	sockaddr_in addr;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(DEFAULT_PORT);
	addr.sin_family = AF_INET;
	result = bind(socketServer, (sockaddr*)&addr, sizeof(addr));
	if (result != 0)
		printf("Bind failed: %d\n", result);

	result = listen(socketServer, SOMAXCONN);
	if (result != 0)
		printf("Listen failed: %d\n", result);

	unsigned long iMode = 1;
	ioctlsocket(socketServer, FIONBIO, &iMode);
}


Server::~Server(void)
{
}


void Server::Update()
{
	for (;;)
	{
		SOCKET newClient;
		sockaddr addr = { 0 };
		int len = sizeof(addr);
		newClient = accept(socketServer, &addr, &len);
		if (newClient == INVALID_SOCKET)
			break;
		std::string connectMode = Receive(newClient);
		OnUserConnected(newClient, connectMode);
	}

	std::string datas = Receive(socketOpponent);
	if (datas.length())
		buffer.push_back(datas);
}


std::string Server::Receive(SOCKET socket)
{
	char buffer[BUFFER_MAX+1] = {0};
	std::string datas;
	int iLastRecievedBufferLen = 0;
	do {
		iLastRecievedBufferLen = recv(socket, buffer, BUFFER_MAX, 0);
		datas += buffer;
	} while (iLastRecievedBufferLen == BUFFER_MAX);
	return datas;
}


void Server::OnUserConnected(SOCKET socket, const std::string& connectMode)
{
	unsigned long iMode = 1;
	ioctlsocket(socket, FIONBIO, &iMode);
	if (connectMode == "0") {
		if (socketOpponent == INVALID_SOCKET) {
			socketOpponent = socket;
			printf("Opponent connected\n");
		}
		else {
			DisconnetClient(socket, "players full");
			printf("New opponent connection rejected\n");
		}
	}
	else if (connectMode == "1") {
		if (socketSpectators.size() < SPECTATOR_MAX)  {
			socketSpectators.push_back(socket);
			printf("Spectator connected\n");
		}
		else {
			DisconnetClient(socket, "spectators full");
			printf("Spectator connection rejected\n");
		}
	}
	else
		DisconnetClient(socket, "invalid connection mode");
}


void Server::DisconnetClient(SOCKET socket, const std::string message)
{
	// send message
	closesocket(socket);
}


void Server::Send(const std::string& _buffer)
{
	send(socketOpponent, &(_buffer[0]), _buffer.length() + 1, 0);
}


void Server::SendMove(bool chessmanTeam, int chessmanIndex, int x, int y)
{
	std::string str = "2";
	if (chessmanTeam)
		str += "0";
	else
		str += "1";
	
	str += chessmanIndex;
	str += x;
	str += y;

	Send(str);
}