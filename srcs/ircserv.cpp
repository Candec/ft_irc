/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 19:40:16 by fporto            #+#    #+#             */
/*   Updated: 2023/10/30 22:13:38 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.hpp"

bool logging;
bool logToFile;
std::string startTime;
Server *server;

void handler(int) { delete server; }

int main(int argc, char *argv[])
{
	startTime = timestamp();
	logging = false;
	logToFile = false;

	if (argc < 3)
		error("./ircserv <port> <password> [--log, --logfile]", EXIT);
	else if (argc > 3) {
		for (size_t i = 3; argv[i]; ++i) {
			if (!strcmp(argv[i], "--log")) {
				logging = true;
				cout << "Logging " << GREEN << "enabled" << RESET << std::endl << std::flush;
			} else if (!strcmp(argv[i], "--logfile")) {
				logging = true;
				logToFile = true;
				cout << "Logging to file " << GREEN << "enabled" << RESET << std::endl << std::flush;
			} else {
				error("./ircserv <port> <password> [--log, --logfile]", EXIT);
			}
		}
	} else {
		cout << "Logging " << RED << "disabled" << RESET << ". To enable use --log or --logfile" << std::endl << std::flush;
	}

	signal(SIGINT, handler);

	server = new Server(argv[1], argv[2]);
	server->run();

	return (EXIT_SUCCESS);
}
