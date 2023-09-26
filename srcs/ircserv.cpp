#include "main.hpp"

Server *server;

void handler(int) { delete server; }

int main(int argc, char*argv[])
{
	if (argc != 3)
		error("./ircserv <port> <password>", EXIT);

	std::string port = argv[1];
	std::string password = argv[2];
	server = new Server(port, password);
	// try
	// {
	// }
	// catch(const std::exception& e)
	// {
	// 	std::cerr << e.what() << '\n';
	// }

	signal(SIGINT, handler);
	server->setup();
	while (true)
		server->run();
	return (EXIT_SUCCESS);
}
