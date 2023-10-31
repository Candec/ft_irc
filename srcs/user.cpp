/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 19:40:37 by fporto            #+#    #+#             */
/*   Updated: 2023/10/30 22:17:43 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/user.hpp"

User::User(const int fd, struct sockaddr_in addr) : _fd(fd), _status(UserFlags::UNVERIFY), _previousPing(time(0)), _role("user")
{
	//Shouldn't be required in linux. It is to block simultanious accesses to the fd
	fcntl(fd, F_SETFL, O_NONBLOCK);

	char hostname[NI_MAXHOST];

	if (getnameinfo((struct sockaddr *)&addr, sizeof(addr), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) != 0)
		error("Get Name Info", EXIT);

	// _hostname = hostname;
	_hostaddr = inet_ntoa(addr.sin_addr);

	setNick("Annon-" + toString(fd));
	// setAtChannel("general");
	// setChannel(server->getChannel("general"));
	setCapable(false);
}

User::~User() { close(_fd); }



//Setters

void User::setPassword(const string &passwd) { _password = passwd; }
void User::setStatus(const int status) { _status = status; }
void User::setPreviousPing(const time_t ping) { _previousPing = ping; }
void User::setHostaddr(const string &hostaddr) { _hostaddr = hostaddr; }
void User::setHostname(const string &hostname) { _hostname = hostname; }
void User::setServername(const string &servername) { _servername = servername; }
void User::setNick(const string &nick) { _nick = nick; }
void User::setUser(const string &user) { _user = user; }
void User::setName(const string &name) { _name = name; }
void User::setRole(const string &role) { _role = role; }
void User::setColor(const string &color) { _color = color; }
void User::setPreviousNick(const string &previousNick) { _previousNick = previousNick; }
void User::setAway(const string &msg) { _awayMsg = msg; }
void User::setModes(const string &modes) { _modes = modes; }
// void User::setAtChannel(const string &atChannel)
// {
// 	if (atChannel == _atChannel)
// 		return ;

// 	_atChannel = atChannel;
// }
// void User::setChannel(Channel *channel) { _channel = channel; }
void User::setCapable(bool capable) { _capable = capable; }



// Getters

int				User::getFd() const { return (_fd); }
int				User::getStatus() const { return (_status); }
time_t			User::getPreviousPing() const { return (_previousPing); }
const string	User::getHostaddr() const { return (_hostaddr); }
const string	User::getHostname() const { return (_hostname); }
const string	User::getServername() const { return (_servername); }
const string	User::getHost() const { return (_hostname.size() ? _hostname : _hostaddr); } // Not in use
const string	User::getNick() const
{
	if (!_color.empty())
		return (_color + _nick + RESET);
	return (_nick);
}
const string	User::getUser() const { return (_user); }
const string	User::getName() const { return (_name); }
const string	User::getRole() const { return (_role); }
const string	User::getColor() const { return (_color); }
const string	User::getPreviousNick() const { return (_previousNick); }
const string	User::getAway() const { return (_awayMsg); }
const string	User::getModes() const { return (_modes); }
// const string	User::getAtChannel() const { return (_atChannel); }
Channel *		User::getChannel() const { return (_channel); }
uint16_t		User::getPort() const { return ntohs(_hostport); }
bool			User::isCapable() const { return (_capable); }

const vector<Channel *> User::getJoinedChannels() const
{
	vector<Channel *> channels;
	if (_joinedChannels.size() > 0)
		for (map<string, Channel *>::const_iterator it = _joinedChannels.begin(); it != _joinedChannels.end(); ++it)
			channels.push_back(it->second);
	return channels;
}

// Functions
void User::sendPrivateMessage(User *To, const string &Message) { To->write(":" + Message); }
void User::write(const string Message) { _waitToSend.push_back(Message); }

void User::push()
{
	if (!_waitToSend.size())
		return ;

	string buffer;
	for (vector<string>::iterator i = _waitToSend.begin(); i != _waitToSend.end(); ++i)
		buffer += *i + MESSAGE_END;
	_waitToSend.clear();

	if (buffer.length())
		return ;

	if (send(_fd, buffer.c_str(), buffer.length(), 0) == SENDING_ERROR)
		error("send", CONTINUE);
}

bool User::isRegistered() const
{
	return (!_user.empty() \
		|| !_hostname.empty() \
		|| !_servername.empty());
}

bool User::isChannelMember(const string &channelName)
{
	return (_joinedChannels.find(channelName) != _joinedChannels.end());
}

void User::joinChannel(const string &channelName)
{
	joinChannel(channelName, "");
}
void User::joinChannel(const string &channelName, const string &key)
{
	if (isChannelMember(channelName))
		return;
	// cout << "User " << MAGENTA + _nick + WHITE + " is joining the channel \"" + YELLOW + channelName + WHITE + "\"" << endl << flush;
	log(MAGENTA + _nick + BLUE + ": " + GREEN + "Joining " + YELLOW + channelName);

	Channel *channel = server->getChannel(channelName);
	if (!channel) {
		cout << RED << "    Channel doesn't exist" << WHITE << endl << flush;
		channel = server->createChannel(channelName);
		if (!channel)
			return;

		channel->addMode(ChannelFlags::OPERATOR, vector<string>(1, _nick), this);
	}
	if (channel->isFull())
		return this->sendError(ERR_CHANNELISFULL, "JOIN", channelName);
	if (channel->getKey() != key)
		return this->sendError(ERR_BADCHANNELKEY, "JOIN", channelName);
	if (channel->getMode() == "i" && !channel->isInvitedUser(this))
		return this->sendError(ERR_INVITEONLYCHAN, "JOIN", channelName);

	channel->addUser(this);
	_joinedChannels.insert(pair<string, Channel *>(channelName, channel));

	if (!_capable)
		channel->setLog(MAGENTA + _nick + " joined the channel" + RESET);
	else
		channel->setLog(_nick + " joined the channel");

	cout << GREEN << " OK" << WHITE << endl << flush;

	string reply;
	// Send JOIN ACK
	reply = ":" + _nick + " JOIN " + channelName;
	server->sendMsg(this, reply);
	// Channel topic
	if (!channel->getTopic().empty())
		this->sendReply(RPL_TOPIC, "JOIN", "");
	// List of channel members
	this->sendReply(RPL_NAMREPLY, "JOIN", channelName);
	this->sendReply(RPL_ENDOFNAMES, "JOIN", "");
}
void User::leaveChannel(Channel *channel)
{
	if (!channel)
		return;
	if (!channel->isMember(this))
		return log("User was not member of " + channel->getName());

	channel->removeUser(this);
	_joinedChannels.erase(channel->getName());

	log(MAGENTA + _nick + RED + " left " + YELLOW + channel->getName() + WHITE);

	if (channel->getUsers().size() == 0)
		server->delChannel(channel);
}
void User::leaveAllChannels()
{
	for (map<string, Channel *>::iterator it = _joinedChannels.begin(); it != _joinedChannels.end(); ++it)
		leaveChannel(it->second);
}



bool User::isModeImplemented(UserFlags::Mode modeLetter) const
{
	switch (modeLetter)
	{
	case UserFlags::INVISIBLE:
	case UserFlags::OPERATOR:
		return true;
	default:
		return false;
	}
}
void User::addMode(UserFlags::Mode modeLetter)
{
	if (!isModeImplemented(modeLetter))
		return this->sendError(ERR_UMODEUNKNOWNFLAG, "MODE", "");

	const char mode = modeLetter;

	log(MAGENTA + _name + BLUE + ": " + GREEN + "Adding" + BLUE + " mode " + mode);

	if (_modes.find(mode) == string::npos)
		_modes += mode;
}
void User::removeMode(UserFlags::Mode modeLetter)
{
	if (!isModeImplemented(modeLetter))
		return this->sendError(ERR_UMODEUNKNOWNFLAG, "MODE", "");

	const char mode = modeLetter;

	log(MAGENTA + _name + BLUE + ": " + RED + "Removing" + BLUE + " mode " + mode);

	size_t pos = _modes.find(mode);
	if (pos != string::npos)
		_modes.erase(pos);
}
bool User::isInvisible() const
{
	return (_modes.find('i') != string::npos);
}

void User::sendReply(Replies type, const std::string &cmd, const std::string &param)
{
	sendReply(type, cmd, std::vector<string>(1, param));
}
void User::sendReply(Replies type, const std::string &cmd, const std::vector<std::string> &params)
{
	sendReply(type, "", "", cmd, params);
}
void User::sendReply(Replies type, const std::string &tags, const std::string &src, const std::string &cmd, const std::string &param)
{
	sendReply(type, tags, src, cmd, std::vector<string>(1, param));
}
void User::sendReply(Replies type, const std::string &tags, const std::string &src, const std::string &cmd, const std::vector<std::string> &params)
{
	std::string reply;

	if (!tags.empty())
		reply += "@" + tags + " ";

	if (!src.empty())
		reply += ":" + src + " ";

	// if (!cmd.empty())
	// 	return;

	reply += codeToString(type) + " ";
	switch (type)
	{
	case RPL_WELCOME:
		reply += rpl_welcome(this);
		break;
	case RPL_ISUPPORT:
		reply += rpl_issupport(this);
		break;
	case RPL_UMODEIS:
		reply += rpl_umodeis(this);
		break;
	case RPL_AWAY:
		reply += rpl_away(this, params[0], params[1]);
		break;
	case RPL_LIST:
		reply += rpl_list(this);
		break;
	case RPL_CHANNELMODEIS:
		reply += rpl_channelmodeis(this, params);
		break;
	case RPL_NOTOPIC:
		reply += rpl_notopic(this, params[0]);
		break;
	case RPL_TOPIC:
		reply += rpl_topic(this, params[0]);
		break;
	case RPL_NAMREPLY:
		reply += rpl_namreply(this, params[0]);
		break;
	case RPL_ENDOFNAMES:
		reply += rpl_endofnames(this, params[0]);
		break;
	default:
		error("Missing reply for numeric " + toString(type), CONTINUE);
	}
	(void)cmd;
	// return reply;
	server->sendMsg(this, reply);
}


void User::sendError(Errors type, const std::string &cmd, const std::string &param) const
{
	sendError(type, cmd, std::vector<string>(1, param));
}
void User::sendError(Errors type, const std::string &cmd, const std::vector<std::string> &params) const
{
	std::string reply;

	reply = codeToString(type) + " ";
	switch (type)
	{
	case ERR_NOSUCHNICK:
		reply += err_nosuchnick(this, params[0]);
		break;
	case ERR_NOSUCHSERVER:
		reply += err_nosuchserver(this, params[0]);
		break;
	case ERR_NOSUCHCHANNEL:
		reply += err_nosuchchannel(this, params[0]);
		break;
	case ERR_CANNOTSENDTOCHAN:
		reply += err_cannotsendtochan(this, params[0]);
		break;
	case ERR_TOOMANYCHANNELS:
		reply += err_toomanychannels(this, params[0]);
		break;
	case ERR_INPUTTOOLONG:
		reply += err_inputtoolong(this);
		break;
	case ERR_NONICKNAMEGIVEN:
		reply += err_nonicknamegiven(this);
		break;
	case ERR_ERRONEUSNICKNAME:
		reply += err_erroneusnickname(this, params[0]);
		break;
	case ERR_NICKNAMEINUSE:
		reply += err_nicknameinuser(this, params[0]);
		break;
	case ERR_NEEDMOREPARAMS:
		reply += err_needmoreparams(this, cmd);
		break;
	case ERR_ALREADYREGISTERED:
		reply += err_alreadyregistered(this);
		break;
	case ERR_PASSWDMISMATCH:
		reply += err_passwdmismatch(this);
		break;
	case ERR_CHANNELISFULL:
		reply += err_channelisfull(this, params[0]);
		break;
	case ERR_BADCHANNELKEY:
		reply += err_badchannelkey(this, params[0]);
		break;
	case ERR_BADCHANMASK:
		reply += err_badchanmask(params[0]);
		break;
	case ERR_CHANOPRIVSNEEDED:
		reply += err_chanoprivsneeded(this, params[0]);
		break;
	case ERR_UMODEUNKNOWNFLAG:
		reply += err_umodeunknownflag(this);
		break;
	case ERR_USERSDONTMATCH:
		reply += err_usersdontmatch(this);
		break;
	case ERR_INVALIDKEY:
		reply += err_invalidkey(this, params[0]);
		break;
	case ERR_INVITEONLYCHAN:
		reply += err_inviteonlychan(this, params[0]);
		break;
	default:
		error("Missing error for numeric " + toString(type), CONTINUE);
		return;
	}
	// (void)tags;
	// (void)src;
	server->sendMsg(this, reply);
}
