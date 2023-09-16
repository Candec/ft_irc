/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:01 by tpereira          #+#    #+#             */
/*   Updated: 2023/09/16 13:17:02 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server() : upTime(std::time(0)), run(OFF)
{
	history.set(0, "Welcome to the FT_IRC server");
}

Server::Server(char *_port, char * _password) : upTime(std::time(0))
{
	Server::setPort(_port);
	Server::setPassword(_password);
	history.set(0, "Welcome to the FT_IRC server");
}

Server::~Server()
{
	std::vector<User *> users = getUsers;
	for (std::vector<User *>::iterator i = users.begin(); i = users.end; i++)
		delUser(*(*i));
}

void Server::setup()
{
	if (port.empty())
		error("port", EXIT);

	//AF_INT: ip_v4 | SOCK_STREAM: TCP
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == 0)
		error("socket", EXIT);

	//Blocks the use of the Address and the Port at close time to avoid package mix
	int optname = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optname, sizeof(optname)))
		error("setsockopt", EXIT);

	//Shouldn't be required in linux. It is to block simultanious accesses to the fd
	if (fcntl(fd, F_SETFL, O_NONBLOCK))
		error("fcntl", EXIT);

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(atoi(port.c_str()));

	if (bind(fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		error("bind", EXIT);

	if (listen(fd, address.sin_port) < 0)
		error("listen", EXIT);

	pollfds.push_back(pollfd());
	pollfds.back().fd = fd;
	pollfds.back().events = POLLIN;

	run = ON;
}

void Server::start()
{
	
}

void Server::setPort(char *_port)
{
	portassign(_port, sizeof(_port));
}

void Server::setPassword(char *_password)
{
	password.assign(_password, sizeof(_password));
}