#include "server.hpp"
#include "utility.hpp"

int main(int argc, char*argv[])
{
	if (argc != 3)
		error("./ircserv <port> <password>", EXIT);

	try
	{
		std::string port = argv[1];
		std::string password = argv[2];
		Server server(port, password);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	

	server.setup();
	while (server.run)
		server.start();
	return (0);
}
