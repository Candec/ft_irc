/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:16:58 by tpereira          #+#    #+#             */
/*   Updated: 2023/09/16 18:35:28 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

int main(int argc, char*argv[])
{
	if (argc != 3)
	{
		std::cout << RED << "Error" << RESET << std::endl;
		std::cout << "./ircserv <port> <password>" << std::endl;
		return (1);
	}

	if (atoi(argv[1]) <= 0 || atoi(argv[1]) > 49150)
	{
		std::cout << RED << "Error" << RESET << std::endl;
		std::cout << "Port out of range" << std::endl;
		return (1);
	}

	Server server(argv[1], argv[2]);

	while (server.run)
		server.start();
	return (0);

}

/* ************************************************************************** */
