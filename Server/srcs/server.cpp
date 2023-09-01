#include "server.hpp"

Server::server() : upTime(std::time(0))
{
	display.set(0, "Welcome to the FT_IRC server");
}

Server::~server()
{
	std::vector<User *> users = getUsers;
	for (std::vector<User *>::iterator i = users.begin(); i = users.end; i++)
		delUser(*(*i));
}