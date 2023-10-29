/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 15:56:34 by fporto            #+#    #+#             */
/*   Updated: 2023/10/29 14:55:53 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/numericreplies.hpp"
#include "../includes/user.hpp"
// #include "main.hpp"

const std::string rpl_welcome(const User *dest) {
	return dest->getNick() + " :Welcome to the " + NETWORK_NAME + " Network, " + dest->getNick();
}
const std::string rpl_issupport(const User *dest) {
	return dest->getNick() + " CHANNELLEN=" + toString(CHANNEL_NAME_MAX_LEN) + " :are supported by this server";
}
const std::string rpl_umodeis(const User *dest) {
	return dest->getNick() + " " + dest->getModes();
}
const std::string rpl_away(const User *dest, const string &targetNick, const string &msg) {
	return dest->getNick() + " " + targetNick + " :" + msg;
}
const std::string rpl_list(const User *dest) {
	const Channel *ch = dest->getChannel();
	return dest->getNick() + " " + ch->getName() + " " + toString(ch->getUsers().size()) + " :" + ch->getTopic();
}
const std::string rpl_channelmodeis(const User *dest, const vector<string> &params) {
	return dest->getNick() + " " + joinStrings(params);
}
const std::string rpl_notopic(const User *dest, const std::string &channelName) {
	return dest->getNick() + " " + channelName + " :No topic is set";
}
const std::string rpl_topic(const User *dest, const string &channelName) {
	return dest->getNick() + " " + channelName + " :" + server->getChannel(channelName)->getTopic();
}
const std::string rpl_namreply(const User *dest, const string &channelName) {
	const Channel *ch = server->getChannel(channelName);
	const std::vector<User *> users = ch->getUsers();

	std::string reply = dest->getNick() + " " + ch->getStatus() + " " + ch->getName();

	for (std::vector<User *>::const_iterator it = users.begin(); it != users.end(); ++it) {
		User *user = *it;
		if (user->isInvisible())
			continue;

		if (ch->isOperator(user))
			reply += '@';

		reply += user->getNick();

		if (it + 1 != users.end())
			reply += ' ';
	}
	return reply;
}
const std::string rpl_endofnames(const User *dest, const std::string &channelName) {
	return dest->getNick() + " " + channelName + " End of /NAMES list";
}



const std::string err_nosuchnick(const User *dest, const std::string &nick) {
	return dest->getNick() + " " + nick + " :No such nick/channel";
}
const std::string err_nosuchserver(const User *dest, const std::string &serverName) {
	return dest->getNick() + " " + serverName + " :No such server";
}
const std::string err_nosuchchannel(const User *dest, const std::string &channelName) {
	return dest->getNick() + " " + channelName + " :No such channel";
}
const std::string err_cannotsendtochan(const User *dest, const std::string &channelName) {
	return dest->getNick() + " " + channelName + " :Cannot send to channel";
}
const std::string err_norecipient(const User *dest, const std::string &cmd) {
	return dest->getNick() + " :No recipient given (" + cmd + ")";
}
const std::string err_notexttosend(const User *dest) {
	return dest->getNick() + " :No text to send";
}
const std::string err_inputtoolong(const User *dest) {
	return dest->getNick() + " :Input line was too long";
}
const std::string err_toomanychannels(const User *dest, const std::string &attemptedChannel) {
	return dest->getNick() + " " + attemptedChannel + " :You have joined too many channels";
}
const std::string err_nonicknamegiven(const User *dest) {
	return dest->getNick() + " :No nickname given";
}
const std::string err_erroneusnickname(const User *dest, const std::string &nick) {
	return dest->getNick() + " " + nick + " :Erroneus nickname";
}
const std::string err_nicknameinuser(const User *dest, const std::string &nick) {
	return dest->getNick() + " " + nick + " :Nickname is already in use";
}
const std::string err_needmoreparams(const User *dest, const std::string &command) {
	return dest->getNick() + " " + command + " :Not enough parameters";
}
const std::string err_alreadyregistered(const User *dest) {
	return dest->getNick() + " :You may not reregister";
}
const std::string err_passwdmismatch(const User *dest) {
	return dest->getNick() + " :Password incorrect";
}
const std::string err_channelisfull(const User *dest, const std::string &channelName) {
	return dest->getNick() + " " + channelName + " :Cannot join channel (+l)";
}
const std::string err_badchannelkey(const User *dest, const std::string &channelName) {
	return dest->getNick() + " " + channelName + " :Channel join channel (+k)";
}
const std::string err_badchanmask(const std::string &channelName) {
	return channelName + " :" + CH_NAMING_ERR;
}
const std::string err_chanoprivsneeded(const User *dest, const std::string &channelName) {
	return dest->getNick() + " " + channelName + " :You're not channel operator";
}
const std::string err_umodeunknownflag(const User *dest) {
	return dest->getNick() + " :Unknown MODE flag";
}
const std::string err_usersdontmatch(const User *dest) {
	return dest->getNick() + " :Cant change mode for other users";
}
const std::string err_invalidkey(const User *dest, const std::string &targetChannel) {
	return dest->getNick() + " " + targetChannel + " :Key is not well-formed";
}
