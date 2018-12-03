#include "server.h"

FTPServer::FTPServer(){
	auth = std::make_shared<Auth>();

	std::cout << "Start socket initialization...\n";
	initHost();
	std::cout << "Listening on addres: "<< inet_ntoa(addrHost.sin_addr) << " port: " << PORT_CONNECTION << "..." << std::endl;
}

void FTPServer::initHost(){
	socketHost = socket(AF_INET, SOCK_STREAM, 0);

	// Tworzenie socketa
	if (socketHost == -1){
		std::cout << "Socket creating error. Exiting...\n";
		exit(1);
	}

	// Nadawanie opcji socketowi
	if (setsockopt(socketHost, SOL_SOCKET, SO_REUSEADDR, &optionValue, sizeof(int)) == -1){
		std::cout << "Socket option error. Exiting...\n";
		exit(1);
	}

	// Tworzenie sktruktury adresu hosta
		 memset(&addrHost, 0, sizeof(addrHost));
	addrHost.sin_family = AF_INET;
	addrHost.sin_port = htons(PORT_CONNECTION);
	addrHost.sin_addr.s_addr = INADDR_ANY;

	// Bindowanie socketa do stworzonego wyzej adresu
	if (bind(socketHost, (struct sockaddr*) &addrHost, sizeof(struct sockaddr)) == -1){
		std::cout << "Socket binding error. Exiting...\n";
		exit(1);
	}

	// Nasłuchiwanie na porcie 
	if (listen(socketHost, MAX_CONNECTIONS) == -1) {
		std::cout << "Listening error. Exiting...\n";
		exit(1);
	}
}

void FTPServer::startListen(){
	std::cout << "Waiting for a new connection...\n\n";

	int sinSize;
	// close(socketHost);
	while(1){
		sinSize = sizeof( struct sockaddr_in );
		if(( socketGuest = accept(socketHost,( struct sockaddr * ) &addrGuest, (socklen_t*) &sinSize ) ) == - 1 ){
			std::cout << "Failed with accepting a new connection\n";
			continue;
		}
		std::cout << "Connection accepted from: " << inet_ntoa(addrGuest.sin_addr) << std::endl;
		send(socketGuest, "220 Welcome\n", 12, 0);
		
		while(1){
			int receivedBytes = recv(socketGuest, &buffer, 5000, 0);
			if (receivedBytes == -1) {
				continue;
			}
			std::string command = parseCommand(buffer);
	
			Command cmd;
			Command::funcCommand xd = cmd.execute(command);
			std::string code = (cmd.*xd)(buffer, this); // LOL XD -> wywolanie funkcji klasy przez wskaznik 

			std::cout << code << std::endl;
			send(socketGuest, code.c_str(), code.size(), 0);	
			memset(buffer, 0, sizeof(buffer));		
		}
	}
}

std::string FTPServer::parseCommand(char* buff){
	std::string raw = std::string(buff);
	std::cout << "RAW COMMAND: " << raw << std::endl;
	std::string command = raw.substr(0, raw.find(" "));
	return command;
}

bool FTPServer::createNewConnection(std::string ip, int port){

	socketDataConnection = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
	// Tworzenie socketa
	if (socketDataConnection == -1){
		std::cout << "Socket creating error. Exiting...\n";
		return false;
	}

	// Tworzenie sktruktury adresu hosta
	struct sockaddr_in addrDataConn;
	memset(&addrDataConn, 0, sizeof(addrDataConn));
	addrDataConn.sin_family = AF_INET;
	addrDataConn.sin_port = htons(port);
	addrDataConn.sin_addr.s_addr = INADDR_ANY;

	if (connect(socketDataConnection, (struct sockaddr*) &addrDataConn, sizeof(struct sockaddr)) == -1) {
		return false;
	}

	return true;
}

bool FTPServer::receiveData(std::shared_ptr<FileOperate> fileOperate){
	std::string msg = "150 Server ready for receving data\n";
	send(socketGuest, msg.c_str(), msg.size(), 0);

	if(!fileOperate -> isValid())
		return false;


	char databuffer[BUFF_SIZE];
	std::ofstream fs = fileOperate -> createFile();
	int n=0;
	do{
		n = recv(socketDataConnection, &databuffer, 5000, 0);
		std::cout << "received bytes: " << n << std::endl;
		fs << databuffer;
	} while (n == 5000);
	
	fs.close();
	fileOperate -> setPermissions();
	close(socketDataConnection);

	return true;
}

bool FTPServer::sendData(std::shared_ptr<FileOperate> fileOperate){
	std::string msg = "150 Server ready for sending data\n";
	send(socketGuest, msg.c_str(), msg.size(), 0);

	char databuffer[BUFF_SIZE];
	int n;

	n = fileOperate -> getFile(databuffer);
	send(socketDataConnection, databuffer, n, 0);
	close(socketDataConnection);
	return true;
}