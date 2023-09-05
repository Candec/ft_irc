#include "Server.hpp"

int main(int argc, char*argv[])
{
	if (argc != 3)
	{
		std::cout << "Error" << std::endl;
		std::cout << "./ircserv <port> <password>" << std::endl;
		return (1);
	}

	if (atoi(port) <= 0 || atoi(port) > 49150)
	{
		std::cout << "Error" << std::endl;
		std::cout << "Port out of range" << std::endl;
		return (1);
	}

	Server server(argv[1], argv[2]);

	while (server.run)
		server.start();
	return (0);

}