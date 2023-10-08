/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:06 by tpereira          #+#    #+#             */
/*   Updated: 2023/10/08 12:33:18 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utility.hpp"

void error(std::string str, bool quit)
{
	std::cout << RED << "Error: " << WHITE << str <<std::endl;
	if (quit)
		exit(EXIT_FAILURE);
}

string to_string(int n)
{
	ostringstream str;
	str << n;
	string ret = str.str();
	return (ret);
}

string timestamp()
{
	time_t		now = time(NULL);
	struct tm	t = *localtime(&now);
	ostringstream str;

	str << "[" << t.tm_hour << ":" << t.tm_min << ":" << t.tm_sec << "] ";
	string ret = str.str();
	return (ret);
}

/* ************************************************************************** */
