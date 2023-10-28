/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:06 by tpereira          #+#    #+#             */
/*   Updated: 2023/10/28 18:19:43 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utility.hpp"

void error(const std::string &str, const bool quit)
{
	std::cout << RED_BG << "Error: " << RED << str << RESET << std::endl;
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
	|| name.find(',') != std::string::npos) {
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
			cout << "pushing_back: " << str.substr(0, pos) << endl << flush;
			ret.push_back(str.substr(0, pos));
			str.erase(0, pos + delimiter.length());
		}
		if (ret.empty())
			ret = std::vector<std::string>(1, str);
	}
	return ret;
}

void log(const std::string &info)
{
	std::ofstream logFile;
	const std::string fileName = "log.txt";
	logFile.open(fileName.c_str());
	logFile << timestamp() <<  info << std::endl << std::flush;
	logFile.close();
	std::cout << BLUE + info + RESET << std::endl << std::flush;
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
