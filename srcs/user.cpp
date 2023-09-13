#include "user.hpp"

User::User(int fd, struct sockaddr_in addr) :
	fd(fd),
	status(VERIFY),
	previousPing(std::time(0)),
	hostaddr(addr),
	hostname(),
	nick(),
	user(),
	name(),
	privilages(W),
	previousNick(),
	previousChannel()
{
	//Shouldn't be required in linux. It is to block simultanious accesses to the fd
	fcntl(fd, F_SETFL, O_NONBLOCK);

	hostaddr = inet_ntoa(addr.sin_addr);
	char _hostname[NI_MAXHOST];

	if (getnameinfo((struct sockaddr *)&addr, sizeof(addr), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) != 0)
		error("Get Name Info", EXIT);

	hostname = _hostname;
}

User::~User();

void User::sendPrivateMessage(User &To, std::string Message);

// Setters
void User::setStatus(int status);
void User::setPing(time_t ping);
void User::setNick(std::string nick);
void User::setUser(std::string user);
void User::setName(std::string name);
void User::setPrivilage(std::string privilages);
void User::setPreviousNick(std::string previousNick);
void User::setPreviousChannel(std::string previousChannel);

// Getters
int User::getFd();
int User::getStatus();
time_t User::getPreviousPing();
std::string User::getHostaddr();
std::string User::getHostname();
std::string User::getNick();
std::string User::getUser();
std::string User::getName();
std::string User::getPrivilages();
std::string User::getPreviousNick();
std::string User::getPreviousChannel();