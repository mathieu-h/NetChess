#include "Client.h"


Client::Client(bool isSpectator)
{
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
		printf("WSAStartup failed: %d\n", result);

	socketClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketClient == INVALID_SOCKET)
		std::cout<<WSAGetLastError()<<std::endl;

	struct addrinfo *addrInfo = NULL, *ptr = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	
	result = getaddrinfo(DEFAULT_ADDRESS , DEFAULT_PORT, &hints, &addrInfo);
	if (result != 0)
		printf("getaddrinfo failed: %d\n", result);

	ptr = addrInfo;

	socketClient = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (socketClient == INVALID_SOCKET)
		printf("Error at socket(): %ld\n", WSAGetLastError());

	result = connect(socketClient, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (result == SOCKET_ERROR)
		printf("Unable to connect to server!\n");

	if (isSpectator) 
		Send("1");
	else
		Send("0");

	unsigned long iMode = 1;
	ioctlsocket(socketClient, FIONBIO, &iMode);
}


Client::~Client(void)
{
}


void Client::Update()
{
	std::string datas = Receive();
	if (datas.length())
		buffer.push_back(datas);
}


std::string Client::Receive()
{
	char buffer[BUFFER_MAX+1] = {0};
	std::string datas;
	int iLastRecievedBufferLen = 0;
	do {
		iLastRecievedBufferLen = recv(socketClient, buffer, BUFFER_MAX, 0);
		datas += buffer;
	} while (iLastRecievedBufferLen == BUFFER_MAX);
	return datas;
}

/*
void Client::Send(const std::string& buffer)
{
	if (buffer.length())
	{
		int iLenToSend = buffer.length() + 1;
		const char* pBuffer = buffer.c_str();
		int iSent = 0;
		do {
			int iLastSent = send(socketClient, pBuffer + iSent, iLenToSend - iSent, 0);
			bool m_bConnected = (iLastSent > 0);
			if (!m_bConnected)
				break;
			iSent += iLastSent;
		} while (iSent < iLenToSend);
	}
}*/

void Client::Send(const std::string& _buffer)
{
	send(socketClient, &(_buffer[0]), _buffer.length() + 1, 0);
}


void Client::SendMove(bool chessmanTeam, int chessmanIndex, int x, int y)
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