#include "server.hpp"
#include "utility.hpp"

int main(int argc, char*argv[])
{
	if (argc != 3)
		error("./ircserv <port> <password>", EXIT);

	Server server(argv[1], argv[2]);

	server.setup();
	while (server.run)
		server.start();
	return (0);
}