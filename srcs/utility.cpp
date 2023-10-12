/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:06 by tpereira          #+#    #+#             */
/*   Updated: 2023/10/12 10:35:56 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utility.hpp"

void error(const string str, bool quit)
{
	cout << RED_BG << "Error: " << RED << str << RESET << endl;
	if (quit)
		exit(EXIT_FAILURE);
}

const string toString(const int n)
{
	ostringstream str;
	str << n;
	return (str.str());
}

const string timestamp()
{
	const time_t	now = time(NULL);
	const struct tm	t = *localtime(&now);
	ostringstream str;

	str << "[" << t.tm_hour << ":" << t.tm_min << ":" << t.tm_sec << "] ";

	return (str.str());
}

bool isValidChannelName(const string& name)
{
	/*
	** Name max length is 200 chars
	** Name must begin with '&' or '#'
	** '#' -> distributed channel known to all servers on the network
	** '&' -> channel only available to clients connected to it's server
	** Can't contain ' ', ascii 7 or ','
	*/
	if (name.size() > 200) {
		error("Channel name too long", CONTINUE);
		return false;
	}
	if (name[0] != '&' && name[0] != '#') {
		error("Missing \"" + name + "\" &/# channel identifier", CONTINUE);
		return false;
	}
	if (name.find(' ') != string::npos \
	|| name.find(7) != string::npos \
	|| name.find(',') != string::npos) {
		error("Invalid character in name", CONTINUE);
		return false;
	}
	return true;
}
