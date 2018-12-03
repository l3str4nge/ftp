#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include "server.h"


int main() {
	FTPServer server;
	server.startListen();
	return 0;
}