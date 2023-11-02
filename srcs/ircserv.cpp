/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 19:40:16 by fporto            #+#    #+#             */
/*   Updated: 2023/11/02 14:42:37 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.hpp"

Server *server;
bool logging = false;
bool logToFile = false;
std::string password;
std::string startTime = timestamp();

void handler(int) { delete server; }

void enableLogsAndPassword(int argc, char *argv[])
{
	int count = 2;
	int i = 2;
	std::string log("--log");
	std::string logfile("--logfile");
	
	if (strncmp(argv[2], "--", 2)) {
		password = argv[2];
		count += 1;
	}
		std::cout << "Password: [" << password << "]" << std::endl;
	
	while (argv[i]) 
	{
		if (!log.compare(argv[i]) && logging == false) {
			logging = true;
			count += 1;
			std::cout << "Logging " << GREEN << "enabled" << RESET << std::endl << std::flush;
		} else if (!logfile.compare(argv[i]) && logToFile == false && logging == false) {
			logging = true;
			logToFile = true;
			count += 1;
			std::cout << "Logging to file " << GREEN << "enabled" << RESET << std::endl << std::flush;
		}
		i++;
	}

	if (count != argc)
		error("./ircserv <port> [password] [--log / --logfile]", EXIT);
}

int main(int argc, char *argv[])
{

	signal(SIGINT, handler);

	if (argc <= 1)
		error("./ircserv <port> [password] [--log / --logfile]", EXIT);
	else if (argc == 2) {
		server = new Server(argv[1], password);
	} else if (argc == 3) {
		enableLogsAndPassword(argc, argv);
		server = new Server(argv[1], password);
	} else if (argc == 4) {
		enableLogsAndPassword(argc, argv);
		server = new Server(argv[1], password);
	} else
		error("./ircserv <port> [password] [--log / --logfile]", EXIT);

	if (!logging)
		std::cout << "Logging " << RED << "disabled" << RESET << ". To enable use --log or --logfile" << std::endl << std::flush;

	server->run();

	return (EXIT_SUCCESS);
}
