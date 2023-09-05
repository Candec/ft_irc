#include "server.hpp"

Server::server() : upTime(std::time(0))
{
	display.set(0, "Welcome to the FT_IRC server");
}

Server::server(std::string _port, std::string _password) : upTime(std::time(0)), port(_port), password(_password)
{
	display.set(0, "Welcome to the FT_IRC server");
}

Server::~server()
{
	std::vector<User *> users = getUsers;
	for (std::vector<User *>::iterator i = users.begin(); i = users.end; i++)
		delUser(*(*i));
}

Server::setup()
{
	//AF_INT: ip_v4 | SOCK_STREAM: TCP
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == 0)
		error("socket", true);

	//Blocks the use of the Address and the Port at close time to avoid package mix
	int optname = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSERADDR | SO_RESUSEPORT, &optname, sizeof(optname)))
		error("setsockopt", true);

	//Shouldn't be required in linux. It is to block simultanious accesses to the fd
	if (fcntl(fd, F_SETFL, O_NONBLOCK))
		error("fcntl", true);

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	if (bind(fd, (struct sockaddr *)&addres, sizeof(addres)) < 0)
		error("bind", true);

	if (listen(fd, address.sin_port) < 0)
		error("listen", true);

	pollfds.push_back(pollfd());
	pollfds.back().fd = fd;
	pollfds.back().events = POLLIN;
}

Server::start()
{
	
}