#include "main.hpp"

bool run = true;
void handler(int) { run = false; }

int main(int argc, char*argv[])
{
	if (argc != 3)
		error("./ircserv <port> <password>", EXIT);


	std::string port = argv[1];
	std::string password = argv[2];
	Server server(port, password);
	// try
	// {
	// }
	// catch(const std::exception& e)
	// {
	// 	std::cerr << e.what() << '\n';
	// }
	

	server.setup();
	signal(SIGINT, handler);
	while (run)
		server.run();
	return (0);
}
