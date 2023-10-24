/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:01 by tpereira          #+#    #+#             */
/*   Updated: 2023/10/24 01:23:30 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.hpp"

/*
	CONSTRUCTORS
*/
Server::Server() : _upTime(time(0))
{
	// history.clear();
	// history.set("Welcome to the FT_IRC server");

	ifstream file("./Configuration/irc.config", ios::in);
	string line;

	while (getline(file, line))
	{
		if (line.substr(0, line.find(":")) == "default_port")
			_port = atoi(line.substr(1, line.find(":")).c_str());

		if (line.substr(0, line.find(":")) == "default_pass")
			_password = line.substr(1, line.find(":"));

		if (line.substr(0, line.find(":")) == "server_name")
			_serverName = atoi(line.substr(1, line.find(":")).c_str());

		if (line.substr(0, line.find(":")) == "max_users")
			_maxUsers = atoi(line.substr(1, line.find(":")).c_str());

		if (line.substr(0, line.find(":")) == "max_ping")
			_ping = atoi(line.substr(1, line.find(":")).c_str());

		if (line.substr(0, line.find(":")) == "timeout")
			_timeout = atoi(line.substr(1, line.find(":")).c_str());

		// if (line.substr(0, line.find(":")) == "default_channels")
		// {
		// 	string channel;
		// 	size_t pos = 0;
		// 	while ((pos = line.find(',')) != string::npos)
		// 	{
		// 		channel = line.substr(0, pos);
		// 		createChannel(channel);
		// 		line.erase(0, pos + 1);
		// 	}
		// }
	}
}

Server::Server(const char * const port, const string password) : _upTime(time(0))
{
	setPort(port);
	setPassword(password);
	setup();

	// history.clear();
	// history.set("Welcome to the FT_IRC server");

	parseConfig();
}

Server::~Server()
{
	cout << endl << BLUE << "Shutting down server" << WHITE << endl;

	for (map<int, User *>::iterator i = _users.begin(); i != _users.end(); ++i)
		delUser(i->second);
	close(_listen_fd);
	exit(EXIT_SUCCESS);
}



void Server::parseConfig()
{
	ifstream file(CONFIG_FILE);
	string line;

	while (getline(file, line))
	{
		// cout << "line: " << line << endl << flush;
		if (line.substr(0, line.find(":")) == "server_name")
			_serverName = line.erase(0, line.find(":") + 1);

		if (line.substr(0, line.find(":")) == "max_users")
			_maxUsers = atoi(line.erase(0, line.find(":") + 1).c_str());

		if (line.substr(0, line.find(":")) == "max_ping")
			_ping = atoi(line.erase(0, line.find(":") + 1).c_str());

		if (line.substr(0, line.find(":")) == "timeout")
			_timeout = atoi(line.erase(0, line.find(":") + 1).c_str());

		// if (line.substr(0, line.find(":")) == "default_channels")
		// {
		// 	string channel;
		// 	size_t pos = 0;
		// 	line.erase(0, line.find(":") + 1);
		// 	while ((pos = line.find(',')) != string::npos)
		// 	{
		// 		channel = line.substr(0, pos);
		// 		createChannel(channel);
		// 		line.erase(0, pos + 1);
		// 	}
		// 	channel = line.substr(0, pos);
		// 	createChannel(channel);
		// }
	}
}

void Server::setup()
{
	//AF_INT: ip_v4 | SOCK_STREAM: TCP
	_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_listen_fd == 0)
		error("socket", EXIT);

	//Blocks the use of the Address and the Port at close time to avoid package mix
	int optname = 1;
	if (setsockopt(_listen_fd, SOL_SOCKET, SO_REUSEADDR, &optname, sizeof(optname)))
		error("setsockopt", EXIT);

	//Shouldn't be required in linux. It is to block simultanious accesses to the fd
	if (fcntl(_listen_fd, F_SETFL, O_NONBLOCK))
		error("fcntl", EXIT);

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htons(INADDR_ANY);
	address.sin_port = htons(_port);

	if (bind(_listen_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		error("bind", EXIT);

	if (listen(_listen_fd, address.sin_port) < 0)
		error("listen", EXIT);

	updatePing();
	// struct in_addr ipAddr = address.sin_addr;
	// char str[INET_ADDRSTRLEN];
	// cout << "IP: " << inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN)  << flush;

	_pollfds.push_back(pollfd());
	_pollfds.back().fd = _listen_fd;
	_pollfds.back().events = POLLIN;
}

void Server::run()
{
	cout << BLUE << "Listening on port " << YELLOW << this->_port << WHITE << endl;

	while (true) {
		if (poll(&_pollfds[0], _pollfds.size(), (_ping * 1000) / 100) == SENDING_ERROR)
			error("Failed poll", EXIT);

		if (time(0) - _previousPing >= _ping)
		{
			// cout << BLUE << "updating ping" << WHITE << endl << flush;
			updatePing();
		}
		else if (_pollfds[0].revents == POLLIN)
		{
			cout << BLUE << "Adding user..." << WHITE << endl << flush;
			createUser();
		}
		else
			updatePoll();

		// vector<User *> users = getUsers();

		// for (vector<User *>::iterator i = users.begin(); i != users.end(); ++i)
		// 	if ((*i)->getStatus() == OFFLINE)
		// 		delUser(*(*i));
		// users = getUsers();
		// for (vector<User *>::iterator j = users.begin(); j != users.end(); ++j)
		// 	(*j)->push();

		// printUsers();
	}
}



/*
	SETTERS
*/
void Server::setPort(const char * const arg)
{
	ostringstream err;
	err << "Port must be a number in range [" << 1024 << ", " << UINT16_MAX << "]";

	// Check if arg is number
	for (size_t i = 0; i < strlen(arg); ++i)
		if (!isdigit(arg[i]))
			error(err.str(), EXIT);

	// Check if port is within acceptable range
	int tmp_port = atoi(arg);
	if (1024 > tmp_port || tmp_port > UINT16_MAX)
		error(err.str(), EXIT);

	_port = static_cast<uint16_t>(tmp_port);
}

void Server::setPassword(const string &password) { _password = password; }



/*
	GETTERS
*/
vector<User *> Server::getUsers() const
{
	vector<User *> usersV = vector<User *>();
	for (map<int, User *>::const_iterator i = _users.begin(); i != _users.end(); ++i)
		usersV.push_back(i->second);

	return (usersV);
}

Channel *Server::getChannel(const string &channelName)
{
    map<string, Channel *>::const_iterator it = _channels.find(channelName);
    if (it == _channels.end())
        return NULL;
    return _channels.at(channelName);
}

vector<Channel *> Server::getChannels() const
{
	vector<Channel *> channelsV = vector<Channel *>();
	for (map<string, Channel *>::const_iterator i = _channels.begin(); i != _channels.end(); ++i)
		channelsV.push_back(i->second);

	return (channelsV);
}



/*
	CREATORS
*/
void Server::createUser()
{
	struct sockaddr_in addr;
	socklen_t socklen = sizeof(addr);
	const int user_fd = accept(_listen_fd, (struct sockaddr *)&addr, &socklen);
	if (user_fd == SENDING_ERROR) {
		error("Failed accept", CONTINUE);
		return ;
	}

	if (_users.size() == _maxUsers) {
		sendMsg(user_fd, "Error: Server Full");
		close(user_fd);
		return ;
	}

	User *user = new User(user_fd, addr, this);
	user->setStatus(VERIFY);
	_users[user_fd] = user;

	cout << BLUE << "User " << GREEN << "connected" << BLUE << " from ";
	cout << user->getHostaddr() << ":" << user->getPort() << WHITE << endl;

	sendClear(user_fd);
	sendMsg(user_fd, WELCOME_MSG);
	sendColorMsg(user_fd, COMMAND_MSG, BOLD);
	sendColorMsg(user_fd, CMD_LIST_MSG, YELLOW);

	// strncpy(buffer, "Server connected\n", 18);
	// send(c_fd, buffer, BUFFER, 0);

	_pollfds.push_back(pollfd());
	_pollfds.back().fd = user_fd;
	_pollfds.back().events = POLLIN;

	if (_users.size() == _maxUsers && (shutdown(_listen_fd, SHUT_RD) == SENDING_ERROR))
		error("Failed shutdown of receptions on listening socket", CONTINUE);
}

void Server::createChannel(const string &channelName, User *user)
{
	if (!isValidChannelName(channelName))
		return;

	cout << GREEN << "creating channel " << YELLOW << channelName << RESET << endl << flush;
	Channel *channel = new Channel(channelName, this);
	_channels[channelName] = channel;

	cout << "setting operator" << endl;
	channel->setUserMode(user, "o");
}



/*
	DELETERS
*/

// void Server::delUser(User *user)
// {
// 	vector<User *> dUser = vector<User *>();
// 	dUser.push_back(user);

// 	vector<Channel *> remove;
// 	for (map<std::string, Channel *>::iterator i = _channels.begin(); i != _channels.end(); ++i)
// 	{
// 		Channel *channel = i->second;
// 		if (channel->isUser(user))
// 		{
// 			channel->removeUser(user);

// 			vector<User *> users = channel->getUsers();
// 			if (!users.size())
// 				remove.push_back(channel);
// 			else
// 				for (vector<User *>::iterator i = users.begin(); i != users.end(); ++i)
// 					if (find(dUser.begin(), dUser.end(), *i) == dUser.end())
// 						dUser.push_back(*i);
// 		}
// 	}

// 	for (vector<Channel *>::iterator j = remove.begin(); j != remove.end(); ++j)
// 		delChannel(*j);

// 	string message = "QUIT :" + user->getNick() + " has quit";
// 	for (vector<User *>::iterator k = dUser.begin(); k != dUser.end(); ++k)
// 		user->sendPrivateMessage(*k, message);
// 	user->push();

// 	// history.remove(user.getFd());

// 	for (vector<pollfd>::iterator l = _pollfds.begin(); l != _pollfds.end(); ++l)
// 		if ((*l).fd == user->getFd())
// 		{
// 			_pollfds.erase(l);
// 			break;
// 		}
// 	_users.erase(user->getFd());

// 	delete &user;
// }

void Server::delUser(User *user)
{
	vector<User *>	others = vector<User *>();

	for (map<string, Channel *>::iterator i = _channels.begin(); i != _channels.end(); ++i)
	{
		Channel	*channel = i->second;
		if (!channel)
			break;

		if (channel->isUser(user))
		{
			channel->removeUser(user);

			vector<User *>	chUsers = channel->getUsers();

			if (!chUsers.size())
				delChannel(channel);
			else
				for (vector<User *>::iterator j = chUsers.begin(); j != chUsers.end(); ++j) {
					if (find(others.begin(), others.end(), *j) == others.end())
						others.push_back(*j);
				}
		}
	}

	const string message = MAGENTA + user->getNick() + " has " + RED + "quit" + WHITE;


	for (vector<User *>::iterator k = others.begin(); k != others.end(); ++k)
		user->sendPrivateMessage(*k, message);
	user->push();
	// history.remove(user->getFd());

	for (vector<pollfd>::iterator l = _pollfds.begin(); l != _pollfds.end(); ++l)
		if (l->fd == user->getFd())
		{
			_pollfds.erase(l);
			break;
		}
	_users.erase(user->getFd());

	delete user;
}

void Server::delChannel(const Channel *channel)
{
	_channels.erase(channel->getName());
	delete channel;
}



bool Server::isChannel(const string &channel) const
{
	return (_channels.find(channel) != _channels.end());
}

void Server::updatePing()
{
	_previousPing = time(0);

	time_t	now = time(0);

	for (map<int, User *>::iterator i = _users.begin(); i != _users.end(); i++)
	{
		User *user = i->second;
		if (now - user->getPreviousPing() >= _timeout)
		{
			user->setStatus(OFFLINE);
			user->write(user->getNick() + "timed out");
			sendColorMsg(user->getFd(), TIMEOUT_ERR, RED);
			delUser(user);
		}
		else if (user->getStatus() == ONLINE)
			user->write("PING " + user->getNick());
	}
}

// void Server::printUsers()
// {
// 	char buffer[42];
// 	snprintf(buffer, sizeof(buffer), "%-4s %-9s %s", "FD", "Nickname", "Host");
// 	history.set(_listen_fd, string("\n") + buffer);
// 	for (map<int, User *>::iterator it = _users.begin(); it != _users.end(); ++it)
// 	{
// 		User *user = (*it).second;
// 		snprintf(buffer, sizeof(buffer), "\033[34m%-4i \033[33m%-9s \033[35m", user->getFd(), user->getNick().c_str());
// 		history.set(user->getFd(), buffer + user->getHost());
// 	}
// }

// void Server::printChannels()
// {
// 	for (map<string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it)
// 	{
// 		Channel *channel = (*it).second;
// 		sendMsg()

// 	}

// }

void Server::updatePoll()
{
	for (vector<pollfd>::iterator i = _pollfds.begin(); i != _pollfds.end(); ++i)
	{
		if (i->revents & POLLIN) {
			if (_users.find(i->fd) != _users.end()) {
				receiveMsg(i);
			}
		}
	}

	for (map<const int, User *>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->second->getStatus() == OFFLINE)
		{
			cout << BLUE << "User " << MAGENTA << it->second->getNick();
			cout << RED << " disconnected" << WHITE << endl;
			delUser(it->second);
		}
	}
}


void Server::sendClear(const int user_fd)
{
	if (_users.at(user_fd)->getCapable())
		return;

	if (send(user_fd, CLEAR_MSG, 5, 0) == -1)
		error("Error sending message", CONTINUE);
}

void Server::sendMsg(const int user_fd, const string &msg)
{
	if (send(user_fd, (msg + MESSAGE_END).c_str(), msg.size() + 2, 0) == SENDING_ERROR)
		error("Error sending message", CONTINUE);
}

void Server::sendMsg(const int user_fd, const int n)
{
	const string msg = toString(n);
	if (send(user_fd, (msg + MESSAGE_END).c_str(), msg.size() + 2, 0) == SENDING_ERROR)
		error("Error sending message", CONTINUE);
}

void Server::sendColorMsg(const int user_fd, const string &msg, const string &color)
{
	string	str;

	if (_users.at(user_fd)->getCapable())
		str = msg + MESSAGE_END;
	else
		str = color + msg + RESET + MESSAGE_END;

	if (send(user_fd, str.c_str(), str.size(), 0) == -1)
		error("Error sending message", CONTINUE);
}

/*
* Command: ERROR
* Parameters: <reason>
This message is sent from a server to a client to report a fatal error, before terminating the client’s connection
This MUST only be used to report fatal errors. Regular errors should use the appropriate numerics or the IRCv3 standard replies framework.
*/
void Server::sendError(const int user_fd, const string &reason)
{
	sendMsg(user_fd, "ERROR :" + reason);
	delUser(_users.at(user_fd));
}



// Since application is single threaded, is a msg buffer for each client needed?
// Can't we just process each fd and flush the parsed content one by one?

void Server::receiveMsg(vector<pollfd>::const_iterator it)
{
	User			*user = _users.at(it->fd);
	char			buf[BUFFER + 1] = { 0 };
	const ssize_t	size = recv(user->getFd(), &buf, BUFFER, 0);

	if (size == SENDING_ERROR) {
		error("Failed recv", CONTINUE);
		return ;
	}
	else if (size == 0) {
		user->setStatus(OFFLINE); // Perhaps user can be removed instantly
		// cout << BLUE << "User " << MAGENTA << user->getNick();
		// cout << RED << " disconnected" << WHITE << endl;
		// delUser(user);
		return ;
	}

	struct s_msg msg = this->parseMessage(user, buf);
	// cout << "in pckg: " << buf << endl << flush;
	printMsg2(it->fd, buf);

	user->buffer += buf;

	const string	delimiter(MESSAGE_END);
	size_t			position;
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
	if (channel && !msg.command)
		channel->setMsg(buf, user->getNick());
}



void Server::printMsg(vector<pollfd>::const_iterator it)
{
	cout << timestamp();

	const int	user_fd = it->fd;
	User		*user = _users.at(user_fd);

	cout << MAGENTA << user->getNick() << WHITE << " @ FD# " << user_fd << ":" << endl;
	cout << user->buffer;
	cout << "*end of message*" << endl;

	user->buffer.clear();
}

void Server::printMsg2(const int user_fd, const char *msg)
{
	cout << timestamp();

	User *user = _users.at(user_fd);

	cout << MAGENTA << user->getNick();
	if (user->getCapable())
		cout << GREEN;
	else
		cout << RED;
	cout << " CAP" << WHITE << " @ FD# " << user_fd << ":" << endl;
	cout << msg;
	cout << "*end of message*" << endl << endl;
}
