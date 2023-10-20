/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 15:56:34 by fporto            #+#    #+#             */
/*   Updated: 2023/10/20 14:10:29 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

// https://modern.ircdocs.horse/#message-format
const std::string getReply(Replies type, User *dest, const std::string &tags, const std::string &src, const std::string &cmd, const vector<std::string> &params)
{
	std::string reply;

	if (!tags.empty())
		reply += "@" + tags + " ";

	if (!src.empty())
		reply += ":" + src + " ";

	// if (!cmd.empty())
	// 	return;

	reply += type + " ";
	switch (type)
	{
	case RPL_WELCOME:
		reply += rpl_welcome(dest);
	case RPL_ISUPPORT:
		reply += rpl_issupport(dest);
	case RPL_LIST:
		reply += rpl_list(dest);
	case RPL_TOPIC:
		reply += rpl_topic(dest);
	case RPL_NAMREPLY:
		reply += rpl_namreply(dest);
	case RPL_ENDOFNAMES:
		reply += rpl_endofnames(dest, params[0]);
	default:
		error("Missing reply for numeric " + type, CONTINUE);
		return "";
	}
	return reply;
}

const std::string rpl_welcome(User *dest) {
	return dest->getNick() + ":Welcome to the " + NETWORK_NAME + " Network, " + dest->getNick();
}

const std::string rpl_issupport(User *dest) {
	return dest->getNick() + " CHANNELLEN=" + toString(CHANNEL_NAME_MAX_LEN) + " :are supported by this server";
}

const std::string rpl_list(User *dest) {
	Channel *ch = dest->getChannel();
	return dest->getNick() + " " + ch->getName() + " " + toString(ch->getUsers().size()) + " :" + ch->getTopic();
}

const std::string rpl_topic(User *dest) {
	Channel *ch = dest->getChannel();
	if (!ch->getTopic().empty())
		return dest->getNick() + " " + ch->getName() + " :" + ch->getTopic();
}

const std::string rpl_namreply(User *dest) {
	Channel *ch = dest->getChannel();
	const vector<User *> users = ch->getUsers();

	std::string reply = dest->getNick() + " " + ch->getStatus() + " " + ch->getName();

	for (vector<User *>::const_iterator it = users.begin(); it != users.end(); ++it) {
		if (ch->isOperator(*it))
			reply += '@';

		reply += (*it)->getNick();

		if (it + 1 != users.end())
			reply += ' ';
	}
	return reply;
}

const std::string rpl_endofnames(User *dest, const std::string &channelName) {
	return dest->getNick() + " " + channelName + " End of /NAMES list";
}

void sendError(Errors type, User *dest, const std::string &tags, const std::string &src, const std::string &cmd, const vector<std::string> &params)
{
	std::string reply;

	reply += type + " ";
	switch (type)
	{
	case ERR_NOSUCHCHANNEL:
		reply += err_nosuchchannel(dest, params[0]);
	case ERR_TOOMANYCHANNELS:
		reply += err_toomanychannels(dest);
	case ERR_NONICKNAMEGIVEN:
		reply += err_nonicknamegiven(dest);
	case ERR_ERRONEUSNICKNAME:
		reply += err_erroneusnickname(dest, params[0]);
	case ERR_NICKNAMEINUSE:
		reply += err_nicknameinuser(dest, params[0]);
	case ERR_NEEDMOREPARAMS:
		reply += err_needmoreparams(dest, cmd);
	case ERR_ALREADYREGISTERED:
		reply += err_alreadyregistered(dest);
	case ERR_PASSWDMISMATCH:
		reply += err_passwdmismatch(dest);
	case ERR_BADCHANNELKEY:
		reply += err_badchannelkey(dest);
	case ERR_INVALIDKEY:
		reply += err_badchannelkey(dest);
	default:
		error("Missing error for numeric " + type, CONTINUE);
		return;
	}
	g_server->sendMsg(dest, reply);
}

const std::string err_nosuchchannel(User *dest, const std::string &channelName) {
	return dest->getNick() + " " + channelName + " :No such channel";
}

const std::string err_inputtoolong(User *dest) {
	return dest->getNick() + " :Input line was too long";
}

const std::string err_toomanychannels(User *dest) {
	return dest->getNick() + " " + dest->getAtChannel() + " :You have joined too many channels";
}

const std::string err_nonicknamegiven(User *dest) {
	return dest->getNick() + " :No nickname given";
}

const std::string err_erroneusnickname(User *dest, const std::string &nick) {
	return dest->getNick() + " " + dest->getNick() + " :Erroneus nickname";
}

const std::string err_nicknameinuser(User *dest, const std::string &nick) {
	return dest->getNick() + " " + nick + " :Nickname is already in use";
}

const std::string err_needmoreparams(User *dest, const std::string &command) {
	return dest->getNick() + " " + command + " :Not enough parameters";
}

const std::string err_alreadyregistered(User *dest) {
	return dest->getNick() + " :You may not reregister";
}

const std::string err_passwdmismatch(User *dest) {
	return dest->getNick() + " :Password incorrect";
}

const std::string err_badchannelkey(User *dest) {
	return dest->getNick() + " " + dest->getAtChannel() + " :Channel join channel (+k)";
}

const std::string err_invalidkey(User *dest, const std::string targetChannel) {
	return dest->getNick() + " " + targetChannel + " :Key is not well-formed";
}
