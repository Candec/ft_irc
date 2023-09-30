/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message_parsing.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 21:15:51 by fporto            #+#    #+#             */
/*   Updated: 2023/09/25 23:10:04 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

using namespace std;

struct s_msg
Server::parseMessage(User *user, const char* const buffer) const
{
	struct s_msg msg;
	msg.src = user;
	msg.timestamp = time(NULL);

	strncpy(msg.buffer, buffer, BUFFER + 1);

	const vector< vector<string> >	lines = splitBuffer(buffer);

	vector< vector<string> >::const_iterator line;
	for (line = lines.begin(); line != lines.end(); line++)
		this->parseLine(user, msg, *line);

	return msg;
}

const vector< vector<string> >
Server::splitBuffer(const char* const buffer) const
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

void
Server::parseLine(User *user, struct s_msg& msg, const vector<string>& words) const
{
	const string& cmd = words[0];

	if (words.size() > 1) {
		const string& arg = words[1];

		if (!arg.empty()) {
			if (!cmd.compare("PASS")) {
				// msg.password = arg;
				user->setPassword(arg);
			} else if (!cmd.compare("NICK")) {
				// msg.nick = arg;
				user->setNick(arg);
			} else if (!cmd.compare("USER")) {
				// msg.user = arg;
				user->setUser(arg);
			}
		}
	} else if (!cmd.compare("QUIT")) {
		// msg.command = "QUIT";
		msg.command = cmd;
	}
}
