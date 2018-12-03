#ifndef FILEOPERATE_H
#define FILEOPERATE_H

#include <string>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

class FileOperate {
public:
	FileOperate(std::string fileName);
	std::ofstream createFile();
	bool isValid();
	void setPermissions();
	int getFile(char* buffer);
	std::string getFileName();

private:
	std::string fileName;
};

#endif