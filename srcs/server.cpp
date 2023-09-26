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
Server::Server() : upTime(std::time(0))
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

Server::Server(std::string _port, std::string _password) : upTime(std::time(0))
{
	Server::setPort(_port);
	Server::setPassword(_password);
	history.set(0, "Welcome to the FT_IRC server");

	std::cout << BLUE << "Listening on port " << YELLOW << this->port << WHITE << std::endl;

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
	std::cout << std::endl << BLUE << "Shutting down server" << WHITE << std::endl;

	std::vector<User *> users = getUsers();
	for (std::vector<User *>::iterator i = users.begin(); i != users.end(); ++i)
		delUser(*(*i));
	close(listen_fd);

	exit(EXIT_SUCCESS);
}

/*
	SETTERS
*/
void Server::setPort(std::string _port) { port = atoi(_port.c_str()); }
void Server::setPassword(std::string _password) { password = _password; }

/*
	GETTERS
*/
std::vector<User *> Server::getUsers()
{
	std::vector<User *> usersV = std::vector<User *>();
	for (std::map<int, User *>::const_iterator i = users.begin(); i != users.end(); ++i)
		usersV.push_back(i->second);

	return (usersV);
}

/*
	ADDERS
*/
void Server::addUser()
{
	struct sockaddr_in addr;
	socklen_t socklen = sizeof(addr);
	int user_fd = accept(listen_fd, (struct sockaddr *)&addr, &socklen);
	if (user_fd == -1) {
		error("Failed accept", CONTINUE);
		return ;
	}

	if (users.size() == maxUsers) {
		sendMsg(user_fd, "Error: Server Full");
		close(user_fd);
		return ;
	}

	User *user = new User(user_fd, addr);
	users[user_fd] = user;
	users[user_fd]->setStatus(VERIFY);

	std::cout << WHITE << "User " << GREEN << "connected" << WHITE << " from ";
	std::cout << user->getHostaddr() << ":" << user->getPort() << std::endl;

	// strncpy(buffer, "Server connected\n", 18);
	// send(c_fd, buffer, BUFFER, 0);

	// TODO remove from here (function futurely used to process all users' messages)
	while (true) {
		char	buffer[BUFFER + 1] = { 0 };
		ssize_t	rd;

		if ((rd = recv(user_fd, buffer, BUFFER, 0)) == -1) {
			// users.erase(user_fd);
			// delete user;
			error("Failed recv", CONTINUE);
			// continue;
			return;
		} else if (!rd)
			continue;

		struct s_msg msg = this->parseMessage(user, buffer);

		// Print timestamp
		time_t		now = time(NULL);
		struct tm	t = *localtime(&now);
		std::cout << "[" << t.tm_hour << ":" << t.tm_min << ":" << t.tm_sec << "] ";

		std::cout << MAGENTA << user->getNick() << WHITE << ":" << std::endl;
		std::cout << buffer;
		std::cout << "*end of message*" << std::endl;

		if (!msg.command.compare("QUIT") || !rd) {
			std::cout << "User " << MAGENTA << user->getNick();
			std::cout << RED << "disconnected" << WHITE << std::endl;
			delUser(*user);
			return;
		}
	}

	pollfds.push_back(pollfd());
	pollfds.back().fd = user_fd;
	pollfds.back().events = POLLIN;

	if (users.size() == maxUsers && (shutdown(listen_fd, SHUT_RD) == -1))
		error("Failed shutdown of receptions on listening socket", CONTINUE);
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
	{
		Channel *channel = i->second;
		if (channel->isUser(user))
		{
			channel->removeUser(user);

			std::vector<User *> users = channel->getUsers();
			if (!users.size())
				remove.push_back(*channel);
			else
				for (std::vector<User *>::iterator i = users.begin(); i != users.end(); ++i)
					if (std::find(dUser.begin(), dUser.end(), *i) == dUser.end())
						dUser.push_back(*i);
		}
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
		User *user = (*i).second;
		if (now - user->getPreviousPing() >= timeout)
		{
			user->setStatus(OFFLINE);
			user->write(user->getNick() + "timed out");
			std::cout << user->getNick() << "timed out" << std::endl << std::flush;
		}
		else if (user->getStatus() == ONLINE)
			user->write("PING " + user->getNick());
	}
}

void Server::printUsers()
{
	char buffer[42];
	sprintf(buffer, "%-4s %-9s %s", "FD", "Nickname", "Host");
	history.set(listen_fd, std::string("\n") + buffer);
	for (std::map<int, User *>::iterator it = users.begin(); it != users.end(); ++it)
	{
		User *user = (*it).second;
		sprintf(buffer, "\033[34m%-4i \033[33m%-9s \033[35m", user->getFd(), user->getNick().c_str());
		history.set(user->getFd(), buffer + user->getHost());
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
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd == 0)
		error("socket", EXIT);

	//Blocks the use of the Address and the Port at close time to avoid package mix
	int optname = 1;
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optname, sizeof(optname)))
		error("setsockopt", EXIT);

	//Shouldn't be required in linux. It is to block simultanious accesses to the fd
	if (fcntl(listen_fd, F_SETFL, O_NONBLOCK))
		error("fcntl", EXIT);

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	if (bind(listen_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		error("bind", EXIT);

	if (listen(listen_fd, address.sin_port) < 0)
		error("listen", EXIT);

	updatePing();
	// struct in_addr ipAddr = address.sin_addr;
	// char str[INET_ADDRSTRLEN];
	// std::cout << "IP: " << inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN)  << std::flush;

	pollfds.push_back(pollfd());
	pollfds.back().fd = listen_fd;
	pollfds.back().events = POLLIN;
}

void Server::run()
{
	if (poll(&pollfds[0], pollfds.size(), (ping * 1000) / 100) == -1) {
		error("Failed poll", CONTINUE);
		return;
	}

	if (std::time(0) - previousPing >= ping)
	{
		std::cout << "updating ping" << std::endl << std::flush;
		updatePing();
	}
	else if (pollfds[0].revents == POLLIN)
	{
		std::cout << YELLOW << "Adding user..." << WHITE << std::endl << std::flush;
		addUser();
	}
	else
		updatePoll();
}

void Server::sendMsg(int client_fd, const std::string &msg)
{
	if (send(client_fd, (msg + MESSAGE_END).c_str(), msg.size() + 2, 0) == -1)
		error("Error sending message", CONTINUE);
}
