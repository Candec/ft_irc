/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message_parsing.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 21:15:51 by fporto            #+#    #+#             */
/*   Updated: 2023/10/07 18:15:43 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

struct s_msg
Server::parseMessage(User *user, char* const buffer)
{
	struct s_msg msg;
	msg.src = user;
	msg.timestamp = time(NULL);

	strncpy(msg.buffer, buffer, BUFFER + 1);

	vector< vector<string> > lines = splitBuffer(buffer);

	vector< vector<string> >::iterator line;
	for (line = lines.begin(); line != lines.end(); line++)
		this->lookForCmd(user, msg, *line);

	return msg;
}

vector< vector<string> >
Server::splitBuffer(char* buffer)
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

template <typename T> bool expectedArgs(T args, size_t n)
{
	if (args.size() == n)
		return(true);

	if (args.size() > n)
		error("too many arguments", CONTINUE);
	else
		error("not enough arguments", CONTINUE);
	return(false);
}

void
Server::lookForCmd(User *user, struct s_msg& msg, vector<string> words)
{
	if (words.empty())
		return ;

	string cmd = words[0];
	// const string& arg = words[1];

	if (!cmd.compare("PASS"))
		passCmd(user, words);
		// msg.password = arg;

	if (!cmd.compare("NICK"))
		nickCmd(user, words);
		// msg.nick = arg;

	if (!cmd.compare("USER"))
		userCmd(user, words);
		// msg.user = arg;

	if (!cmd.compare("COLOR"))
		colorCmd(user, words);

	if (!cmd.compare("JOIN"))
		joinCmd(user,words);

	if (!cmd.compare("QUIT"))
		msg.command = cmd;
		// msg.cmd = "QUIT";
}


/*
** LIST OF COMMANDS
*/

void Server::passCmd(User *user, vector<string> words)
{
	if (expectedArgs(words, 2))
		user->setPassword(words[1]);
}

void Server::nickCmd(User *user, vector<string> words)
{
	if (expectedArgs(words, 2))
		user->setNick(words[1]);
}

void Server::userCmd(User *user, vector<string> words)
{
	if (expectedArgs(words, 2))
		user->setUser(words[1]);
}

void Server::joinCmd(User *user, vector<string> words)
{
	if (!expectedArgs(words, 2))
		return ;

	if (user->getAtChannel() == words[1])
		return ;

	if (!isChannel(words[1]))
		setChannel(words[1]);

	//leaving channel msg
	Channel *prevChannel = getChannel(user->getAtChannel());
	prevChannel->setHistory(user->getNick() + " left the channel");

	//joining channel msg
	user->setAtChannel(words[1]);
	Channel *channel = getChannel(words[1]);
	user->setChannel(channel);
	channel->setHistory(user->getNick() + " joined the channel");
}

void Server::colorCmd(User *user, vector<string> words)
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
		sendError(user->getFd(), "Color not found\nTry: red, black, green, yellow, blue, pink cyan, white, none");
	user->setColor(colors[words[1]]);
}
