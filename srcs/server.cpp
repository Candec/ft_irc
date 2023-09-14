#include "server.hpp"

/*
	CONSTRUCTORS
*/
Server::Server() : upTime(std::time(0)), run(OFF)
{
	history.set(0, "Welcome to the FT_IRC server");

	std::ifstream file("./Configuration/irc.config", std::ios::in);
	std::string line;

	while (std::getline(file, line))
	{
		if (line.substr(0, line.find("=")) == "default_port")
			port = atoi(line.substr(1, line.find("=")).c_str());

		if (line.substr(0, line.find("=")) == "default_pass")
			password = line.substr(1, line.find("="));

		if (line.substr(0, line.find("=")) == "max_ping")
			ping = atoi(line.substr(1, line.find("=")).c_str());

		if (line.substr(0, line.find("=")) == "timeout")
			timeout = atoi(line.substr(1, line.find("=")).c_str());
	}
}

Server::Server(char *_port, char * _password) : upTime(std::time(0)), run(OFF)
{
	Server::setPort(_port);
	Server::setPassword(_password);
	history.set(0, "Welcome to the FT_IRC server");

	std::ifstream file("./Configuration/irc.config");
	std::string line;

	while (std::getline(file, line))
	{
		if (line.substr(0, line.find("=")) == "max_ping")
			ping = atoi(line.substr(1, line.find("=")).c_str());

		if (line.substr(0, line.find("=")) == "timeout")
			timeout = atoi(line.substr(1, line.find("=")).c_str());
	}
}

Server::~Server()
{
	std::vector<User *> users = getUsers();
	for (std::vector<User *>::iterator i = users.begin(); i != users.end(); ++i)
		delUser(*(*i));
}

/*
	SETTERS
*/
void Server::setPort(char *_port)
{
	port.assign(atoi(_port), sizeof(atoi(_port)));
}

void Server::setPassword(char *_password)
{
	password.assign(_password, sizeof(_password));
}

/*
	GETTERS
*/
std::vector<User *> Server::getUsers()
{
	std::vector<User *> usersV = std::vector<User *>();
	for (std::map<int, User *>::iterator i = users.begin(); i != users.end(); ++i)
		usersV.push_back(i->second);

	return (usersV);
}

/*
	DELETERS
*/
void Server::delUser(User &user)
{
	(void)user;
}

void Server::delChannel(Channel &channel)
{
	(void)channel;
}

void Server::updatePing()
{
	time_t now = std::time(0);

	for (std::map<int, User *>::iterator i = users.begin(); i != users.end(); i++)
	{
		if (now - (*i).second->getPreviousPing() >= timeout)
			(*i).second->setStatus(OFFLINE);
		else if ((*i).second->getStatus() == ONLINE)
			(*i).second->write("PING " + (*i).second->getNick());
	}
}

void Server::setup()
{
	if (port.empty())
		error("port", EXIT);

	//AF_INT: ip_v4 | SOCK_STREAM: TCP
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == 0)
		error("socket", EXIT);

	//Blocks the use of the Address and the Port at close time to avoid package mix
	int optname = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optname, sizeof(optname)))
		error("setsockopt", EXIT);

	//Shouldn't be required in linux. It is to block simultanious accesses to the fd
	if (fcntl(fd, F_SETFL, O_NONBLOCK))
		error("fcntl", EXIT);

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(atoi(port.c_str()));

	if (bind(fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		error("bind", EXIT);

	if (listen(fd, address.sin_port) < 0)
		error("listen", EXIT);

	pollfds.push_back(pollfd());
	pollfds.back().fd = fd;
	pollfds.back().events = POLLIN;

	run = ON;
}

void Server::start()
{
	if (poll(&pollfds[0], pollfds.size(), (ping * 1000) / 10) == -1)
		return ;

	if (std::time(0) - previous_ping >= ping)
	{
		updatePing();
		return ;
	}
}