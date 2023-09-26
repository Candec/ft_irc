#include "main.hpp"

Server *server;

void handler(int) { delete server; }

int main(int argc, char*argv[])
{
	if (argc != 3)
		error("./ircserv <port> <password>", EXIT);

	string port = argv[1];
	string password = argv[2];
	server = new Server(port, password);
	// try
	// {
	// }
	// catch(const exception& e)
	// {
	// 	cerr << e.what() << '\n';
	// }

	signal(SIGINT, handler);
	server->setup();
	while (true)
		server->run();
	return (EXIT_SUCCESS);
}
