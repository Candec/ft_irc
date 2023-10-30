/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message_parsing.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 21:15:51 by fporto            #+#    #+#             */
/*   Updated: 2023/10/30 16:42:48 by jibanez-         ###   ########.fr       */
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

	vector< vector<string> > lines = splitBuffer(buffer);

	vector< vector<string> >::iterator line;
	for (line = lines.begin(); line != lines.end(); line++)
		lookForCmd(user, cmdToEnum((*line)[0]), *line, msg);
		// lookForCmd(user, *line, msg);

	return msg;
}

vector< vector<string> >
Server::splitBuffer(const char * const buffer)
{
	vector< vector<string> >	lines;
	istringstream				lines_iss(buffer);
	string						line;

	while (getline(lines_iss, line) && !line.empty()) {
		//? Maybe ignore empty lines but keep parsing
		//? https://modern.ircdocs.horse/#compatibility-with-incorrect-software
		// if (line.empty())
		// 	continue;

		vector<string>	words;
		istringstream	words_iss(line);
		string			word;

		while (words_iss >> word)
			words.push_back(word);

		words.resize(words.size());
		lines.push_back(words);
	}
	lines.resize(lines.size());

	return lines;
}

static bool
expectedArgs(const vector<string> &args, const size_t n)
{
	if (args.size() == n)
		return(true);

	error(args[0], CONTINUE);
	if (args.size() > n)
		error("too many arguments", CONTINUE);
	else
		error("not enough arguments", CONTINUE);
	return(false);
}

bool
Server::isCmd(const string &param)
{
	if (cmdToEnum(param) != Commands::UNKNOWN)
		return true;
	return false;
}

void
Server::lookForCmd(User *user, const int cmd, vector<string> params, struct s_msg &msg)
{
	if (params.empty())
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
Server::passCmd(User *user, const vector<string> &params)
{
	if (params.size() < 1) {
		return sendMsg(user, ERR_NEEDMOREPARAMS);
	} else if (!expectedArgs(params, 1)) // There's a numeric reply for too little but not too many
		return;

	const string password = params[0];
	if (password != _password)
	{
		sendMsg(user, ERR_PASSWDMISMATCH);
		user->setStatus(UserFlags::OFFLINE);
		return;
	}

	cout << "++++++++++++++++++++" << endl;
	cout << GREEN << "Password given by " << user->getNick() << " was valid" << RESET << endl;
	user->setPassword(password);
	user->setStatus(UserFlags::ACCEPT);
}

/*
* Command: NICK
* Parameters: <nickname>
*/
void
Server::nickCmd(User *user, const vector<string> &params)
{
	if (params.empty())
		return user->sendError(ERR_NONICKNAMEGIVEN, "NICK", params);

	// Sanitize nickname
	const string nickName = params[0];
	if (
		nickName[0] == '#' \
		|| nickName[0] == ':' \
		|| nickName.find(' ') != string::npos
	)
		return user->sendError(ERR_ERRONEUSNICKNAME, "NICK", params);

	// Find duplicate
	for (map<int, User *>::const_iterator it = _users.begin(); it != _users.end(); ++it)
		if (it->second->getNick() == nickName)
			return user->sendError(ERR_NICKNAMEINUSE, "NICK", params);

	const string oldNickName = user->getNick();
	user->setNick(nickName);
	broadcast(":" + oldNickName + " NICK " + nickName);
}

/*
* Command: USER
* Parameters: <username> 0 * <realname>
*/
void
Server::userCmd(User *user, const vector<string> &params)
{
	const size_t n_args = params.size();

	if (user->isRegistered())
		return user->sendError(ERR_ALREADYREGISTERED, "USER", params);

	if (n_args < 4)
		return user->sendError(ERR_NEEDMOREPARAMS, "USER", params);

	user->setUser(params[0]);
	user->setHostname(params[1]);
	user->setServername(params[2]);

	string realName = params[3];
	for (size_t i = 4; i < n_args; ++i)
		realName += " " + params[i];
	if (realName[0] == ':')
		realName.erase(realName.begin());
	user->setName(realName);

	if (user->isRegistered())
		log(MAGENTA + user->getNick() + BLUE + " (" \
			+ MAGENTA + toString(user->getFd()) + BLUE + ") is " + GREEN + "registered");

	user->sendReply(RPL_WELCOME, "USER", "");
}

/*
* Command: JOIN
* Parameters: <channel>{,<channel>} [<key>{,<key>}]
* Alt Params: 0
*/
void
Server::joinCmd(User *user, const vector<string> &params)
{
	// This is equivalent to sending a PART command to each channel
	if (params.size() == 1 && params[0] == "0")
		user->leaveAllChannels();

	// Split vectors by , delimiter -> pair< vector<Channels> vector<Keys> >
	const string channelNames = params[0];
	const vector<string> names = splitString(channelNames, ",");

	log(string(MAGENTA + user->getNick() + BLUE + ": " \
		+ GREEN + "Joining" + BLUE + " channels " + YELLOW + channelNames));

	string channelKeys;
	if (params.size() > 1)
		channelKeys = params[1];
	const vector<string> keys = splitString(channelKeys, ",");

	vector<string>::const_iterator names_it = names.begin();
	vector<string>::const_iterator key_it = keys.begin();
	for (; names_it != names.end(); ++names_it) {
		if (channelKeys.empty())
			user->joinChannel(*names_it);
		else
		{
			user->joinChannel(*names_it, *key_it);
			++key_it;
		}
	}

	// removes the user from the previous channel list of users
	// prevChannel->removeUser(*user);
}

void
Server::colorCmd(User *user, const vector<string> &words)
{
	if (!expectedArgs(words, 2))
		return ;

	map<string, string>colors;

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
Server::quitCmd(User *user, const string &reason)
{
	// cout << BLUE << "User " << user->getNick() << RED << " disconnected" << RESET << endl;
	const vector<Channel *> channels = user->getJoinedChannels();
	for (vector<Channel *>::const_iterator it = channels.begin(); it != channels.end(); ++it)
		(*it)->broadcast(user->getNick() + ": Quit: " + reason);

	sendErrFatal(user, "");
}

// void
// Server::capCmd(User *user, const vector<string> &words)
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
Server::capCmd(User *user, const vector<string> &params)
{
	if (params.size() >= 2) {
		if (
			params[0] == "LS" \
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
Server::topicCmd(User *user, const vector<string> &params)
{
	if (params.size() == 0)
		error("No params passed to topicCmd", CONTINUE);

	const string channelName = params[0];
	Channel *channel = getChannel(channelName);

	if (params.size() == 1) {
		if (!channel->getTopic().empty())
			user->sendReply(RPL_TOPIC, "TOPIC", params);
		else
			user->sendReply(RPL_NOTOPIC, "TOPIC", params);
	}
	else {
		if (
			channel->isTopicProtected() \
			&& !channel->isOperator(user)
		)
			return user->sendError(ERR_CHANOPRIVSNEEDED, "TOPIC", params);

		const string topic = params[1];

		channel->setTopic(topic);

		string reply = "TOPIC " + channelName;
		if (!topic.empty())
			reply += " " + topic;

		channel->broadcast(reply);
	}
}

/*
* Command: PRIVMSG
* Parameters: <target>{,<target>} <text to be sent>
*/
void
Server::privmsgCmd(User *user, const vector<string> &params)
{
	const vector<string> targets = splitString(params[0], ",");

	const string msg = "PRIVMSG " + joinStrings(params);
	vector<string>::const_iterator it;
	for (it = targets.begin(); it != targets.end(); ++it) {
		const string targetName = *it;

		if (targetName[0] == '$') {
			broadcast(msg);
		}
		else if (getChannel(targetName) != NULL) {
			Channel *channel = getChannel(targetName);

			if (channel->isBanned(user))
				return;
			if (channel->noExternalMessages() && !channel->isMember(user))
				return user->sendError(ERR_CANNOTSENDTOCHAN, "PRIVMSG", channel->getName());

			channel->broadcast(msg, user, user->getNick());
		}
		else if (getUser(targetName) != NULL) {
			User *user = getUser(targetName);

			vector<string>::const_iterator it;
			for (it = targets.begin(); it != targets.end(); ++it) {
				User *target = getUser(targetName);

				if (!target->getAway().empty()) {
					vector<string> tmp;
					tmp.push_back(target->getNick());
					tmp.push_back(msg);
					user->sendReply(RPL_AWAY, "PRIVMSG", tmp);
				}
				sendMsg(target, msg, user->getNick());
			}
		}
		else {
			user->sendError(ERR_NOSUCHNICK, "PRIVMSG", targetName);
		}
	}
}

/*
* Command: AWAY
* Parameters: [<text>]
*/
void
Server::awayCmd(User *user, const vector<string> &params)
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
Server::pingCmd(User *user, const vector<string> &token)
{
	sendMsg(user, "PONG " + _serverName + " " + joinStrings(token));
}

/*
* Command: MODE
* Parameters: <target> [<modestring> [<mode arguments>...]]
*/
void
Server::modeCmd(User *user, const vector<string> &params)
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
			return user->sendError(ERR_NOSUCHNICK, "MODE", targetName);
		if (target->getNick() != targetName)
			return user->sendError(ERR_USERSDONTMATCH, "MODE", "");
		if (params.size() == 1)
			return user->sendReply(RPL_UMODEIS, "MODE", "");

		char lastFlag;
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
				target->addMode((UserFlags::Mode)*it);
			else if (lastFlag == '-')
				target->removeMode((UserFlags::Mode)*it);
		}
	} else {
		// Channel mode
		Channel *target = getChannel(targetName);
		if (!target)
			return user->sendError(ERR_NOSUCHCHANNEL, "MODE", targetName);
		if (params.size() == 1)
			return user->sendReply(RPL_CHANNELMODEIS, "MODE", params);
		if (!target->isOperator(user))
			return user->sendError(ERR_CHANOPRIVSNEEDED, "MODE", params);

		char lastFlag;
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
				target->addMode((ChannelFlags::Mode)*it, modeArguments, user);
			else if (lastFlag == '-')
				target->removeMode((ChannelFlags::Mode)*it, modeArguments, user);
		}
		target->broadcast("MODE " + joinStrings(params));
	}
}

/*
* Command: KICK
* Parameters: [<channel>] <nicks> [<reason>]
*/
// void
// Server::kickCmd(User *user, const vector<string> &params)
// {

// }

/*
* Command: INVITE
* Parameters: <nick> [<channel>]
*/
void
Server::inviteCmd(User *user, const vector<string> &params)
{
	Channel *invChannel;

	// Parameters number check
	if (params.size() < 1)
		return sendMsg(user, ERR_NEEDMOREPARAMS);
	else if (params.size() > 2)
		return sendMsg(user, "Too many arguments");
	
	// Getting the channel
	if (params.size() == 1)
		invChannel = user->getChannel();
	else if (isValidChannelName(params[1]))
		invChannel = server->getChannel(params[1]);
	else
		return sendMsg(user, CH_NAMING_ERR);

	// Check if user is operator of the channel
	if (!invChannel->isOperator(user))
		return (sendMsg(user, ERR_CHANOPRIVSNEEDED));

	invChannel->addInvitedUser(user);
}

