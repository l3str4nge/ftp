#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <functional>
#include <stdlib.h>
#include <memory>
#include <map>
#include "server.h"

class FTPServer;

class Command {

public:
	typedef std::string (Command::*funcCommand)(char* buffer, FTPServer *server);

	Command();
	funcCommand execute(std::string type);

private:
	std::map<std::string, funcCommand> commandMap;
	std::string user(char* buffer, FTPServer *server);
	std::string notImplemented(char* buffer, FTPServer *server);
	std::string password(char* buffer, FTPServer *server);
	std::string port(char* buffer, FTPServer *server);
	std::string stor(char* buffer, FTPServer *server);
	std::string system(char* buffer, FTPServer *server);
	std::string type(char* buffer, FTPServer *server);
	std::string retr(char* buffer, FTPServer *server);

};
#endif