/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numericreplies.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 15:56:34 by fporto            #+#    #+#             */
/*   Updated: 2023/11/05 03:04:59 by fporto           ###   ########.fr       */
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
const std::string rpl_umodeis(const std::string &nick) {
	const User *user = server->getUser(nick);
	if (!user)
		error("No user found @rpl_umodeis", EXIT);

	return user->getNick() + " " + user->getModes();
}
const std::string rpl_away(const User *dest, const std::string &targetNick) {
	const User *target = server->getUser(targetNick);
	if (!target)
		error("No user found @rpl_away", EXIT);

	return dest->getNick() + " " + targetNick + " :" + target->getAwayMsg();
}
const std::string rpl_whoisuser(const User *dest, const std::string &targetNick) {
	const User *target = server->getUser(targetNick);
	if (!target)
		error("No user found @rpl_whoisuser", EXIT);

	return dest->getNick() + " " + targetNick + " " + target->getUser() + " " + target->getHostname() +" * :" + target->getName();
}
const std::string rpl_whoisoperator(const User *dest, const std::string &targetNick) {
	return dest->getNick() + " " + targetNick + " :is an IRC operator";
}
const std::string rpl_endofwho(const User *dest, const std::string &mask) {
	return dest->getNick() + " " + mask + " :End of WHO list";
}
const std::string rpl_whoisidle(const User *dest, const std::string &targetNick) {
	const User *target = server->getUser(targetNick);
	if (!target)
		error("No user found @rpl_whoisidle", EXIT);

	return dest->getNick() + " " + targetNick + " " + toString(time(NULL) - target->getIdleSince()) + " " + toString(time(NULL) - target->getJoinTime()) + " :seconds idle, signon time";
}
const std::string rpl_endofwhois(const User *dest, const std::string &targetNick) {
	return dest->getNick() + " " + targetNick + " :End of /WHOIS list";
}
const std::string rpl_whoischannels(const User *dest, const std::string &targetNick) {
	const User *target = server->getUser(targetNick);
	if (!target)
		error("No user found @rpl_whoischannels", EXIT);

	std::string ret = dest->getNick() + " " + targetNick + " :";

	const std::vector<Channel *> joined = target->getJoinedChannels();
	for (std::vector<Channel *>::const_iterator it = joined.begin(); it != joined.end(); ++it) {
		if ((*it)->isOperator(target))
			ret += '@';
		ret += (*it)->getName();

		if (it + 1 != joined.end())
			ret += ' ';
	}
	return ret;
}
const std::string rpl_list(const User *dest) {
	const Channel *channel = dest->getChannel();
	if (!channel)
		error("No channel found @rpl_list", EXIT);

	return dest->getNick() + " " + channel->getName() + " " + toString(channel->getUsers().size()) + " :" + channel->getTopic();
}
const std::string rpl_channelmodeis(const User *dest, const std::vector<std::string> &params) {
	return dest->getNick() + " " + joinStrings(params);
}
const std::string rpl_notopic(const User *dest, const std::string &channelName) {
	return dest->getNick() + " " + channelName + " :No topic is set";
}
const std::string rpl_topic(const User *dest, const std::string &channelName) {
	return dest->getNick() + " " + channelName + " :" + server->getChannel(channelName)->getTopic();
}
const std::string rpl_topicwhotime(const User *dest, const std::string &channelName) {
	const Channel *channel = server->getChannel(channelName);
	if (!channel)
		error("No channel found @rpl_topicwhotime", EXIT);

	return dest->getNick() + " " + channelName + " " + channel->getTopicSetBy() + " " + channel->getTopicSetAt();
}
const std::string rpl_inviting(const User *dest, const std::string &invitedNick, const std::string &channelName) {
	return dest->getNick() + " " + invitedNick + " " + channelName;
}
const std::string rpl_whoreply(const User *dest, const std::vector<std::string> &params) {
	const std::string channelName = params.size() > 1 ? params[1] : "*";

	const User *target = server->getUser(params[0]);
	if (!target)
		error("No user found @rpl_whoreply", EXIT);

	std::string flags = target->isAway() ? "G" : "H";
	if (server->isOperator(target))
		flags += '*';

	return dest->getNick() + " " + channelName + " " + target->getUser() + " " + target->getHostname() + " " + target->getServername() + " " + target->getNick() + " " + flags + " :0 " + target->getName();
}
const std::string rpl_namreply(const User *dest, const std::string &channelName) {
	const Channel *channel = server->getChannel(channelName);
	if (!channel)
		error("No channel found @rpl_namreply", EXIT);

	const std::vector<User *> users = channel->getUsers();

	std::string reply = dest->getNick() + " " + channel->getStatus() + " " + channelName + " :";

	for (std::vector<User *>::const_iterator it = users.begin(); it != users.end(); ++it) {
		const User *user = *it;
		if (user->isInvisible())
			continue;

		if (channel->isOperator(user))
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
const std::string rpl_whoismodes(const User *dest, const std::string &targetNick) {
	const User *target = server->getUser(targetNick);
	if (!target)
		error("No user found @rpl_whoismodes", EXIT);

	return dest->getNick() + " " + targetNick + " :is using modes " + target->getModes();
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
const std::string err_notonchannel(const User *dest, const std::string &channelName) {
	return dest->getNick() + " " + channelName + " :You're not on that channel";
}
const std::string err_useronchannel(const User *dest, const std::string &targetNick, const std::string &channelName) {
	return dest->getNick() + " " + targetNick + " " + channelName + " :is already on channel";
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
const std::string err_inviteonlychan(const User *dest, const std::string &targetChannel) {
	return dest->getNick() + " " + targetChannel + " :Cannot join channel (+i)";
}
