/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 19:40:37 by fporto            #+#    #+#             */
/*   Updated: 2023/10/11 19:40:37 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "user.hpp"

User::User(const int fd, struct sockaddr_in addr, Server *server) : _fd(fd), _status(VERIFY), _previousPing(time(0)), _role("user"), _server(server)
{
	//Shouldn't be required in linux. It is to block simultanious accesses to the fd
	fcntl(fd, F_SETFL, O_NONBLOCK);

	char hostname[NI_MAXHOST];

	if (getnameinfo((struct sockaddr *)&addr, sizeof(addr), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) != 0)
		error("Get Name Info", EXIT);

	_hostname = hostname;
	_hostaddr = inet_ntoa(addr.sin_addr);

	setNick("Annon-" + toString(fd));
	setAtChannel("general");
	setChannel(server->getChannel("general"));
	setCapable(false);
}

User::~User() { close(_fd); }



//Setters

void User::setPassword(const string passwd) { _password = passwd; }
void User::setStatus(const int status) { _status = status; }
void User::setPreviousPing(const time_t ping) { _previousPing = ping; }
void User::setHostaddr(const string hostaddr) { _hostaddr = hostaddr; }
void User::setHostname(const string hostname) { _hostname = hostname; }
void User::setServername(const string servername) { _servername = servername; }
void User::setNick(const string nick) { _nick = nick; }
void User::setUser(const string user) { _user = user; }
void User::setName(const string name) { _name = name; }
void User::setRole(const string role) { _role = role; }
void User::setColor(const string color) { _color = color; }
void User::setPreviousNick(const string previousNick) { _previousNick = previousNick; }
void User::setAtChannel(const string atChannel)
{
	if (atChannel == _atChannel)
		return ;

	_atChannel = atChannel;
}
void User::setChannel(Channel *channel) { _channel = channel; }
void User::setCapable(bool capable) { _capable = capable; }



// Getters

int User::getFd() const { return (_fd); }
int User::getStatus() const { return (_status); }
time_t User::getPreviousPing() const { return (_previousPing); }
const string User::getHostaddr() const { return (_hostaddr); }
const string User::getHostname() const { return (_hostname); }
const string User::getServername() const { return (_servername); }
const string User::getHost() const { return (_hostname.size() ? _hostname : _hostaddr); } // Not in use
const string User::getNick() const
{
	if (!_color.empty())
		return (_color + _nick + RESET);
	return (_nick);
}
const string User::getUser() const { return (_user); }
const string User::getName() const { return (_name); }
const string User::getRole() const { return (_role); }
const string User::getColor() const { return (_color); }
const string User::getPreviousNick() const { return (_previousNick); }
const string User::getAtChannel() const { return (_atChannel); }
Channel * User::getChannel() const { return (_channel); }
uint16_t User::getPort() const { return ntohs(_hostport); }
bool User::getCapable() const { return (_capable); }

// Functions
void User::sendPrivateMessage(User *To, const string Message) { To->write(":" + Message); }
void User::write(const string Message) { _waitToSend.push_back(Message); }

void User::push()
{
	if (!_waitToSend.size())
		return ;

	string buffer;
	for (vector<string>::iterator i = _waitToSend.begin(); i != _waitToSend.end(); ++i)
		buffer += *i + MESSAGE_END;
	_waitToSend.clear();

	if (buffer.length())
		return ;

	if (send(_fd, buffer.c_str(), buffer.length(), 0) == SENDING_ERROR)
		error("send", CONTINUE);
}

bool User::isRegistered() const
{
	return (!_user.empty() \
		|| !_hostname.empty() \
		|| !_servername.empty());
}
