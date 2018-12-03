#include "command.h"

Command::Command(){
	commandMap["USER"] = &Command::user;
	commandMap["PASS"] = &Command::password;
	commandMap["PORT"] = &Command::port;
	commandMap["SYST\r\n"] = &Command::system;
	commandMap["TYPE"] = &Command::type;
	commandMap["STOR"] = &Command::stor;
	commandMap["RETR"] = &Command::retr;
}

Command::funcCommand Command::execute(std::string type){
	// TODO: ZAWSZE SPRAWDZAC AUTHENTYKACJE PRZED SZUKANIEM KOMENDY !!!

	std::cout << "RETRIEVED COMMAND: " << type << std::endl;

	funcCommand command = commandMap[type];
	if (command != NULL) {
		return command;
	}

	return &Command::notImplemented;
}

std::string Command::notImplemented(char* buffer, FTPServer *server) {
	return "502 Command not implemented yet...\n";
}

std::string Command::user(char* buffer, FTPServer *server){
	std::string strbuff(buffer);
	server->auth->setUsername(strbuff.substr(5));

	if(!server->auth->isUserValid()){
		return "500 User not valid\n";
	}

	return "331 User OK\n"; 
}

std::string Command::password(char* buffer, FTPServer *server) {
	std::string strbuff(buffer);
	server->auth->setPassword(strbuff.substr(5));

	if(!server->auth->isAuth()){
		return "530 Authentication not valid\n";
	}

	return "230 Password OK\n";
}

std::string Command::port(char* buffer, FTPServer *server) {
	std::string buff(buffer);
	int last = buff.rfind(","); // ostatnie wystapienie ","
	int previousLast = buff.rfind(",", last - 1);
	std::string port_H = buff.substr(last+1);
	std::string port_L = buff.substr(previousLast + 1, last - previousLast);

	int h = std::stoi(port_L);
	int l = std::stoi(port_H);

	int connectionPort = (h << 8) | l; // zeby stwrzyc 16bitowa zmienna z dwoch 8bitowych

	std::string ip = buff.substr(5, previousLast - 5);

	for(int i=0; i<ip.size(); i++){
		if(ip.at(i) == ','){
			ip.replace(i, 1, ".");
		}
	}

	bool created = server -> createNewConnection(ip, connectionPort);
	if(!created){
		return "500\n";
	}

	return "200 Port command succescfull\n";
}

std::string Command::stor(char* buffer, FTPServer *server) {
	std::cout << "STOR " <<  buffer << std::endl;
	std::string fileName = std::string(buffer).substr(5);
	fileName = fileName.substr(0, fileName.find("$", 0));
	std::shared_ptr<FileOperate> fileOperate = std::make_shared<FileOperate>(fileName);
	bool stored = server -> receiveData(fileOperate);

	if(!stored)
		return "500\n";

	return "226 Done...\n";
}

std::string Command::system(char* buffer, FTPServer *server){
	return "215 UNIX Type: L8\n";
}

std::string Command::type(char* buffer, FTPServer *server){
	// w tej implementacji narazie bedzie tylko i wylacznie image type
	return "200 Set to IMAGE TYPE\n";
}

std::string Command::retr(char* buffer, FTPServer *server){
	std::string fileName = std::string(buffer).substr(5);
	fileName = fileName.substr(0, fileName.find("$", 0));
	std::shared_ptr<FileOperate> fileOperate = std::make_shared<FileOperate>(fileName);
	bool sent = server -> sendData(fileOperate);

	if (!sent){
		return "500\n";
	}

	return "226 Sending file done..\n";
}