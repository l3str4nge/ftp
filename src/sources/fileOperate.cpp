#include "fileOperate.h"

FileOperate::FileOperate(std::string fileName){
	this->fileName = fileName;
	this->fileName.erase(this->fileName.rfind("\r\n"), 2);
}

std::ofstream FileOperate::createFile(){
	return std::ofstream(fileName);
}

bool FileOperate::isValid(){
	return true;
}

void FileOperate::setPermissions(){
	chmod(fileName.c_str(), S_IRWXU | S_IRWXG | S_IROTH);
}

int FileOperate::getFile(char *buffer){
	FILE* fp;
	fp = fopen(fileName.c_str(), "rb");

	if (fp == NULL){
		return false;
	}

	int n;

	do {
		n = fread(buffer, sizeof(char), 5000, fp);
	}while (n == 5000);
	
	fclose(fp);

	return n;
}

std::string FileOperate::getFileName(){
	return fileName;
}

