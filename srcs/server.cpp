/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:01 by tpereira          #+#    #+#             */
/*   Updated: 2023/11/09 08:45:54 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"
#include "../includes/main.hpp"

/*
	CONSTRUCTORS
*/
Server::Server() : _upTime(time(0))
{
	// history.clear();
	// history.set("Welcome to the FT_IRC server");

	std::ifstream file("./Configuration/irc.config", std::ios::in);
	std::string line;

	while (getline(file, line))
	{
		if (line.substr(0, line.find(":")) == "default_port")
			_port = atoi(line.substr(1, line.find(":")).c_str());

		if (line.substr(0, line.find(":")) == "default_pass")
			_password = line.substr(1, line.find(":"));

		if (line.substr(0, line.find(":")) == "server_name")
			_name = atoi(line.substr(1, line.find(":")).c_str());

		if (line.substr(0, line.find(":")) == "max_users")
			_maxUsers = atoi(line.substr(1, line.find(":")).c_str());

		if (line.substr(0, line.find(":")) == "max_ping")
			_ping = atoi(line.substr(1, line.find(":")).c_str());

		if (line.substr(0, line.find(":")) == "timeout")
			_timeout = atoi(line.substr(1, line.find(":")).c_str());

		if (line.substr(0, line.find(":")) == "default_channels")
		{
			std::string channel;
			size_t pos = 0;
			while ((pos = line.find(',')) != std::string::npos)
			{
				channel = line.substr(0, pos);
				createChannel(channel);
				line.erase(0, pos + 1);
			}
		}
	}
}

Server::Server(const char * const port, const std::string password) : _upTime(time(0))
{
	setPort(port);
	setPassword(password);
	setup();

	// history.clear();
	// history.set("Welcome to the FT_IRC server");

	parseConfig();
}

Server::Server(const char * const port) : _upTime(time(0))
{
	setPort(port);
	setPassword(NULL);
	setup();

	// history.clear();
	// history.set("Welcome to the FT_IRC server");

	parseConfig();
}

Server::~Server()
{
	log(std::string(RED) + "Shutting down server", false);
	std::cout << std::endl << RED << "Shutting down server" << WHITE << std::endl << std::flush;

	for (std::map<int, User *>::iterator i = _users.begin(); i != _users.end(); ++i) {
		sendErrFatal(i->second, "Server shutdown");
		delUser(i->second);
	}

	close(_listen_fd);

	std::cout << "Server closed" << std::endl << std::flush;
	exit(EXIT_SUCCESS);
}



void Server::parseConfig()
{
	log("Server: Parsing config...");

	std::ifstream file(CONFIG_FILE);
	std::string line;

	while (getline(file, line))
	{
		// cout << "line: " << line << std::endl << std::flush;
		if (line.substr(0, line.find(":")) == "server_name")
			_name = line.erase(0, line.find(":") + 1);

		if (line.substr(0, line.find(":")) == "max_users")
			_maxUsers = atoi(line.erase(0, line.find(":") + 1).c_str());

		if (line.substr(0, line.find(":")) == "max_ping")
			_ping = atoi(line.erase(0, line.find(":") + 1).c_str());

		if (line.substr(0, line.find(":")) == "timeout")
			_timeout = atoi(line.erase(0, line.find(":") + 1).c_str());

		if (line.substr(0, line.find(":")) == "default_channels")
		{
			std::string channel;
			size_t pos = 0;
			line.erase(0, line.find(":") + 1);
			while ((pos = line.find(',')) != std::string::npos)
			{
				channel = line.substr(0, pos);
				createChannel(channel);
				line.erase(0, pos + 1);
			}
			channel = line.substr(0, pos);
			createChannel(channel);
		}
	}
}

void Server::setup()
{
	log("Server: Setting up...");

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
	// cout << "IP: " << inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN)  << std::flush;

	_pollfds_in.push_back(pollfd());
	_pollfds_in.back().fd = _listen_fd;
	_pollfds_in.back().events = POLLIN;
}

void Server::run()
{
	log(std::string("Server: ") + GREEN + "Listening" + BLUE + " on port " + YELLOW + toString(_port), false);
	std::cout << BLUE << "Listening on port " << YELLOW << this->_port << WHITE << std::endl << std::flush;

	while (true) {
		if (poll(&_pollfds_in[0], _pollfds_in.size(), -1) == SENDING_ERROR)
			error("Failed poll _pollfds_in", EXIT);

		if (time(0) - _previousPing >= _ping)
			updatePing();
		else if (_pollfds_in[0].revents == POLLIN)
			createUser();
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
	std::stringstream err;
	err << "Port must be a number in range [" << 1024 << ", " << UINT16_MAX << "]";

	std::cout << BLUE << "Listening on Port: " << YELLOW << arg << RESET << std::endl << std::flush;
	// Check if arg is number
	for (size_t i = 0; i < strlen(arg); ++i)
		if (!isdigit(arg[i]))
			error(err.str(), EXIT);

	std::ostringstream err2;
	// Check if port is within acceptable range
	int tmp_port = atoi(arg);
	if (1024 > tmp_port || tmp_port > UINT16_MAX)
		error(err.str(), EXIT);

	_port = static_cast<uint16_t>(tmp_port);
}

void Server::setPassword(const std::string &password) { _password = password; }



/*
	GETTERS
*/
std::vector<User *> Server::getUsers() const
{
	std::vector<User *> usersV;
	for (std::map<int, User *>::const_iterator i = _users.begin(); i != _users.end(); ++i)
		usersV.push_back(i->second);

	return (usersV);
}

Channel *Server::getChannel(const std::string &channelName) const
{
	if (_channels.find(channelName) != _channels.end())
		return _channels.at(channelName);
	return NULL;
}

std::vector<Channel *> Server::getChannels() const
{
	std::vector<Channel *> channelsV;
	for (std::map<std::string, Channel *>::const_iterator i = _channels.begin(); i != _channels.end(); ++i)
		channelsV.push_back(i->second);

	return (channelsV);
}



/*
	CREATORS
*/
void Server::createUser()
{
	// cout << BLUE << "Adding user..." << WHITE << std::endl << std::flush;
	log(std::string("Server: ") + GREEN + "Adding" + BLUE + " user...");

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

	User *user = new User(user_fd, addr);
	_users[user_fd] = user;

	std::cout << BLUE << "User " << GREEN << "connected" << BLUE << " from ";
	std::cout << user->getHostaddr() << ":" << user->getPort() << WHITE << std::endl << std::flush;

	_pollfds_in.push_back(pollfd());
	_pollfds_in.back().fd = user_fd;
	_pollfds_in.back().events = POLLIN;

	_pollfds_out.push_back(pollfd());
	_pollfds_out.back().fd = user_fd;
	_pollfds_out.back().events = POLLOUT;

	if (_users.size() == _maxUsers && (shutdown(_listen_fd, SHUT_RD) == SENDING_ERROR))
		error("Failed shutdown of receptions on listening socket", CONTINUE);
}

Channel *Server::createChannel(const std::string &channelName)
{
	if (!isValidChannelName(channelName, true))
		return NULL;

	// cout << GREEN << "Creating channel " << YELLOW << channelName << RESET << std::flush;
	log(std::string("    Server: ") + GREEN + "Creating " + YELLOW + channelName + RESET);

	Channel *channel = new Channel(channelName);
	if (!channel) {
		std::cout << RED << " failed" << WHITE << std::endl << std::flush;
		// error("Channel " + channelName + " creation failed", CONTINUE);
		log(std::string("        ") + YELLOW + channelName + RED + " not created");
	}
	else {
		_channels.insert(std::pair<std::string, Channel *>(channelName, channel));
		// _channels[channelName] = channel;
		// cout << GREEN << " created" << WHITE << std::endl << std::flush;
		log(std::string("        ") + YELLOW + channelName + GREEN + " created");
	}
	// cout << "channel [ mem: " << _channels[channelName] << " | << name: " << _channels[channelName]->getName() << " ] created" << std::endl << std::flush;
	return channel;
}
Channel *Server::createChannel(const std::string &channelName, User *creator)
{
	if (isValidChannelName(channelName, true))
		return createChannel(channelName);
	creator->sendError(ERR_BADCHANMASK, channelName);
	return NULL;
}



/*
	DELETERS
*/
void Server::delUser(User *user)
{
	if (!user)
		return;

	if (poll(&_pollfds_out[0], _pollfds_out.size(), -1) == SENDING_ERROR)
		error("Failed poll _pollfds_out", EXIT);
	for (std::vector<pollfd>::iterator i = _pollfds_out.begin(); i != _pollfds_out.end(); ++i)
		if (i->fd == user->getFd() && i->revents == POLLOUT)
			sendBuffer(i->fd);

	std::cout << MAGENTA << user->getNick();
	std::cout << RED << " disconnected" << WHITE << std::endl << std::flush;
	log(std::string("Server: ") + RED + "Removing " + MAGENTA + user->getNick() + RESET + " (" \
		+ MAGENTA + toString(user->getFd()) + RESET + ")");

	user->leaveAllChannels();

	for (std::vector<pollfd>::iterator l = _pollfds_in.begin(); l != _pollfds_in.end(); ++l) {
		if (l->fd == user->getFd()) {
			_pollfds_in.erase(l);
			break;
		}
	}
	for (std::vector<pollfd>::iterator l = _pollfds_out.begin(); l != _pollfds_out.end(); ++l) {
		if (l->fd == user->getFd()) {
			_pollfds_out.erase(l);
			break;
		}
	}

	_users.erase(user->getFd());

	delete user;
}

void Server::delChannel(const Channel *channel)
{
	log(std::string("Server: ") + RED + "Deleting " + YELLOW + channel->getName());

	if (_channels.find(channel->getName()) != _channels.end())
		_channels.erase(channel->getName());
	delete channel;
}

void Server::updatePing()
{
	// log("Server: Updating ping...");

	_previousPing = time(0);

	time_t	now = time(0);

	for (std::map<int, User *>::iterator i = _users.begin(); i != _users.end(); i++) {
		User *user = i->second;

		if (now - user->getPreviousPing() >= _timeout)
			quitCmd(user, "Timed out");
		else if (user->getStatus() == UserFlags::ONLINE)
			sendMsg(user, "PING " + user->getNick());
	}
}

void Server::updatePoll()
{
	for (std::vector<pollfd>::iterator i = _pollfds_in.begin(); i != _pollfds_in.end(); ++i)
	{
		if (i->revents & POLLIN && _users.find(i->fd) != _users.end())
			receiveMsg(i);
	}

	if (poll(&_pollfds_out[0], _pollfds_out.size(), -1) == SENDING_ERROR)
		error("Failed poll _pollfds_out", EXIT);
	for (std::vector<pollfd>::iterator i = _pollfds_out.begin(); i != _pollfds_out.end(); ++i)
		if (i->revents == POLLOUT)
			sendBuffer(i->fd);

	for (std::map<int, User *>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->second->getStatus() == UserFlags::OFFLINE
		|| it->second->getStatus() == UserFlags::UNVERIFY)
			delUser(it->second);
	}
}


void Server::sendMsg(User *user, const int n) const
{
	sendMsg(user, toString(n));
}
void Server::sendMsg(User *user, const std::string &msg, const std::string &src) const
{
	sendMsg(user, ":" + src + "@" + server->getName() + " " + msg);
}
void Server::sendMsg(const int user_fd, const int n) const
{
	sendMsg(user_fd, toString(n));
}
void Server::sendMsg(const int user_fd, const std::string &msg) const
{
	sendMsg(_users.at(user_fd), msg);
}
void Server::sendMsg(User *user, const std::string &msg) const
{
	user->send_buffer += msg + ' ' + MESSAGE_END;
}

void Server::sendBuffer(const int user_fd) const
{
	User *const user = _users.at(user_fd);

	if (user->send_buffer.empty())
		return;

	std::ostringstream oss;
	oss << "SEND " << RESET << timestamp() << BLUE << "Server: Sending to " \
		<< MAGENTA << user->getNick() << RESET << " (" \
		<< MAGENTA << user->getFd() << RESET << "):" << std::endl \
		<< user->send_buffer << std::flush;
	log(oss.str());

	if (send(user->getFd(), user->send_buffer.c_str(), user->send_buffer.size(), 0) == SENDING_ERROR)
		return error("Error sending message", CONTINUE);

	user->send_buffer.clear();
}

void Server::broadcast(const std::string &message) const
{
	for (std::map<int, User *>::const_iterator it = _users.begin(); it != _users.end(); ++it)
		sendMsg(it->second, message);
}

void Server::sendColorMsg(const int user_fd, const std::string &msg, const std::string &color) const
{
	if (_users.find(user_fd) == _users.end())
		return error("User not found @sendColorMsg", EXIT);

	User *user = _users.at(user_fd);
	sendColorMsg(user, msg, color);
}
void Server::sendColorMsg(User *user, const std::string &msg, const std::string &color) const
{
	if (!user->isCapable())
		return sendMsg(user, msg);
	sendMsg(user, color + msg + RESET);
}

void Server::sendClear(User *user) const
{
	if (user->isCapable())
		return;
	sendMsg(user, CLEAR_MSG);
}
void Server::sendClear(const int user_fd) const
{
	if (_users.find(user_fd) == _users.end())
		return error("User not found @sendClear", EXIT);

	User *user = _users.at(user_fd);
	sendClear(user);
}

/*
* Command: ERROR
* Parameters: <reason>
This message is sent from a server to a client to report a fatal error, before terminating the client’s connection
This MUST only be used to report fatal errors. Regular errors should use the appropriate numerics or the IRCv3 standard replies framework.
*/
void Server::sendErrFatal(User *user, const std::string &reason)
{
	if (!reason.empty())
		sendMsg(user, "ERROR " + reason);
	else
		sendMsg(user, "ERROR");
	// delUser(user);
	user->setStatus(UserFlags::OFFLINE);
}

void Server::receiveMsg(std::vector<pollfd>::const_iterator it)
{
	if (_users.find(it->fd) == _users.end())
		error("User not found @receiveMsg", EXIT);
	User			*user = _users.at(it->fd);
	char			buf[BUFFER + 1] = { 0 };
	const ssize_t	size = recv(user->getFd(), &buf, BUFFER, 0);

	if (size == SENDING_ERROR) {
		error("Failed recv", CONTINUE);
		return ;
	}
	else if (size == 0) {
		user->setStatus(UserFlags::OFFLINE); // Perhaps user can be removed instantly
		// cout << BLUE << "User " << MAGENTA << user->getNick();
		// cout << RED << " disconnected" << WHITE << std::endl << std::flush;
		// delUser(user);
		return ;
	}

	user->setPreviousPing(time(NULL));

	std::cout << "in pckg: " << buf << std::endl << std::flush;

	user->recv_buffer += buf;

	if (user->recv_buffer.find('\n') == std::string::npos) {
		user->setStatus(UserFlags::WAITING);
		return;
	}

	printMsg2(user, user->recv_buffer.c_str());
	parseMessage(user, user->recv_buffer.c_str());
	user->recv_buffer.clear();


	// const std::string	delimiter(MESSAGE_END);
	// size_t				position;
	// while ((position = user->buffer.find(delimiter)) != std::string::npos)
	// {
	// 	std::string message = user->buffer.substr(0, position);
	// 	user->buffer.erase(0, position + delimiter.length());
	// 	if (!message.length())
	// 		continue;
	// 	user->push();
	// }
}



// void Server::printMsg(vector<pollfd>::const_iterator it)
// {
// 	std::cout << timestamp();

// 	const int	user_fd = it->fd;
// 	User		*user = _users.at(user_fd);

// 	std::cout << MAGENTA << user->getNick() << WHITE << " @ FD# " << user_fd << ":" << std::endl << std::flush;
// 	std::cout << user->buffer;
// 	std::cout << "*end of message*" << std::endl << std::flush;

// 	user->buffer.clear();
// }

void Server::printMsg2(User *user, const char *msg)
{
	if (!logging)
		return;

	std::ostringstream oss;
	oss << MAGENTA << "RECV " << RESET << timestamp() \
		<< MAGENTA << user->getNick() << RESET << " (" \
		<< MAGENTA << user->getFd() << RESET << ")";
	if (user->isCapable())
		oss << GREEN;
	else
		oss << RED;
	oss << " CAP" << RESET << ":" << std::endl << msg;

	log(oss.str());
	// cout << YELLOW << "*end of message*" << RESET << std::endl << std::endl << std::flush;
}

User * Server::getUser(const std::string &nick) const
{
	if (nick.empty())
		return NULL;

	std::map<int, User *>::const_iterator it;
	for (it = _users.begin(); it != _users.end(); ++it)
		if (it->second->getNick() == nick)
			return it->second;
	return NULL;
}

const std::string Server::getName() const { return _name; }
std::string Server::getPassword() { return(_password); }

bool Server::shareChannels(const User *user1, const User *user2) const
{
	std::vector<Channel *>user1Channels = user1->getJoinedChannels();
	std::vector<Channel *>user2Channels = user2->getJoinedChannels();
	for (std::vector<Channel *>::const_iterator it = user1Channels.begin(); it != user1Channels.end(); ++it)
		for (std::vector<Channel *>::const_iterator it2 = user2Channels.begin(); it2 != user2Channels.end(); ++it2)
			if (*it == *it2)
				return true;
	return false;
}
