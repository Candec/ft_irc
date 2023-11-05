/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:06 by tpereira          #+#    #+#             */
/*   Updated: 2023/11/05 03:46:25 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utility.hpp"

void error(const std::string &str, const bool quit)
{
	log("Error: " + str, false);
	std::cout << RED_BG << "Error: " << RESET << RED << str << RESET << std::endl;
	if (quit)
		exit(EXIT_FAILURE);
}

// template<typename T>
// const std::string toString(const T n)
// {
// 	ostringstream str;
// 	str << n;
// 	return (str.str());
// }
const std::string toString(const time_t n)
{
	std::ostringstream str;
	str << n;
	return (str.str());
}
const std::string toString(const int n)
{
	std::ostringstream str;
	str << n;
	return (str.str());
}
const std::string toString(const size_t n)
{
	std::ostringstream str;
	str << n;
	return (str.str());
}

// Checks for ASCII space withing given string
bool
hasSpace(const string &str)
{
	return (str.find(' ') != string::npos);
}

const std::string codeToString(const int code)
{
	std::string ret;
	if (code < 100)
		ret += '0';
	if (code < 10)
		ret += '0';
	ret += toString(code);
	return ret;
}

const std::string timestamp()
{
	const time_t	now = time(NULL);
	const struct tm	t = *localtime(&now);
	std::ostringstream str;

	str << "[";
	if (t.tm_hour < 10)
		str << "0";
	str << t.tm_hour << ":";
	if (t.tm_min < 10)
		str << "0";
	str << t.tm_min << ":";
	if (t.tm_sec < 10)
		str << "0";
	str << t.tm_sec << "] ";

	return (str.str());
}

bool isValidChannelName(const std::string& name)
{
	/*
	** Name max length is 200 chars
	** Name must begin with '&' or '#'
	** '#' -> distributed channel known to all servers on the network
	** '&' -> channel only available to clients connected to it's server
	** Can't contain ' ', ascii 7 or ','
	*/
	if (name.size() > CHANNEL_NAME_MAX_LEN) {
		error("Channel name too long", CONTINUE);
		return false;
	}
	if (name[0] != '&' && name[0] != '#') {
		error("Missing \"" + name + "\"'s &/# channel identifier", CONTINUE);
		return false;
	}
	if (name.find(' ') != std::string::npos \
		|| name.find(7) != std::string::npos \
		|| name.find(',') != std::string::npos
	) {
		error("Invalid character in name", CONTINUE);
		return false;
	}
	return true;
}

const std::vector<std::string> splitString(std::string str, const std::string &delimiter)
{
	std::vector<std::string> ret;

	if (!str.empty()) {
		// size_t n_channels = 0;
		size_t pos = 0;
		while ((pos = str.find(delimiter)) != std::string::npos) {
			// ret[n_channels++] = str.substr(0, pos);
			std::cout << "pushing_back: " << str.substr(0, pos) << std::endl << std::flush;
			ret.push_back(str.substr(0, pos));
			str.erase(0, pos + delimiter.length());
		}
		if (ret.empty())
			ret = std::vector<std::string>(1, str);
	}
	return ret;
}

void log(const std::string &info, const bool outputToTerminal)
{
	if ((info.find("PING") != std::string::npos || info.find("PONG") != std::string::npos) \
		&& !LOG_PINGS)
		return;
	if (logging) {
		if (logToFile) {
			std::ofstream logFile;
			const std::string fileName = startTime + "log.txt";
			logFile.open(fileName.c_str(), std::ios_base::app);
			logFile << timestamp() <<  removeColor(info) << std::endl << std::flush;
			logFile.close();
		}
		if (outputToTerminal)
			std::cout << BLUE << info << RESET << std::endl << std::flush;
	}
}
void log(const std::string &info)
{
	log(info, true);
}

const std::string joinStrings(const std::vector<std::string> &strings)
{
	std::string ret;
	std::vector<std::string>::const_iterator it;
	for (it = strings.begin(); it != strings.end(); ++it) {
		ret += *it;
		if (it + 1 != strings.end())
			ret += " ";
	}
	return ret;
}

const std::string joinStringsButFirst(const std::vector<std::string> &strings)
{
	std::string ret;
	std::vector<std::string>::const_iterator it;
	for (it = ++strings.begin(); it != strings.end(); ++it) {
		ret += *it;
		if (it + 1 != strings.end())
			ret += " ";
	}
	return ret;
}

const std::string removeColor(std::string str)
{
	size_t pos;

	while ((pos = str.find('\033')) != std::string::npos) {
		size_t lastPos = str.find('m', pos);
		if (lastPos != std::string::npos)
			str.erase(pos, lastPos - pos + 1);
	}
	return str;
}
