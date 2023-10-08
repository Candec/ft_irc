/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:01 by tpereira          #+#    #+#             */
/*   Updated: 2023/10/08 16:38:40 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

/*
	CONSTRUCTORS
*/
Server::Server() : upTime(time(0))
{
	// history.clear();
	// history.set("Welcome to the FT_IRC server");

	ifstream file("./Configuration/irc.config", ios::in);
	string line;

	while (getline(file, line))
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

		if (line.substr(0, line.find(":")) == "default_channels")
		{
			std::string channel;
			size_t pos = 0;
			while ((pos = line.find(',')) != std::string::npos)
			{
				channel = line.substr(0, pos);
				setChannel(channel);
				line.erase(0, pos + 1);
			}
		}
	}
}

Server::Server(string _port, string _password) : upTime(time(0))
{
	Server::setPort(_port);
	Server::setPassword(_password);

	// history.clear();
	// history.set("Welcome to the FT_IRC server");

	cout << BLUE << "Listening on port " << YELLOW << this->port << WHITE << endl;

	ifstream file("./Configuration/irc.config");
	string line;

	while (getline(file, line))
	{
		// cout << "line: " << line << endl << flush;
		if (line.substr(0, line.find(":")) == "server_name")
			serverName = line.erase(0, line.find(":") + 1);

		if (line.substr(0, line.find(":")) == "max_users")
			maxUsers = atoi(line.erase(0, line.find(":") + 1).c_str());

		if (line.substr(0, line.find(":")) == "max_ping")
			ping = atoi(line.erase(0, line.find(":") + 1).c_str());

		if (line.substr(0, line.find(":")) == "timeout")
			timeout = atoi(line.erase(0, line.find(":") + 1).c_str());

		if (line.substr(0, line.find(":")) == "default_channels")
		{
			std::string channel;
			size_t pos = 0;
			line.erase(0, line.find(":") + 1);
			while ((pos = line.find(',')) != std::string::npos)
			{
				channel = line.substr(0, pos);
				setChannel(channel);
				line.erase(0, pos + 1);
			}
			channel = line.substr(0, pos);
			setChannel(channel);
		}
	}
}

Server::~Server()
{
	cout << endl << BLUE << "Shutting down server" << WHITE << endl;

	vector<User *> users = getUsers();
	for (vector<User *>::iterator i = users.begin(); i != users.end(); ++i)
		delUser(*(*i));
	close(listen_fd);

	exit(EXIT_SUCCESS);
}

/*
	SETTERS
*/
void Server::setPort(string _port) { port = atoi(_port.c_str()); }
void Server::setPassword(string _password) { password = _password; }

void Server::setChannel(string channelName)
{
	Server *ptrServer = this;
	Channel *channel = new Channel(channelName, ptrServer);
	channels.insert( std::pair<std::string, Channel *>(channelName, channel));
	cout << "channel [ mem: " << channels[channelName] << " | << name: " << channels[channelName]->getName() << " ] created" << endl << flush;
}

/*
	GETTERS
*/
vector<User *> Server::getUsers()
{
	vector<User *> usersV = vector<User *>();
	for (map<int, User *>::const_iterator i = users.begin(); i != users.end(); ++i)
		usersV.push_back(i->second);

	return (usersV);
}

Channel *Server::getChannel(const std::string &channelName)
{
	return (channels[channelName]);
}

vector<Channel *> Server::getChannels()
{
	vector<Channel *> channelsV = vector<Channel *>();
	for (map<std::string, Channel *>::const_iterator i = channels.begin(); i != channels.end(); ++i)
		channelsV.push_back(i->second);

	return (channelsV);
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

	User *user = new User(user_fd, addr, this);
	users[user_fd] = user;
	users[user_fd]->setStatus(VERIFY);

	cout << BLUE << "User " << GREEN << "connected" << BLUE << " from ";
	cout << user->getHostaddr() << ":" << user->getPort() << WHITE << endl;

	sendClear(user_fd);
	sendMsg(user_fd, WELCOME_MSG);
	sendColorMsg(user_fd, COMMAND_MSG, BOLD);
	sendColorMsg(user_fd, CMD_LIST_MSG, YELLOW);

	// strncpy(buffer, "Server connected\n", 18);
	// send(c_fd, buffer, BUFFER, 0);

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
	vector<User *> dUser = vector<User *>();
	dUser.push_back(&user);

	vector<Channel> remove;
	for (map<std::string, Channel *>::iterator i = channels.begin(); i != channels.end(); ++i)
	{
		Channel *channel = i->second;
		if (channel->isUser(user))
		{
			channel->removeUser(user);

			vector<User *> users = channel->getUsers();
			if (!users.size())
				remove.push_back(*channel);
			else
				for (vector<User *>::iterator i = users.begin(); i != users.end(); ++i)
					if (find(dUser.begin(), dUser.end(), *i) == dUser.end())
						dUser.push_back(*i);
		}
	}

	for (vector<Channel>::iterator j = remove.begin(); j != remove.end(); ++j)
		delChannel(*j);

	string message = "QUIT :" + user.getNick() + " has quit";
	for (vector<User *>::iterator k = dUser.begin(); k != dUser.end(); ++k)
		user.sendPrivateMessage(*(*k), message);
	user.push();

	// history.remove(user.getFd());

	for (vector<pollfd>::iterator l = pollfds.begin(); l != pollfds.end(); ++l)
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
	channels.erase(channel.getName());
}

bool Server::isChannel(string const &channel)
{
	if (channels.find(channel) == channels.end())
		return (false);
	return (true);
}

void Server::updatePing()
{
	previousPing = time(0);

	time_t now = time(0);

	for (map<int, User *>::iterator i = users.begin(); i != users.end(); i++)
	{
		User *user = (*i).second;
		if (now - user->getPreviousPing() >= timeout)
		{
			user->setStatus(OFFLINE);
			user->write(user->getNick() + "timed out");
			cout << user->getNick() << "timed out" << endl << flush;
		}
		else if (user->getStatus() == ONLINE)
			user->write("PING " + user->getNick());
	}
}

// void Server::printUsers()
// {
// 	char buffer[42];
// 	snprintf(buffer, sizeof(buffer), "%-4s %-9s %s", "FD", "Nickname", "Host");
// 	history.set(listen_fd, string("\n") + buffer);
// 	for (map<int, User *>::iterator it = users.begin(); it != users.end(); ++it)
// 	{
// 		User *user = (*it).second;
// 		snprintf(buffer, sizeof(buffer), "\033[34m%-4i \033[33m%-9s \033[35m", user->getFd(), user->getNick().c_str());
// 		history.set(user->getFd(), buffer + user->getHost());
// 	}
// }

// void Server::printChannels()
// {
// 	for (map<string, Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
// 	{
// 		Channel *channel = (*it).second;
// 		sendMsg()

// 	}

// }

void Server::updatePoll()
{
	for (vector<pollfd>::iterator i = pollfds.begin(); i != pollfds.end(); i++)
	{
		if (i->revents & POLLIN) {
			if (users.find(i->fd) != users.end()) {
				// if (receiveMsg2(i->fd) == -1)
				// 	break;
				if (receiveMsg(i) == QUIT) {
					User *user = users[i->fd];
					cout << BLUE << "User " << MAGENTA << user->getNick();
					cout << RED << " disconnected" << WHITE << endl;
					delUser(*user);
					break;
				}
			}
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
	address.sin_addr.s_addr = htons(INADDR_ANY);
	address.sin_port = htons(port);

	if (bind(listen_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		error("bind", EXIT);

	if (listen(listen_fd, address.sin_port) < 0)
		error("listen", EXIT);

	updatePing();
	// struct in_addr ipAddr = address.sin_addr;
	// char str[INET_ADDRSTRLEN];
	// cout << "IP: " << inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN)  << flush;

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

	if (time(0) - previousPing >= ping)
	{
		// cout << BLUE << "updating ping" << WHITE << endl << flush;
		updatePing();
	}
	else if (pollfds[0].revents == POLLIN)
	{
		cout << BLUE << "Adding user..." << WHITE << endl << flush;
		addUser();
	}
	else
		updatePoll();

	// std::vector<User *> users = getUsers();

	// for (std::vector<User *>::iterator i = users.begin(); i != users.end(); ++i)
	// 	if ((*i)->getStatus() == OFFLINE)
	// 		delUser(*(*i));
	// users = getUsers();
	// for (std::vector<User *>::iterator j = users.begin(); j != users.end(); ++j)
	// 	(*j)->push();

	// printUsers();
}

void Server::sendClear(int user_fd)
{
	if (send(user_fd, CLEAR_MSG, 5, 0) == -1)
		error("Error sending message", CONTINUE);
}

void Server::sendMsg(int user_fd, const string &msg)
{
	cout << "Reporting message to the client: " << msg << endl;
	if (send(user_fd, (msg + MESSAGE_END).c_str(), msg.size() + 2, 0) == -1)
		error("Error sending message", CONTINUE);
}

void Server::sendColorMsg(int user_fd, const string &msg, const string &color)
{
	string str;
	str = color + msg + RESET + MESSAGE_END;
	// int size = str.str().size();
	cout << "Reporting message to the client: " << str << endl;
	if (send(user_fd, str.c_str(), str.size(), 0) == -1)
		error("Error sending message", CONTINUE);
}

// int Server::receiveMsg2(int user_fd)
// {
// 	char	buffer[BUFFER + 1] = { 0 };
// 	ssize_t	rd;
// 	User	*user = users.at(user_fd);

// 	if ((rd = recv(user_fd, buffer, BUFFER, 0)) == -1) {
// 		error("Failed recv", CONTINUE);
// 		return 0;
// 	} else if (rd == 0) {
// 		return -1;
// 	}

// 	struct s_msg msg = this->parseMessage(user, buffer);

// 	// Print timestamp
// 	time_t		now = time(NULL);
// 	struct tm	t = *localtime(&now);
// 	cout << "[" << t.tm_hour << ":" << t.tm_min << ":" << t.tm_sec << "] ";

// 	cout << MAGENTA << user->getNick() << WHITE << " @ FD# " << user_fd << ":" << endl;
// 	cout << buffer;
// 	cout << "*end of message*" << endl << endl;

// 	if (!msg.command.compare("QUIT"))
// 		return -1;

// 	return 0;
// }



// Since application is single threaded, is a msg buffer for each client needed?
// Can't we just process each fd and flush the parsed content one by one?

int Server::receiveMsg(vector<pollfd>::iterator it)
{
	User	*user = users.at(it->fd);
	char	buf[BUFFER + 1] = { 0 };
	ssize_t	size = recv(user->getFd(), &buf, BUFFER, 0);

	if (size == -1) {
		error("Failed recv", CONTINUE);
		return 0;
	}
	else if (size == 0) {
		user->setStatus(OFFLINE); // Perhaps user can be removed instantly
		return -1;
	}

	struct s_msg msg = this->parseMessage(user, buf);
	// cout << "in pckg: " << buf << endl << flush;
	printMsg2(it->fd, buf);

	// buf[size] = 0;
	user->buffer += buf;

	string delimiter(MESSAGE_END);
	size_t position;
	while ((position = user->buffer.find(delimiter)) != string::npos)
	{
		string message = user->buffer.substr(0, position);
		user->buffer.erase(0, position + delimiter.length());
		if (!message.length())
			continue;
		user->push();
	}
	// messages_operations();

	// printMsg(it);
	Channel *channel = user->getChannel();
	channel->set(user->buffer);

	if (!msg.command.compare("QUIT"))
		return -1;

	return 0;
}



void Server::printMsg(vector<pollfd>::const_iterator it)
{
	// Print timestamp
	cout << timestamp();

	int		user_fd = it->fd;
	User	*user = users[user_fd];

	// Test to check if we could use just one of the formats (user_fd vs it->fd)
	// to relate to the user's fd
	if (user->getFd() != user_fd)
		error("FDs mismatch", EXIT);
	else
		cout << GREEN << "FDs match " << WHITE;

	cout << MAGENTA << user->getNick() << WHITE << " @ FD# " << user_fd << ":" << endl;
	cout << user->buffer;
	cout << "*end of message*" << endl << endl;
}

void Server::printMsg2(const int user_fd, const char *msg)
{
	// Print timestamp
	cout << timestamp();

	User	*user = users[user_fd];

	// Test to check if we could use just one of the formats (user_fd vs it->fd)
	// to relate to the user's fd
	if (user->getFd() != user_fd)
		error("FDs mismatch", EXIT);
	else
		cout << GREEN << "FDs match " << WHITE;

	cout << MAGENTA << user->getNick() << WHITE << " @ FD# " << user_fd << ":" << endl;
	cout << msg;
	cout << "*end of message*" << endl << endl;
}
