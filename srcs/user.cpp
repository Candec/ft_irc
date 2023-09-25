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

User::~User() { close(fd); }

//Setters
void User::setStatus(int _status) {	status = _status; }
void User::setPreviousPing(time_t ping) { previousPing = ping; }
void User::setNick(std::string _nick) { nick = _nick; }
void User::setUser(std::string _user) { user = _user; }
void User::setName(std::string _name) { name = _name; }
void User::setRole(std::string _role) { role = _role ; }
void User::setPreviousNick(std::string _previousNick) { previousNick = _previousNick; }
void User::setPreviousChannel(std::string _previousChannel) { previousChannel = _previousChannel; }

// Getters
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

// Functions
void User::sendPrivateMessage(User &To, std::string Message) { To.write(":" + Message); }
void User::write(std::string Message) { waitToSend.push_back(Message); }

void User::push()
{
	if (!waitToSend.size())
		return ;

	std::string buffer;
	for (std::vector<std::string>::iterator i = waitToSend.begin(); i != waitToSend.end(); ++i)
		buffer += *i + MESSAGE_END;
	waitToSend.clear();

	if (buffer.length())
		return ;

	if (send(fd, buffer.c_str(), buffer.length(), 0) == -1)
		error("send", false);
}
