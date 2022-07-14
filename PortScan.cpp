#include <iostream>
#include <fstream>
#include <string>
#include <winsock2.h>
#include <windows.h>

using namespace std;

int checkPort(string address, int port);

int main(int argc , char *argv[])
{
    ifstream ports, addresses;
    ports.open("ports.txt",ios_base::in);
    addresses.open("addresses.txt",ios_base::in);
    if(!ports || !addresses) {
        cout << "Can't open file!";
        exit(0);
    }
    string currentAddress = "";
    int currentPort = 0;
    while(!addresses.eof()) {
        getline(addresses,currentAddress);
        cout << "Checking address: " << currentAddress << endl << endl;
        while(!ports.eof()) {
            ports >> currentPort;
            cout << "Checking port: " << currentPort << "... ";
            switch(checkPort(currentAddress,currentPort)) {
        case 0:
            cout << "Port is opened." << endl;
            break;
        case 1:
            cout << "Port is closed." << endl;
            break;
        case 2:
            cout << "Cant't start WinSock2 service!" << endl;
            exit(0);
            break;
        case 3:
            cout << "Can't create socket!" << endl;
            exit(0);
            break;
            }

        }
        cout << endl;
        ports.seekg(0,ios_base::beg);
    }
	return 0;
}

int checkPort(string address, int port) {
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	//cout << "Initialising WinSock2... ";
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Cant' start WinSock2. Error Code : %d\n",WSAGetLastError());
		return 2;
	}
	//cout << "Successfully" << endl;

    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d" , WSAGetLastError());
		return 3;
	}

	//cout << "Socket created" << endl;

	server.sin_addr.s_addr = inet_addr(address.c_str());
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		//cout << "Connection error";
		return 1;
	}
	//cout << "Connected";
    closesocket(s);
    WSACleanup();
    return 0;
}
