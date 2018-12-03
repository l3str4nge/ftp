#ifndef AUTH_H
#define AUTH_H
#define _XOPEN_SOURCE // Important.
#include <string>
#include <pwd.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <shadow.h>
#include <unistd.h>


class Auth {
public:
	Auth();
	bool isAuth();
	void setUsername(std::string username);
	void setPassword(std::string password);
	void reset();
	bool isUserValid();

private:
	const std::string SALT = "$6$q3SDdF5X";

	std::string username;
	std::string password;
	struct spwd* userInfo;
};

#endif