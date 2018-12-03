#include "auth.h"

Auth::Auth(){}

bool Auth::isAuth(){
	if(username.empty() && password.empty()){
		return false;
	}

	char *cryptedPassword = crypt(password.c_str(), SALT.c_str());

	if (strcmp(userInfo->sp_pwdp, cryptedPassword) != 0){
		return false;
	}

	return true;
}

void Auth::setUsername(std::string username){
	this->username = username;
	this->username.erase(this->username.rfind("\r\n"), 2);
}

void Auth::setPassword(std::string password){
	this->password = password;
	this->password.erase(this->password.rfind("\r\n"), 2);
}

void Auth::reset(){
	password = "";
	username = "";
}
bool Auth::isUserValid(){
	userInfo = getspnam(username.c_str());
	if (userInfo == NULL){
		return false;
	}

	return true;
}
