/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:16:55 by jibanez-          #+#    #+#             */
/*   Updated: 2023/10/12 15:45:36 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

Channel::Channel() : _mode("n") {}
Channel::Channel(const string name, Server *server) : _name(name), _mode("n"), _server(server)
{
	_type = _name[0];

	const string str = WLC_CH_MSG + name + RESET;

	set(YELLOW_BG + str);
	if (!_topic.empty())
		set(_topic);
}
Channel::~Channel() {} // ? Missing any handling of players' current channel?

// Setters
void Channel::setName(const string name) { _name = name; }
void Channel::setMode(const string mode) { _mode = mode; }
void Channel::setTopic(const string topic) { _topic = topic; }
void Channel::setKey(const string key) { _key = key; }
void Channel::setType(const char type) { _type = type; }
void Channel::setStatus(const char status) { _status = status; }

void Channel::setMaxUsers(const string users_max) { _users_max = users_max; }
void Channel::setUserMode(const User *user, string mode) { _user_mode[user->getFd()] = mode; }

// Getters
const string Channel::getName() const { return _name; }
const string Channel::getMode() const { return _mode; }
const string Channel::getTopic() const { return _topic; }
const string Channel::getKey() const { return _key; }
char Channel::getType() const { return _type; }
char Channel::getStatus() const { return _status; }

const string Channel::getMaxUsers() const { return _users_max; }
const string Channel::getUserMode(User *user) const { return _user_mode.at(user->getFd()); }
vector<User *> Channel::getUsers() const
{
	vector<User *> users = vector<User *>();

	for (map<int, User *>::const_iterator i = _users.begin(); i != _users.end(); ++i)
		users.push_back(i->second);
	return users;
}


void Channel::addUser(User *user) { _users[user->getFd()] = user; }
void Channel::removeUser(User *user) { _users.erase(_users.find(user->getFd())); }
void Channel::removeUser(const string &nick)
{
	for (map<int, User *>::iterator i = _users.begin(); i != _users.end(); ++i)
		if (i->second->getNick() == nick) {
			_users.erase(i);
			return;
		}
}


bool Channel::isUser(User *user) { return _users.find(user->getFd()) != _users.end(); }
bool Channel::isOnChannel(int const &fd)
{
	 for (map<int, User *>::iterator i = _users.begin(); i != _users.end(); ++i)
		if (i->second->getFd() == fd)
			return true;
	return false;
}



void Channel::addInvitedUser(User *user) { _invitations.push_back(user); }
bool Channel::isInvitedUser(User *user) const { return (find(_invitations.begin(), _invitations.end(), user) != _invitations.end()); }
void Channel::revokeInvitation(User *user)
{
	vector<User *>::iterator i = find(_invitations.begin(), _invitations.end(), user);
	if (i != _invitations.end())
		_invitations.erase(i);
}

void Channel::broadcast(User *user, const string &message)
{
	for (map<int, User *>::iterator i = _users.begin(); i != _users.end(); ++i)
		user->sendPrivateMessage(i->second, message);
}

void Channel::update()
{
	// clear();
	for (map<int, User *>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		User *user = it->second;
		if (isOnChannel(user->getFd()))
		{
			_server->sendClear(user->getFd());
			for (map<int, string>::iterator i = _history.begin(); i != _history.end(); i++)
				_server->sendMsg(user->getFd(), i->second);
		}
	}
	// cout << i->second << RESET << endl << flush;
}

void Channel::set(const string &line)
{
	unsigned int last = _history.size();

	_history.insert(pair<unsigned int, string>(last, line));
	update();
}

void Channel::setLog(const string &line)
{
	unsigned int last = _history.size();

	_history.insert(pair<unsigned int, string>(last, "[ log ]: " + line));
	update();
}

void Channel::setMsg(const string &line, const string &nick)
{
	unsigned int last = _history.size();

	_history.insert(pair<unsigned int, string>(last, "[ " + nick + " ]: " + line));
	update();
}

void Channel::remove(const unsigned int i)
{
	_history.erase(i);
	update();
}
