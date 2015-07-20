#include "ofMain.h"
#include "ofApp.h"

#define DEFAULT_ADDRESS "127.0.0.1"
#define DEFAULT_PORT 27015

//========================================================================
int main()
{
	int choice = 0;
	string ip = DEFAULT_ADDRESS;
	int port = DEFAULT_PORT;
	
	cout << "- 1 : Create game" << endl << "- 2 : Join game" << endl << "- 3 : Spectate game" << endl;
	while (choice != 1 && choice != 2 && choice != 3) {
		cin >> choice;
	}
	
	if (choice == 2 || choice == 3) {
		cout << "IP :" << endl;
		cin >> ip;
		if (ip == "")
			ip = DEFAULT_ADDRESS;
	}

	cout << "Port :" << endl;
	cin >> port;
	if (port < 0 || port > 65535)
		port = DEFAULT_PORT;
	
	ofSetupOpenGL(800,800,OF_WINDOW);
	ofApp* app = new ofApp();
	app->SetNetwork(choice, ip, port);
	ofRunApp(app);
}
