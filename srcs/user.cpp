#include "user.hpp"

User::User(int fd, struct sockaddr_in addr, Server *server) : fd(fd), status(VERIFY), previousPing(time(0)), role("user"), server(server)
{
	//Shouldn't be required in linux. It is to block simultanious accesses to the fd
	fcntl(fd, F_SETFL, O_NONBLOCK);

	char _hostname[NI_MAXHOST];

	if (getnameinfo((struct sockaddr *)&addr, sizeof(addr), _hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) != 0)
		error("Get Name Info", EXIT);

	hostname = _hostname;
	hostaddr = inet_ntoa(addr.sin_addr);

	setNick("Annon-" + to_string(fd));
	setAtChannel("general");
	setChannel(server->getChannel("general"));
}

User::~User() { close(fd); }

//Setters
void User::setPassword(string passwd) { _password = passwd; }

void User::setStatus(int _status) { status = _status; }
void User::setPreviousPing(time_t ping) { previousPing = ping; }
void User::setNick(string _nick) { nick = _nick; }
void User::setUser(string _user) { user = _user; }
void User::setName(string _name) { name = _name; }
void User::setRole(string _role) { role = _role; }
void User::setColor(string _color) { color = _color; }
void User::setPreviousNick(string _previousNick) { previousNick = _previousNick; }
void User::setAtChannel(string _atChannel)
{
	if (_atChannel == atChannel)
		return ;

	atChannel = _atChannel;
}
void User::setChannel(Channel *_channel) { channel = _channel; }



// Getters
int User::getFd() { return (fd); }
int User::getStatus() { return (status); }
time_t User::getPreviousPing() { return (previousPing); }
string User::getHostaddr() { return (hostaddr); }
string User::getHostname() { return (hostname); }
string User::getHost() { return (hostname.size() ? hostname : hostaddr); }
string User::getNick()
{
	if (!color.empty())
		return (color + nick + RESET);
	return (nick);
}
string User::getUser() { return (user); }
string User::getName() { return (name); }
string User::getRole() { return (role); }
string User::getColor() { return (color); }
string User::getPreviousNick() { return (previousNick); }
const string User::getAtChannel() { return (atChannel); }
Channel * User::getChannel() { return (channel); }


uint16_t User::getPort() const { return ntohs(hostport); }

// Functions
void User::sendPrivateMessage(User &To, string Message) { To.write(":" + Message); }
void User::write(string Message) { waitToSend.push_back(Message); }

void User::push()
{
	if (!waitToSend.size())
		return ;

	string buffer;
	for (vector<string>::iterator i = waitToSend.begin(); i != waitToSend.end(); ++i)
		buffer += *i + MESSAGE_END;
	waitToSend.clear();

	if (buffer.length())
		return ;

	if (send(fd, buffer.c_str(), buffer.length(), 0) == -1)
		error("send", CONTINUE);
}
