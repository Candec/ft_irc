/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message_parsing.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 21:15:51 by fporto            #+#    #+#             */
/*   Updated: 2023/11/08 05:08:31 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/commands.hpp"
#include "../includes/main.hpp"

struct s_msg
Server::parseMessage(User *user, const char * const buffer)
{
	struct s_msg msg;
	msg.src = user;
	msg.timestamp = time(NULL);
	msg.command = false;

	strncpy(msg.buffer, buffer, BUFFER + 1);

	std::vector< std::vector<std::string> > lines = splitBuffer(buffer);

	std::vector< std::vector<std::string> >::iterator line;
	for (line = lines.begin(); line != lines.end(); line++)
		lookForCmd(user, cmdToEnum((*line)[0]), *line, msg);
		// lookForCmd(user, *line, msg);

	return msg;
}

std::vector< std::vector<std::string> >
Server::splitBuffer(const char * const buffer)
{
	std::vector< std::vector<std::string> >	lines;
	std::istringstream						lines_iss(buffer);
	std::string								line;

	while (getline(lines_iss, line) && !line.empty()) {
		//? Maybe ignore empty lines but keep parsing
		//? https://modern.ircdocs.horse/#compatibility-with-incorrect-software
		// if (line.empty())
		// 	continue;

		std::vector<std::string>	words;
		std::istringstream			words_iss(line);
		std::string					word;

		while (words_iss >> word)
			words.push_back(word);

		words.resize(words.size());
		lines.push_back(words);
	}
	lines.resize(lines.size());

	return lines;
}

static bool
expectedArgs(const std::vector<std::string> &args, const size_t n)
{
	if (args.size() == n)
		return true;

	error(args[0], CONTINUE);
	if (args.size() > n)
		error("too many arguments", CONTINUE);
	else
		error("not enough arguments", CONTINUE);
	return false;
}

bool
Server::isCmd(const std::string &param) const { return (cmdToEnum(param) != Commands::UNKNOWN); }
bool
Server::isChannel(const std::string &channel) const { return (_channels.find(channel) != _channels.end()); }
bool
Server::isOperator(const User *user) const { return (_operators.find(user->getFd()) != _operators.end()); }

void
Server::lookForCmd(User *user, const int cmd, std::vector<std::string> params, struct s_msg &msg)
{
	if (params.empty() || user->getStatus() == UserFlags::OFFLINE)
		return;

	params.erase(params.begin());
	msg.command = true;

	switch (cmd)
	{
	case Commands::PASS:
		return passCmd(user, params);
	case Commands::NICK:
		return nickCmd(user, params);
	case Commands::USER:
		return userCmd(user, params);
	case Commands::COLOR:
		return colorCmd(user, params);
	case Commands::KICK:
		return kickCmd(user, params);
	case Commands::JOIN:
		return joinCmd(user, params);
	case Commands::QUIT:
		return quitCmd(user, params[0]);
	case Commands::CAP:
		return capCmd(user, params);
	case Commands::TOPIC:
		return topicCmd(user, params);
	case Commands::PRIVMSG:
		return privmsgCmd(user, params);
	case Commands::AWAY:
		return awayCmd(user, params);
	case Commands::PING:
		return pingCmd(user, params);
	case Commands::MODE:
		return modeCmd(user, params);
	case Commands::INVITE:
		return inviteCmd(user, params);
	case Commands::PART:
		return partCmd(user, params);
	case Commands::WHO:
		return whoCmd(user, params);
	case Commands::WHOIS:
		return whoisCmd(user, params);
	default:
		return;
	}

}


/*
** LIST OF COMMANDS
*/

/*
* Command: PASS
* Parameters: <password>
*/
void
Server::passCmd(User *user, const std::vector<std::string> &params)
{
	const std::string password = params[0];

	if (_password.empty())
	{
		// user->setPassword(NULL);
		user->setStatus(UserFlags::ACCEPT);
		return;
	}

	if (params.size() < 1) {
		return user->sendError(ERR_NEEDMOREPARAMS, "", "PASS");
	} else if (!expectedArgs(params, 1)) // There's a numeric reply for too little but not too many
		return;

	if (password != _password)
	{
		user->sendError(ERR_PASSWDMISMATCH);
		user->setStatus(UserFlags::OFFLINE);
		return;
	}

	std::cout << "++++++++++++++++++++" << std::endl << std::flush;
	std::cout << GREEN << "Password given by " << user->getNick() << " was valid" << RESET << std::endl << std::flush;
	user->setPassword(password);
	user->setStatus(UserFlags::ACCEPT);
}

/*
* Command: NICK
* Parameters: <nickname>
*/
void
Server::nickCmd(User *user, const std::vector<std::string> &params)
{
	std::cout << "status: " << user->getStatus() << std::endl << std::flush;
	if (user->getStatus() == UserFlags::UNVERIFY)
		return sendErrFatal(user, "NICK: No password given");

	std::string validC = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890-_";

	if (params.empty())
		return user->sendError(ERR_NONICKNAMEGIVEN);

	// Sanitize nickname
	const std::string nickName = params[0];
	for (std::string::const_iterator it = nickName.begin(); it != nickName.end(); it++)
	{
		if (validC.find(*it) == std::string::npos)
		{
			std::cout << "inv char: " << *it << std::endl << std::flush;
			return user->sendError(ERR_ERRONEUSNICKNAME, nickName);
		}
	}

	if (params.size() > 1)
		return user->sendError(ERR_ERRONEUSNICKNAME, joinStrings(params));

	// Find duplicate
	if (getUser(nickName) != NULL)
		return user->sendError(ERR_NICKNAMEINUSE, nickName);

	const std::string oldNickName = user->getNick();
	user->setNick(nickName);
	broadcast(":" + oldNickName + " NICK " + nickName);
}

/*
* Command: USER
* Parameters: <username> 0 * <realname>
*/
void
Server::userCmd(User *user, const std::vector<std::string> &params)
{
	if (user->getStatus() == UserFlags::UNVERIFY)
		return sendErrFatal(user, "USER: No password given");

	const size_t n_args = params.size();

	if (user->isRegistered())
		return user->sendError(ERR_ALREADYREGISTERED);

	if (n_args < 4)
		return user->sendError(ERR_NEEDMOREPARAMS, "", "USER");

	user->setUser("~" + params[0]);
	user->setHostname(params[1]);
	user->setServername(params[2]);

	std::string realName = params[3];
	for (size_t i = 4; i < n_args; ++i)
		realName += " " + params[i];
	if (realName[0] == ':')
		realName.erase(realName.begin());
	user->setName(realName);

	if (user->isRegistered())
		log(MAGENTA + user->getNick() + BLUE + " (" \
			+ MAGENTA + toString(user->getFd()) + BLUE + ") is " + GREEN + "registered");

	user->setStatus(UserFlags::ONLINE);

	user->sendReply(RPL_WELCOME);
}

/*
* Command: JOIN
* Parameters: <channel>{,<channel>} [<key>{,<key>}]
* Alt Params: 0
*/
void
Server::joinCmd(User *user, const std::vector<std::string> &params)
{
	// This is equivalent to sending a PART command to each channel
	if (params.size() == 1 && params[0] == "0")
		user->leaveAllChannels();

	// Split vectors by , delimiter -> pair< vector<Channels> vector<Keys> >
	const std::string channelNames = params[0];
	const std::vector<std::string> names = splitString(channelNames, ",");

	std::string channelKeys;
	if (params.size() > 1)
		channelKeys = params[1];
	const std::vector<std::string> keys = splitString(channelKeys, ",");

	std::vector<std::string>::const_iterator names_it = names.begin();
	std::vector<std::string>::const_iterator key_it = keys.begin();
	for (; names_it != names.end(); ++names_it) {
		if (channelKeys.empty())
			user->joinChannel(*names_it, "");
		else
		{
			user->joinChannel(*names_it, *key_it);
			++key_it;
		}
	}

}

void
Server::colorCmd(User *user, const std::vector<std::string> &words)
{
	if (!expectedArgs(words, 2))
		return ;

	std::map<std::string, std::string>colors;

	colors["r"] = RED_BOLD;
	colors["k"] = BLACK_BOLD;
	colors["g"] = GREEN_BOLD;
	colors["y"] = YELLOW_BOLD;
	colors["b"] = BLUE_BOLD;
	colors["p"] = MAGENTA_BOLD;
	colors["c"] = CYAN_BOLD;
	colors["w"] = WHITE_BOLD;
	colors["n"] = BOLD;

	colors["red"] = RED_BOLD;
	colors["black"] = BLACK_BOLD;
	colors["green"] = GREEN_BOLD;
	colors["yellow"] = YELLOW_BOLD;
	colors["blue"] = BLUE_BOLD;
	colors["pink"] = MAGENTA_BOLD;
	colors["cyan"] = CYAN_BOLD;
	colors["white"] = WHITE_BOLD;
	colors["none"] = BOLD;

	colors["RED"] = RED_BOLD;
	colors["BLACK"] = BLACK_BOLD;
	colors["GREEN"] = GREEN_BOLD;
	colors["YELLOW"] = YELLOW_BOLD;
	colors["BLUE"] = BLUE_BOLD;
	colors["PINK"] = MAGENTA_BOLD;
	colors["CYAN"] = CYAN_BOLD;
	colors["WHITE"] = WHITE_BOLD;
	colors["NONE"] = BOLD;

	if (colors.find(words[1]) == colors.end())
		sendColorMsg(user->getFd(), COLOR_ERR, RED);
	user->setColor(colors[words[1]]);
}

/*
* Command: QUIT
* Parameters: [<reason>]
*/
void
Server::quitCmd(User *user, const std::string &reason)
{
	// std::cout << BLUE << "User " << user->getNick() << RED << " disconnected" << RESET << std::endl << std::flush;
	const std::vector<Channel *> channels = user->getJoinedChannels();
	for (std::vector<Channel *>::const_iterator it = channels.begin(); it != channels.end(); ++it)
		(*it)->broadcast(user->getNick() + ": Quit: " + reason);

	sendErrFatal(user, reason);
}

// void
// Server::capCmd(User *user, const std::vector<std::string> &words)
// {
// 	if (
// 		!words[1].compare("LS")
// 		&& !words[2].compare("302")
// 	)
// 		user->setCapable(true);
// 	user->setCapable(false);
// }

/*
* Command: CAP
* Parameters: <subcommand> [:<capabilities>]
*/
void
Server::capCmd(User *user, const std::vector<std::string> &params)
{
	if (params.size() >= 2) {
		if (params[0] == "LS" \
			&& params[1] == "302"
		) {
			user->setCapable(true);
			sendMsg(user, "CAP * LS :");
		}
		else if (params[1] == "END") {
			// user
		}
	}
	// else
	// 	user->setCapable(false);
}

/*
* Command: TOPIC
* Parameters: <channel> [<topic>]
*/
void
Server::topicCmd(User *user, const std::vector<std::string> &params)
{
	if (params.size() == 0)
		return user->sendError(ERR_NEEDMOREPARAMS, "", "TOPIC");

	const std::string channelName = params[0];
	Channel *channel = getChannel(channelName);
	if (!channel)
		return user->sendError(ERR_NOSUCHCHANNEL, channelName);

	if (params.size() == 1) {
		if (!channel->getTopic().empty()) {
			user->sendReply(RPL_TOPIC, channelName);
			user->sendReply(RPL_TOPICWHOTIME, channelName);
		} else
			user->sendReply(RPL_NOTOPIC, channelName);
	}
	else {
		if (channel->isTopicProtected() && !channel->isOperator(user))
			return user->sendError(ERR_CHANOPRIVSNEEDED, channelName);

		std::string topic = joinStringsButFirst(params);
		if (topic[0] == ':')
			topic.erase(topic.begin());

		channel->setTopic(topic, user);

		std::string reply = "TOPIC " + channelName;
		if (!topic.empty())
			reply += " " + topic;

		channel->broadcast(reply, NULL, user->getNick());
	}
}

/*
* Command: PRIVMSG
* Parameters: <target>{,<target>} <text to be sent>
*/
void
Server::privmsgCmd(User *user, const std::vector<std::string> &params)
{
	const std::vector<std::string> targets = splitString(params[0], ",");

	const std::string msg = "PRIVMSG " + joinStrings(params);
	// std::cout << msg << std::endl << std::flush;

	std::vector<std::string>::const_iterator it;
	for (it = targets.begin(); it != targets.end(); ++it) {
		const std::string targetName = *it;

		if (targetName[0] == '$') {
			broadcast(msg);
		}
		else if (getChannel(targetName) != NULL) { // Target's a channel
			Channel *channel = getChannel(targetName);

			if (channel->isBanned(user))
				return;
			if (channel->noExternalMessages() && !channel->isMember(user))
				return user->sendError(ERR_CANNOTSENDTOCHAN, targetName);

			channel->broadcast(msg, user, user->getNick());
		}
		else if (getUser(targetName) != NULL) { // Target's a user
			User *target = getUser(targetName);

			if (target->isAway())
				user->sendReply(RPL_AWAY, targetName);

			sendMsg(target, msg, user->getNick());
		}
		else {
			user->sendError(ERR_NOSUCHNICK, targetName);
		}
	}
}

/*
* Command: AWAY
* Parameters: [<text>]
*/
void
Server::awayCmd(User *user, const std::vector<std::string> &params)
{
	(void)user;
	(void)params;
	//TODO implement
}

/*
* Command: PING
* Parameters: <token>
*/
void
Server::pingCmd(User *user, const std::vector<std::string> &token)
{
	sendMsg(user, "PONG " + _name + " " + joinStrings(token));
}

/*
* Command: MODE
* Parameters: <target> [<modestring> [<mode arguments>...]]
*/
void
Server::modeCmd(User *user, const std::vector<std::string> &params)
{
	const std::string targetName = params[0];
	std::string modeString;
	if (params.size() > 1) {
		modeString = params[1];
		// params.erase()
	}
	std::vector<std::string> modeArguments;
	if (params.size() > 2)
		modeArguments = std::vector<std::string>(params.begin() + 2, params.end());

	//? how to differentiate between user and channel modes?
	if (!(targetName[0] == ChannelFlags::REGULAR \
		|| targetName[0] == ChannelFlags::LOCAL)
	) {
		// User modes
		User *target = getUser(targetName);
		if (!target)
			return user->sendError(ERR_NOSUCHNICK, targetName);

		if (target->getNick() != targetName)
			return user->sendError(ERR_USERSDONTMATCH);
		if (params.size() == 1)
			return user->sendReply(RPL_UMODEIS, targetName);

		char lastFlag = 0;
		std::string::const_iterator it;
		for (it = modeString.begin(); it != modeString.end(); ++it) {
			if (*it == '+') {
				lastFlag = '+';
				continue;
			} else if (*it == '-') {
				lastFlag = '-';
				continue;
			}
			if (lastFlag == '+')
				target->addMode((UserFlags::ModeLetter)*it);
			else if (lastFlag == '-')
				target->removeMode((UserFlags::ModeLetter)*it);
		}
	} else {
		// Channel mode
		Channel *target = getChannel(targetName);
		if (!target)
			return user->sendError(ERR_NOSUCHCHANNEL, targetName);

		if (params.size() == 1)
			return user->sendReply(RPL_CHANNELMODEIS, targetName);
		if (!target->isOperator(user))
			return user->sendError(ERR_CHANOPRIVSNEEDED, targetName);

		char lastFlag = 0;
		std::string::const_iterator it;
		for (it = modeString.begin(); it != modeString.end(); ++it) {
			if (*it == '+') {
				lastFlag = '+';
				continue;
			} else if (*it == '-') {
				lastFlag = '-';
				continue;
			}
			if (lastFlag == '+')
				target->addMode((ChannelFlags::ModeLetter)*it, modeArguments, user);
			else if (lastFlag == '-')
				target->removeMode((ChannelFlags::ModeLetter)*it, modeArguments, user);
		}
		// if (modeString.size() == 1 && !target->isModeImplemented((ChannelFlags::ModeLetter)modeString[0])) // Maybe restrict broadcast when "MODE <channel> b" is received
		// 	return;

		target->broadcast("MODE " + joinStrings(params));
	}
}

/*
* Command: KICK
* Parameters: [<channel>] <user> *( "," <user> ) [<comment>]
*/
void
Server::kickCmd(User *user, std::vector<std::string> &params)
{
	std::vector<std::string> firstSplit;
	std::vector<Channel *> channels;
	std::vector<std::string> nicks;
	vector<std::string> reasonParams;
	std::string reason;

	if (params.size() < 1)
		return user->sendError(ERR_NEEDMOREPARAMS, "");

	firstSplit = splitString(params[0], ",");

	if (isChannel(firstSplit[0]))
	{
		for (std::vector<std::string>::iterator it = firstSplit.begin(); it != firstSplit.end(); ++it)
		{
			if (!getChannel(*it))
				return user->sendError(ERR_NOSUCHCHANNEL, *it);

			channels.push_back(getChannel(*it));
		}
		nicks = splitString(params[1], ",");
	}
	else
	{
		nicks = firstSplit;
		// channels.push_back(user->getChannel());
	}

	if (channels.size() > nicks.size())
		return user->sendError(ERR_NEEDMOREPARAMS, "Required as many channel parameters as there are user parameters");

	if (params.size() > 3)
	{
		for (std::vector<std::string>::iterator it = params.begin() + 2; it != params.end(); ++it)
			reasonParams.push_back(*it);
		reason = joinStrings(reasonParams);
	}
	else
		reason = "you have been kicked by " + user->getNick();

	for (std::vector<Channel *>::iterator i = channels.begin(); i != channels.end(); ++i) {
		Channel *target = *i;
		if (!target->isOperator(user))
			return user->sendError(ERR_CHANOPRIVSNEEDED, target->getName());
	}

	for (std::vector<Channel *>::iterator i = channels.begin(); i != channels.end(); ++i) {
		Channel *channel = *i;
		for (std::vector<std::string>::iterator it = nicks.begin(); it != nicks.end(); ++it)
		{
			User *target = getUser(*it);

			if (!channel) {
				user->sendError(ERR_NOSUCHCHANNEL, *it);
				continue;
			}

			if (!channel->isMember(getUser(*it))) {
				user->sendError(ERR_NOTONCHANNEL, *it);
				continue;
			}

			if (target->isChannelMember(channel->getName()))
				target->leaveChannel(channel);
			sendMsg(target, reason);
			const std::string cast(target->getNick() + " was kicked from " + channel->getName());
			channel->broadcast(cast);
		}
	}
}

/*
* Command: INVITE
* Parameters: <nick> <channel>
*/
void
Server::inviteCmd(User *user, const std::vector<std::string> &params)
{
	// Parameters number check
	if (params.size() < 2)
		user->sendError(ERR_NEEDMOREPARAMS, "", "INVITE");
	else if (params.size() > 2)
		return sendMsg(user, "Too many arguments");

	const User *target = getUser(params[0]);
	if (!target)
		return user->sendError(ERR_NOSUCHNICK, params[0]);

	const std::string channelName = params[1];

	// Getting the channel
	Channel *invChannel = getChannel(channelName);
	if (!invChannel)
		return user->sendError(ERR_NOSUCHCHANNEL, channelName);

	if (!invChannel->isMember(user))
		return user->sendError(ERR_NOTONCHANNEL, channelName);
	if (!invChannel->isOperator(user))
		return user->sendError(ERR_CHANOPRIVSNEEDED, channelName);
	if (invChannel->isMember(target))
		return user->sendError(ERR_USERONCHANNEL, params);

	invChannel->addInvitedUser(target);
	user->sendReply(RPL_INVITING, params);
	server->sendMsg(target, "INVITE " + target->getNick() + " " + channelName, user->getNick());
}


/*
* Command: PART
* Parameters: <channel>{,<channel>} [<reason>]
*/
void
Server::partCmd(User *user, const std::vector<std::string> &params)
{
	if (!params.size())
		user->sendError(ERR_NEEDMOREPARAMS, "", "PART");

	std::vector<std::string> channelNames = splitString(params[0], ",");
	std::string reason = "";

	if (params.size() > 1)
		reason = joinStringsButFirst(params);

	for (std::vector<std::string>::const_iterator it = channelNames.begin(); it != channelNames.end(); ++it)
	{
		Channel *channel = getChannel(*it);
		if (!channel) {
			user->sendError(ERR_NOSUCHCHANNEL, *it);
			continue;
		}

		if (!user->isChannelMember(channel->getName())) {
			user->sendError(ERR_NOTONCHANNEL, *it);
			continue;
		}

		user->leaveChannel(channel, reason);
	}
}

/*
* Command: WHO
* Parameters: <mask>
*/
void
Server::whoCmd(const User *user, const std::vector<std::string> &params)
{
	if (params.empty())
		return error("Empty params @whoCmd", CONTINUE);

	const std::string mask = params[0];
	const Channel	*target_channel = getChannel(mask);
	const User		*target_user = getUser(mask);

	if (target_channel) { // Mask is channel
		const std::vector<const User *> users = target_channel->getUsers();
		for (std::vector<const User *>::const_iterator it = users.begin(); it != users.end(); ++it) {
			const User *tmp_user = *it;

			std::vector<std::string> tmp;
			tmp.push_back(tmp_user->getNick());
			tmp.push_back(mask);

			user->sendReply(RPL_WHOREPLY, tmp);
		}
	} else if (target_user) { // Mask is user
		user->sendReply(RPL_WHOREPLY, mask);
	}
	user->sendReply(RPL_ENDOFWHO, mask);
}

/*
* Command: WHOIS
* Parameters: [<target>] <nick>
*/
void
Server::whoisCmd(const User *user, const std::vector<std::string> &params)
{
	std::string targetNick;

	if (params.size() == 0)
		return user->sendError(ERR_NONICKNAMEGIVEN);
	else if (params.size() == 1)
		targetNick = params[0];
	else if (params.size() == 2)
		targetNick = params[1];

	// ERR_NOSUCHSERVER

	User *target = getUser(targetNick);
	if (!target)
		return user->sendError(ERR_NOSUCHNICK, targetNick);

	if (target->isInvisible() && !shareChannels(user, target))
		return user->sendReply(RPL_ENDOFWHOIS, targetNick);

	if (user->isAway())
		user->sendReply(RPL_AWAY, targetNick);

	if (isOperator(user)) {
		user->sendReply(RPL_WHOISUSER, targetNick);
		user->sendReply(RPL_WHOISIDLE, targetNick);
		user->sendReply(RPL_WHOISCHANNELS, targetNick);
		user->sendReply(RPL_WHOISMODES, targetNick);
	}
	if (isOperator(target)) {
		user->sendReply(RPL_WHOISOPERATOR, targetNick);
	}
	user->sendReply(RPL_ENDOFWHOIS, targetNick);
}

// void
// Server::modeCmd(User *user, vector<string> &words)
// {

// }
