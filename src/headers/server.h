#ifndef FTPSERVER_H
#define FTPSERVER_H

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <memory>
#include <netdb.h>
#include <iostream>
#include <fstream>
#include "command.h"
#include "auth.h"
#include "fileOperate.h"

#define PORT_CONNECTION		3478
#define PORT_DATA			3480
#define BUFF_SIZE			5000
#define MAX_CONNECTIONS		10

class FTPServer {

public:
	FTPServer();
	void startListen();
	bool createNewConnection(std::string ip, int port);
	bool receiveData(std::shared_ptr<FileOperate> fileOperate);
	bool sendData(std::shared_ptr<FileOperate> fileOperate);

public:
	std::shared_ptr<Auth> auth;

private:
	struct sockaddr_in addrHost;
	struct sockaddr_in addrGuest;
	
	int socketHost;
	int socketGuest;
	int socketDataConnection;
	int optionValue = 1;
	char buffer[BUFF_SIZE];

private:
	void initHost();
	std::string parseCommand(char* buff);
};
#endif