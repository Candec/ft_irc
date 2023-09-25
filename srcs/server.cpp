/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:01 by tpereira          #+#    #+#             */
/*   Updated: 2023/09/24 12:01:30 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

/*
	CONSTRUCTORS
*/
Server::Server() : upTime(std::time(0)), run(OFF)
{
	history.set(0, "Welcome to the FT_IRC server");

	std::ifstream file("./Configuration/irc.config", std::ios::in);
	std::string line;

	while (std::getline(file, line))
	{
		if (line.substr(0, line.find(":")) == "default_port")
			port = atoi(line.substr(1, line.find(":")).c_str());

		if (line.substr(0, line.find(":")) == "default_pass")
			password = line.substr(1, line.find(":"));

		if (line.substr(0, line.find(":")) == "server_name")
			serverName = atoi(line.substr(1, line.find(":")).c_str());

		if (line.substr(0, line.find(":")) == "max_users")
			maxUsers = atoi(line.substr(1, line.find(":")).c_str());

		if (line.substr(0, line.find(":")) == "max_ping")
			ping = atoi(line.substr(1, line.find(":")).c_str());

		if (line.substr(0, line.find(":")) == "timeout")
			timeout = atoi(line.substr(1, line.find(":")).c_str());
	}
}

Server::Server(std::string _port, std::string _password) : upTime(std::time(0)), run(OFF)
{
	Server::setPort(_port);
	Server::setPassword(_password);
	history.set(0, "Welcome to the FT_IRC server");

	std::ifstream file("./Configuration/irc.config");
	std::string line;

	while (std::getline(file, line))
	{
		// std::cout << "line: " << line << std::endl << std::flush;
		if (line.substr(0, line.find(":")) == "server_name")
			serverName = line.erase(0, line.find(":") + 1);

		if (line.substr(0, line.find(":")) == "max_users")
			maxUsers = atoi(line.erase(0, line.find(":") + 1).c_str());

		if (line.substr(0, line.find(":")) == "max_ping")
			ping = atoi(line.erase(0, line.find(":") + 1).c_str());

		if (line.substr(0, line.find(":")) == "timeout")
			timeout = atoi(line.erase(0, line.find(":") + 1).c_str());
	}
		// std::cout << "serverName: "<< serverName << std::endl << std::flush;
		// std::cout << "max_users: "<< maxUsers << std::endl << std::flush;
		// std::cout << "max_ping: "<< ping << std::endl << std::flush;
		// std::cout << "timeout: "<< timeout << std::endl << std::flush;
		// std::cout << "port: "<< port << std::endl << std::flush;
		// std::cout << "pass: "<< password << std::endl << std::flush;
}

Server::~Server()
{
	std::vector<User *> users = getUsers();
	for (std::vector<User *>::iterator i = users.begin(); i != users.end(); ++i)
		delUser(*(*i));
}

/*
	SETTERS
*/
void Server::setPort(std::string _port)
{
	port = atoi(_port.c_str());
}

void Server::setPassword(std::string _password)
{
	password = _password;
}

/*
	GETTERS
*/
std::vector<User *> Server::getUsers()
{
	std::vector<User *> usersV = std::vector<User *>();
	for (std::map<int, User *>::iterator i = users.begin(); i != users.end(); ++i)
		usersV.push_back(i->second);

	return (usersV);
}

/*
	ADDERS
*/
void Server::addUser()
{
	if (users.size() == maxUsers)
		if (shutdown(fd, SHUT_RD) == -1)
			return ;

	struct sockaddr_in addr;
	socklen_t socklen = sizeof(addr);
	int _fd = accept(fd, (struct sockaddr *)&addr, &socklen);

	if (_fd == -1)
		return ;

	users[fd] = new User(fd, addr);
	users[fd]->setStatus(VERIFY);

	pollfds.push_back(pollfd());
	pollfds.back().fd = fd;
	pollfds.back().events = POLLIN;
}


/*
	DELETERS
*/
void Server::delUser(User &user)
{
	std::vector<User *> dUser = std::vector<User *>();
	dUser.push_back(&user);

	std::vector<Channel> remove;
	for (std::map<int, Channel *>::iterator i = channels.begin(); i != channels.end(); ++i)
		if ((*i).second->isUser(user))
		{
			(*i).second->removeUser(user);
			
			std::vector<User *> users = i->second->getUsers();
			if (!users.size())
				remove.push_back((*i->second));
			else
				for (std::vector<User *>::iterator i = users.begin(); i != users.end(); ++i)
					if (std::find(dUser.begin(), dUser.end(), *i) == dUser.end())
						dUser.push_back(*i);
		}

	for (std::vector<Channel>::iterator j = remove.begin(); j != remove.end(); ++j)
		delChannel(*j);

	std::string message = "QUIT :" + user.getFd();
	for (std::vector<User *>::iterator k = dUser.begin(); k != dUser.end(); ++k)
		user.sendPrivateMessage(*(*k), message);
	user.push();

	history.remove(user.getFd());

	for (std::vector<pollfd>::iterator l = pollfds.begin(); l != pollfds.end(); ++l)
		if ((*l).fd == user.getFd())
		{
			pollfds.erase(l);
			break;
		}
	users.erase(user.getFd());
	delete &user;
}

void Server::delChannel(Channel &channel)
{
	(void)channel;
}

void Server::updatePing()
{
	previousPing = std::time(0);

	time_t now = std::time(0);

	for (std::map<int, User *>::iterator i = users.begin(); i != users.end(); i++)
	{
		if (now - (*i).second->getPreviousPing() >= timeout)
		{
			(*i).second->setStatus(OFFLINE);
			(*i).second->write((*i).second->getNick() + "timed out");
		}
		else if ((*i).second->getStatus() == ONLINE)
			(*i).second->write("PING " + (*i).second->getNick());
	}
}

void Server::updatePoll()
{
	for (std::vector<pollfd>::iterator i = pollfds.begin(); i != pollfds.end(); i++)
	{
		if ((*i).revents == POLLIN)
		{
			char buf[BUFFER + 1];
			ssize_t size = recv(users[(*i).fd]->getFd(), &buf, BUFFER, 0);

			std::cout << "in pckg: " << buf << std::endl << std::flush;
			if (size == -1)
				continue;

			if (size == 0)
			{
				users[(*i).fd]->setStatus(OFFLINE);
				continue;
			}

			buf[size] = 0;
			users[(*i).fd]->buffer += buf;

			std::string delimiter(MESSAGE_END);
			size_t position;
			while ((position = users[(*i).fd]->buffer.find(delimiter)) != std::string::npos)
			{
				std::string message = users[(*i).fd]->buffer.substr(0, position);
				users[(*i).fd]->buffer.erase(0, position + delimiter.length());
				if (!message.length())
					continue;
				users[(*i).fd]->push();
			}
			// messages_operations();
		}
	}
}

void Server::setup()
{
	if (port <= 0)
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
	address.sin_port = htons(port);

	if (bind(fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		error("bind", EXIT);

	if (listen(fd, address.sin_port) < 0)
		error("listen", EXIT);

	updatePing();
	// struct in_addr ipAddr = address.sin_addr;
	// char str[INET_ADDRSTRLEN];
	// std::cout << "IP: " << inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN)  << std::flush;

	pollfds.push_back(pollfd());
	pollfds.back().fd = fd;
	pollfds.back().events = POLLIN;

	run = ON;
}

void Server::start()
{
	if (poll(&pollfds[0], pollfds.size(), (ping * 1000) / 100) == -1)
		return ;

	if (std::time(0) - previousPing >= ping)
	{
		std::cout << "updating ping" << std::endl << std::flush;
		updatePing();
	}
	else if (pollfds[0].revents == POLLIN)
	{
		std::cout << "adding user" << std::endl << std::flush;
		addUser();
	}
	else
		updatePoll();
}
