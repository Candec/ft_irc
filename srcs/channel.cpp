/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:16:55 by tpereira          #+#    #+#             */
/*   Updated: 2023/09/24 11:45:49 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

Channel::Channel() : _mode("n") {}
Channel::~Channel() {}

void Channel::setName(std::string name) { _name = name; }
std::string Channel::getName() { return _name; }

void Channel::setDescription(std::string description) { _description = description; }
std::string Channel::getDescription() { return _description; }

void Channel::addUser(User &user) { _users[user.getFd()] = &user; }
void Channel::removeUser(User &user) { _users.erase(_users.find(user.getFd())); }
void Channel::removeUser(std::string const &nick)
{
	for (std::map<int, User *>::iterator i = _users.begin(); i != _users.end(); ++i)
	{
		if (i->second->getNick() == nick)
		{
			_users.erase(i);
			return;
		}
	}
}

std::vector<User *> Channel::getUsers()
{
	std::vector<User *> users = std::vector<User *>();

	for (std::map<int, User *>::iterator i = _users.begin(); i != _users.end(); ++i)
		users.push_back(i->second);
	return users;
}

bool Channel::isUser(User &user) { return _users.find(user.getFd()) != _users.end(); }
bool Channel::isOnChannel(std::string const &nick)
{
	for (std::map<int, User *>::iterator i = _users.begin(); i != _users.end(); ++i)
		if (i->second->getNick() == nick)
			return true;
	return false;
}

void Channel::setMode(std::string mode) { _mode = mode; }
std::string Channel::getMode() { return _mode; }

void Channel::setUserMode(User &user, std::string mode) { _user_mode[user.getFd()] = mode; }
std::string Channel::getUserMode(User &user) { return _user_mode[user.getFd()]; }

void Channel::setPass(std::string pass) { _pass = pass; }
std::string Channel::getPass() { return _pass; }

void Channel::setMaxUsers(std::string users_max) { _users_max = users_max; }
std::string Channel::getMaxUsers() { return _users_max; }

void Channel::addInvitedUser(User &user) { _invitations.push_back(&user); }
bool Channel::isInvitedUser(User &user) { return std::find(_invitations.begin(), _invitations.end(), &user) != _invitations.end(); }
void Channel::revokeInvitation(User &user)
{
	std::vector<User *>::iterator i = std::find(_invitations.begin(), _invitations.end(), &user);
	if (i != _invitations.end())
		_invitations.erase(i);
}

void Channel::broadcast(User &user, std::string message)
{
	for (std::map<int, User *>::iterator i = _users.begin(); i != _users.end(); ++i)
		user.sendPrivateMessage(*i->second, message);
}