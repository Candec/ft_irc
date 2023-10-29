/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 19:40:16 by fporto            #+#    #+#             */
/*   Updated: 2023/10/29 15:32:39 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.hpp"

Server *server;

void handler(int) { delete server; }

int main(int argc, char *argv[])
{
	if (argc != 3)
		error("./ircserv <port> <password>", EXIT);

	signal(SIGINT, handler);

	server = new Server(argv[1], argv[2]);
	server->run();

	return (EXIT_SUCCESS);
}
