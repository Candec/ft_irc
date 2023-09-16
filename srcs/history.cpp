/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:16:51 by tpereira          #+#    #+#             */
/*   Updated: 2023/09/16 17:44:42 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

History::History()
{
	update();
}

void History::update()
{
	clear();

	for (std::map<unsigned char, std::string>::iterator i = history.begin(); i != history.end(); i++)
		std::cout << i->second << "\033[0m" << std::endl;
}

void History::clear()
{
	std::cout << "\033[2J" << std::flush;
}

void History::set(unsigned char i, std::string line)
{
	if (history[i] == line)
		return;
	history[i] = line;
	update();
}

void History::remove(unsigned char i)
{
	history.erase(i);
	update();
}

/* ************************************************************************** */
