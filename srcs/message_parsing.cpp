/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message_parsing.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 21:15:51 by fporto            #+#    #+#             */
/*   Updated: 2023/10/12 15:45:53 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

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
		lookForCmd(user, *line, msg);

	return msg;
}

vector< vector<string> >
Server::splitBuffer(const char * const buffer)
{
	vector< vector<string> >	lines;
	istringstream				lines_iss(buffer);
	string						line;

	while (getline(lines_iss, line) && !line.empty()) {
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

// template <typename T>
// bool expectedArgs(T args, size_t n)
// {
// 	if (args.size() == n)
// 		return(true);

// 	error(args, CONTINUE);
// 	if (args.size() > n)
// 		error("too many arguments", CONTINUE);
// 	else
// 		error("not enough arguments", CONTINUE);
// 	return(false);
// }

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
Server::isCmd(const string &word)
{
	for (size_t i = 0; !Commands[i].empty(); ++i) {
		if (word == Commands[i])
			return true;
	}
	return false;
}

void
Server::lookForCmd(User *user, vector<string> words, struct s_msg &msg)
{
	if (words.empty() || !isCmd(words[0]))
		return;

	const string cmd = words[0];
	msg.command = true;

	if (!cmd.compare("PASS")) {
		passCmd(user, words);

	} else if (!cmd.compare("NICK")) {
		nickCmd(user, words);

	} else if (!cmd.compare("USER")) {
		userCmd(user, words);

	} else if (!cmd.compare("COLOR")) {
		colorCmd(user, words);

	} else if (!cmd.compare("JOIN")) {
		joinCmd(user, words);

	} else if (!cmd.compare("QUIT")) {
		quitCmd(user);

	} else if (!cmd.compare("CAP")) {
		capCmd(user, words);

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
Server::passCmd(User *user, const vector<string> words)
{
	if (words.size() < 2) {
		sendMsg(user->getFd(), ERR_NEEDMOREPARAMS);
		return;
	} else if (!expectedArgs(words, 2)) // There's a numeric reply for roo little but not too many
		return;

	if (words[1] != _password) {
		sendMsg(user->getFd(), ERR_PASSWDMISMATCH);
		return;
	}

	user->setPassword(words[1]);
	user->setStatus(ACCEPT);
}

/*
* Command: NICK
* Parameters: <nickname>
*/
void
Server::nickCmd(User *user, const vector<string> &words)
{
	if (words.size() != 2) {
		sendMsg(user->getFd(), ERR_NONICKNAMEGIVEN);
		return;
	}

	// Sanitize nickname
	const string nickname = words[1];
	if (nickname[0] == '#' \
	|| nickname[0] == ':' \
	|| hasSpace(nickname)) {
		sendMsg(user->getFd(), ERR_ERRONEUSNICKNAME);
		return;
	}

	// Find duplicate
	for (map<int, User *>::const_iterator it = _users.begin(); it != _users.end(); ++it)
		if (it->second->getNick() == nickname) {
			sendMsg(user->getFd(), ERR_NICKNAMEINUSE);
			return;
		}

	user->setNick(nickname);
}

/*
* Command: USER
* Parameters: <username> 0 * <realname>
*/
void
Server::userCmd(User *user, const vector<string> &words)
{
	const size_t n_args = words.size();

	if (user->isRegistered()) {
		sendMsg(user->getFd(), ERR_ALREADYREGISTERED);
		return;
	}

	if (n_args < 5) {
		sendMsg(user->getFd(), ERR_NEEDMOREPARAMS);
		return;
	}

	user->setUser(words[1]);
	user->setHostname(words[2]);
	user->setServername(words[3]);

	string word = words[4];
	for (size_t i = 5; i < n_args; ++i)
		word += " " + words[i];
	if (word[0] == ':')
		word.erase(word.begin());
	user->setName(word);
}

/*
* Command: JOIN
* Parameters: <channel>{,<channel>} [<key>{,<key>}]
* Alt Params: 0
*/
void
Server::joinCmd(User *user, vector<string> words)
{
	// cout << "User " << user->getNick() << " is going to join the channel" << endl << flush;
	if (!expectedArgs(words, 2))
		return ;

	// cout << "args are fine" << endl << flush;
	if (user->getAtChannel() == words[1])
		return ;

	// cout << "user wasn't already in channel" << endl << flush;
	if (!isChannel(words[1]))
		createChannel(words[1]);

	// cout << "back in the joinCmd" << endl << flush;
	//leaving channel msg
	Channel *prevChannel = getChannel(user->getAtChannel());
	if (prevChannel)
		prevChannel->setLog(user->getNick() + " left " + prevChannel->getName());

	//joining channel msg
	// cout << "user fd: " << user->getFd() << endl << flush;
	sendClear(user->getFd());
	user->setAtChannel(words[1]);
	Channel *channel = getChannel(words[1]);
	user->setChannel(channel);
	channel->addUser(user);

	// Send reply
	string reply;
	// JOIN ACK
	reply = ":" + user->getNick() + " JOIN " + channel->getName();
	sendMsg(user->getFd(), reply);
	// Channel topic
	if (!channel->getTopic().empty()) {
		reply = user->getNick() + " " + channel->getName() + " :" + channel->getTopic();
		sendMsg(user->getFd(), reply);
	}
	// List of users in the channel
	reply = user->getNick() + " " + channel->getStatus() + " " + channel->getName() + " :";
	const vector<User *> users = channel->getUsers();
	for (vector<User *>::const_iterator i = users.begin(); i != users.end(); ++i) {
		if (channel->getUserMode(*i) == "o")
			reply += '@';
		reply += (*i)->getNick();
		if (i + 1 != users.end())
			reply += ' ';
		sendMsg(user->getFd(), reply);
	}
	reply = user->getNick() + " " + channel->getName() + " :End of /NAMES list";
	sendMsg(user->getFd(), reply);

	string tmp;
	if (!user->getCapable())
		tmp = BLACK + user->getNick() + " joined the channel" + RESET;
	else
		tmp = user->getNick() + " joined the channel";
	channel->setLog(tmp);

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
Server::quitCmd(User *user)
{
	cout << BLUE << "User " << user->getNick();
	cout << RED << " disconnected" << RESET << endl;
	delUser(user);
}

void
Server::capCmd(User *user, vector<string> &words)
{
	if (!words[1].compare("LS")
	&& !words[2].compare("302"))
		user->setCapable(true);
}

// Checks for ASCII space withing given string
bool
Server::hasSpace(const string &str) const
{
	return (str.find(' ') != string::npos);
}
