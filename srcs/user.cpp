#include "user.hpp"

User::User(int fd, struct sockaddr_in addr) : fd(fd), status(VERIFY), previousPing(std::time(0)), role("user")
{
	//Shouldn't be required in linux. It is to block simultanious accesses to the fd
	fcntl(fd, F_SETFL, O_NONBLOCK);

	char _hostname[NI_MAXHOST];
	
	if (getnameinfo((struct sockaddr *)&addr, sizeof(addr), _hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) != 0)
		error("Get Name Info", EXIT);

	hostname = _hostname;
	hostaddr = inet_ntoa(addr.sin_addr);

}

User::~User()
{
	close(fd);
}

// // Setters
void User::setStatus(int _status) {	status = _status; }
// void User::setPing(time_t ping);
// void User::setNick(std::string nick);
// void User::setUser(std::string user);
// void User::setName(std::string name);
// void User::setRole(std::string role);
// void User::setPreviousNick(std::string previousNick);
// void User::setPreviousChannel(std::string previousChannel);

// // Getters
int User::getFd() {return (fd); }
int User::getStatus() { return (status); }
time_t User::getPreviousPing() { return (previousPing); }
std::string User::getHostaddr() { return (hostaddr); }
std::string User::getHostname() { return (hostname); }
std::string User::getNick() { return (nick); }
std::string User::getUser() {return (user); }
std::string User::getName() {return (name); }
std::string User::getRole() {return (role); }
std::string User::getPreviousNick() { return (previousNick); }
std::string User::getPreviousChannel() {return (previousChannel); }

// void User::sendPrivateMessage(User &To, std::string Message);
void User::write(std::string Message) { waitToSend.push_back(Message); }
