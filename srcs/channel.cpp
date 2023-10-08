/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:16:55 by jibanez-          #+#    #+#             */
/*   Updated: 2023/10/08 16:38:15 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

Channel::Channel() : _mode("n") {}
Channel::Channel(std::string name, Server *server) : _mode("n"), _server(server)
{
	_name = name;

	set("Welcome to " + name);
	set("Este canal es de " + name);
}
Channel::~Channel() {}

void Channel::setName(string name) { _name = name; }
string Channel::getName() { return _name; }

void Channel::setDescription(string description) { _description = description; }
string Channel::getDescription() { return _description; }

void Channel::addUser(User &user) { _users[user.getFd()] = &user; }
void Channel::removeUser(User &user) { _users.erase(_users.find(user.getFd())); }
void Channel::removeUser(string const &nick)
{
	for (map<int, User *>::iterator i = _users.begin(); i != _users.end(); ++i)
	{
		if (i->second->getNick() == nick)
		{
			_users.erase(i);
			return;
		}
	}
}

vector<User *> Channel::getUsers()
{
	vector<User *> users = vector<User *>();

	for (map<int, User *>::iterator i = _users.begin(); i != _users.end(); ++i)
		users.push_back(i->second);
	return users;
}

bool Channel::isUser(User &user) { return _users.find(user.getFd()) != _users.end(); }
bool Channel::isOnChannel(int const &fd)
{
	 for (map<int, User *>::iterator i = _users.begin(); i != _users.end(); ++i)
		if (i->second->getFd() == fd)
			return true;
	return false;
}

void Channel::setMode(string mode) { _mode = mode; }
string Channel::getMode() { return _mode; }

void Channel::setUserMode(User &user, string mode) { _user_mode[user.getFd()] = mode; }
string Channel::getUserMode(User &user) { return _user_mode[user.getFd()]; }

void Channel::setPass(string pass) { _pass = pass; }
string Channel::getPass() { return _pass; }

void Channel::setMaxUsers(string users_max) { _users_max = users_max; }
string Channel::getMaxUsers() { return _users_max; }

void Channel::addInvitedUser(User &user) { _invitations.push_back(&user); }
bool Channel::isInvitedUser(User &user) { return find(_invitations.begin(), _invitations.end(), &user) != _invitations.end(); }
void Channel::revokeInvitation(User &user)
{
	vector<User *>::iterator i = find(_invitations.begin(), _invitations.end(), &user);
	if (i != _invitations.end())
		_invitations.erase(i);
}

void Channel::broadcast(User &user, string message)
{
	for (map<int, User *>::iterator i = _users.begin(); i != _users.end(); ++i)
		user.sendPrivateMessage(*i->second, message);
}

void Channel::update()
{
	// clear();
	for (map<int, User *>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (isOnChannel(it->second->getFd()))
		{
			_server->sendClear(it->second->getFd());
			for (map<int, string>::iterator i = _history.begin(); i != _history.end(); i++)
				_server->sendMsg(it->second->getFd(), i->second);
		}
	}
	// cout << i->second << RESET << endl << flush;
}

void Channel::set(string line)
{
	unsigned int last = _history.size();

	_history.insert(std::pair<unsigned int, string>(last, line));
	update();
}

void Channel::remove(unsigned int i)
{
	_history.erase(i);
	update();
}
