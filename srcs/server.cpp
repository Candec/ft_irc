/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:01 by tpereira          #+#    #+#             */
/*   Updated: 2023/09/16 18:22:49 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

Server::Server() : _upTime(std::time(0)), run(OFF)
{
	_history.set(0, "Welcome to the FT_IRC server");
}

Server::Server(char *_port, char * _password) : _upTime(std::time(0))
{
	Server::setPort(_port);
	Server::setPassword(_password);
	_history.set(0, "Welcome to the FT_IRC server");
}

Server::~Server()
{
	std::vector<User *> users = getUsers();
	for (std::vector<User *>::iterator i = users.begin(); i < users.end(); i++)
		delUser(*(*i));
}

void Server::setup()
{
	if (_port.empty())
		error("port", EXIT);

	//AF_INT: ip_v4 | SOCK_STREAM: TCP
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd == 0)
		error("socket", EXIT);

	//Blocks the use of the Address and the Port at close time to avoid package mix
	int optname = 1;
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optname, sizeof(optname)))
		error("setsockopt", EXIT);

	//Shouldn't be required in linux. It is to block simultanious accesses to the fd
	if (fcntl(_fd, F_SETFL, O_NONBLOCK))
		error("fcntl", EXIT);

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(atoi(_port.c_str()));

	if (bind(_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		error("bind", EXIT);

	if (listen(_fd, address.sin_port) < 0)
		error("listen", EXIT);

	_pollfds.push_back(pollfd());
	_pollfds.back().fd = _fd;
	_pollfds.back().events = POLLIN;

	run = ON;
}

void Server::start()
{
	
}

void Server::setPort(std::string port)
{
	_port = port;
}

void Server::setPassword(char *password)
{
	_password.assign(_password, sizeof(password));
}

std::vector<User *> Server::getUsers()
{
    std::vector<User *> users;
    for (std::map<int, User>::iterator i = _users.begin(); i != _users.end(); i++)
        users.push_back(&i->second);
    return users;
}


void Server::delUser(User &user)
{
    std::vector<User *> users = getUsers();
    for (std::vector<User *>::iterator i = users.begin(); i != users.end(); )
    {
        if (*(*i) == user)
            i = users.erase(i); // Erase the element and update the iterator
        else
            ++i; // Move to the next element
    }
}
