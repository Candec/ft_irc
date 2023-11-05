/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 19:40:37 by fporto            #+#    #+#             */
/*   Updated: 2023/11/05 02:35:03 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/user.hpp"

User::User(const int fd, struct sockaddr_in addr) : _joinTime(time(NULL)), _fd(fd), _previousPing(time(0)), _role("user")
{
	std::cout << "creating user" << std::endl;
	//Shouldn't be required in linux. It is to block simultanious accesses to the fd
	fcntl(fd, F_SETFL, O_NONBLOCK);

	char hostname[NI_MAXHOST];

	if (getnameinfo((struct sockaddr *)&addr, sizeof(addr), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) != 0)
		error("Get Name Info", EXIT);

	_hostaddr = inet_ntoa(addr.sin_addr);

	if (server->getPassword().empty())
	{
		setStatus(UserFlags::ACCEPT);
		std::cout << "Status set to accept: " << getStatus() << std::endl;
	}
	else
	{
		setStatus(UserFlags::UNVERIFY);
		std::cout << "Status set to unverify: " << getStatus() << std::endl;
	}

	setNick("Annon-" + toString(fd));
	setCapable(false);
}

User::~User() { close(_fd); }



//Setters

void User::setPassword(const std::string &passwd) { _password = passwd; }
void User::updateIdleTime() { _idleSince = time(NULL); }
void User::setStatus(UserFlags::Status status) { _status = status; }
void User::setPreviousPing(const time_t ping) { _previousPing = ping; }
void User::setHostaddr(const std::string &hostaddr) { _hostaddr = hostaddr; }
void User::setHostname(const std::string &hostname) { _hostname = hostname; }
void User::setServername(const std::string &servername) { _servername = servername; }
void User::setNick(const std::string &nick) { _nick = nick; }
void User::setUser(const std::string &user) { _user = user; }
void User::setName(const std::string &name) { _name = name; }
void User::setRole(const std::string &role) { _role = role; }
void User::setColor(const std::string &color) { _color = color; }
void User::setPreviousNick(const std::string &previousNick) { _previousNick = previousNick; }
void User::setAway(const std::string &msg) { _awayMsg = msg; }
void User::setModes(const std::string &modes) { _modes = modes; }
// void User::setAtChannel(const std::string &atChannel)
// {
// 	if (atChannel == _atChannel)
// 		return ;

// 	_atChannel = atChannel;
// }
// void User::setChannel(Channel *channel) { _channel = channel; }
void User::setCapable(bool capable) { _capable = capable; }



// Getters

time_t				User::getJoinTime() const { return _joinTime; }
time_t				User::getIdleSince() const { return _idleSince; }
int					User::getFd() const { return (_fd); }
int					User::getStatus() const { return (_status); }
time_t				User::getPreviousPing() const { return (_previousPing); }
const std::string	User::getHostaddr() const { return (_hostaddr); }
const std::string	User::getHostname() const { return (_hostname); }
const std::string	User::getServername() const { return (_servername); }
const std::string	User::getHost() const { return (_hostname.size() ? _hostname : _hostaddr); } // Not in use
const std::string	User::getNick() const
{
	if (!_color.empty())
		return (_color + _nick + RESET);
	return (_nick);
}
const std::string	User::getUser() const { return (_user); }
const std::string	User::getName() const { return (_name); }
const std::string	User::getRole() const { return (_role); }
const std::string	User::getColor() const { return (_color); }
const std::string	User::getPreviousNick() const { return (_previousNick); }
const std::string	User::getAwayMsg() const { return (_awayMsg); }
const std::string	User::getModes() const { return (_modes); }
// const std::string	User::getAtChannel() const { return (_atChannel); }
Channel *			User::getChannel() const { return (_channel); }
uint16_t			User::getPort() const { return ntohs(_hostport); }
bool				User::isCapable() const { return (_capable); }

const std::vector<Channel *> User::getJoinedChannels() const
{
	std::vector<Channel *> channels;
	if (_joinedChannels.size() > 0)
		for (std::map<std::string, Channel *>::const_iterator it = _joinedChannels.begin(); it != _joinedChannels.end(); ++it)
			channels.push_back(it->second);
	return channels;
}

// Functions
void User::sendPrivateMessage(User *To, const std::string &Message) { To->write(":" + Message); }
void User::write(const std::string Message) { _waitToSend.push_back(Message); }

void User::push()
{
	if (!_waitToSend.size())
		return ;

	std::string buf;
	for (std::vector<std::string>::iterator i = _waitToSend.begin(); i != _waitToSend.end(); ++i)
		buf += *i + MESSAGE_END;
	_waitToSend.clear();

	if (buf.length())
		return ;

	if (send(_fd, buf.c_str(), buf.length(), 0) == SENDING_ERROR)
		error("send", CONTINUE);
}

bool User::isRegistered() const
{
	return (!_user.empty() \
		|| !_hostname.empty() \
		|| !_servername.empty());
}

bool User::isChannelMember(const std::string &channelName) const
{
	return (_joinedChannels.find(channelName) != _joinedChannels.end());
}

void User::joinChannel(const std::string &channelName)
{
	joinChannel(channelName, "");
}
void User::joinChannel(const std::string &channelName, const std::string &key)
{
	if (isChannelMember(channelName))
		return;
	// std::cout << "User " << MAGENTA + _nick + WHITE + " is joining the channel \"" + YELLOW + channelName + WHITE + "\"" << std::endl << std::flush;
	log(MAGENTA + _nick + BLUE + ": " + GREEN + "Joining " + YELLOW + channelName);

	Channel *channel = server->getChannel(channelName);
	if (!channel) {
		std::cout << RED << "    Channel doesn't exist" << WHITE << std::endl << std::flush;
		channel = server->createChannel(channelName);
		if (!channel)
			return;

		channel->addMode(ChannelFlags::OPERATOR, std::vector<std::string>(1, _nick), this);
	}
	if (channel->isFull())
		return sendError(ERR_CHANNELISFULL, channelName);
	if (channel->getKey() != key)
		return sendError(ERR_BADCHANNELKEY, channelName);
	if (channel->isInviteOnly() && !channel->isInvitedUser(this))
		return sendError(ERR_INVITEONLYCHAN, channelName);

	channel->addUser(this);
	_joinedChannels.insert(std::pair<std::string, Channel *>(channelName, channel));

	// if (!_capable)
	// 	channel->setLog(MAGENTA + _nick + " joined the channel" + RESET);
	// else
	// 	channel->setLog(_nick + " joined the channel");

	// std::cout << GREEN << " OK" << WHITE << std::endl << std::flush;

	std::string reply;
	// Send JOIN ACK
	reply = ":" + _nick + " JOIN " + channelName;
	server->sendMsg(this, reply);
	// Channel topic
	if (!channel->getTopic().empty())
		this->sendReply(RPL_TOPIC, channelName);
	// List of channel members
	this->sendReply(RPL_NAMREPLY, channelName);
	this->sendReply(RPL_ENDOFNAMES, channelName);

	channel->broadcast(std::string("JOIN " + channelName), this, getNick());
}
void User::leaveChannel(Channel *channel)
{
	if (!channel)
		return;
	if (!channel->isMember(this))
		return log("User was not member of " + channel->getName());

	channel->broadcast(std::string("PART " + channel->getName()), NULL, _nick);

	channel->removeUser(this);
	_joinedChannels.erase(channel->getName());

	log(MAGENTA + _nick + RED + " left " + YELLOW + channel->getName() + WHITE);

	if (channel->getUsers().size() == 0)
		server->delChannel(channel);
}
void User::leaveAllChannels()
{
	for (std::map<std::string, Channel *>::iterator it = _joinedChannels.begin(); it != _joinedChannels.end(); ++it)
		leaveChannel(it->second);
}



bool User::isModeImplemented(UserFlags::ModeLetter modeLetter) const
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
void User::addMode(UserFlags::ModeLetter modeLetter)
{
	if (!isModeImplemented(modeLetter))
		return sendError(ERR_UMODEUNKNOWNFLAG);

	const char mode = modeLetter;

	log(MAGENTA + _nick + BLUE + ": " + GREEN + "Adding" + BLUE + " mode " + mode);

	if (_modes.find(mode) == std::string::npos)
		_modes += mode;
}
void User::removeMode(UserFlags::ModeLetter modeLetter)
{
	if (!isModeImplemented(modeLetter))
		return sendError(ERR_UMODEUNKNOWNFLAG);

	const char mode = modeLetter;

	log(MAGENTA + _nick + BLUE + ": " + RED + "Removing" + BLUE + " mode " + mode);

	size_t pos = _modes.find(mode);
	if (pos != std::string::npos)
		_modes.erase(pos);
}
bool User::isInvisible() const { return (_modes.find('i') != std::string::npos); }
bool User::isAway() const { return !_awayMsg.empty(); }

void User::sendReply(Replies type) const { sendReply(type, "", ""); }
void User::sendReply(Replies type, const std::string &param) const { sendReply(type, param, ""); }
void User::sendReply(Replies type, const std::vector<std::string> &params) const { sendReply(type, params, ""); }
void User::sendReply(Replies type, const std::string &param, const std::string &cmd) const { sendReply(type, std::vector<std::string>(1, param), cmd); }
void User::sendReply(Replies type, const std::vector<std::string> &params, const std::string &cmd) const { sendReply(type, params, cmd, "", ""); }
void User::sendReply(Replies type, const std::string &param, const std::string &cmd, const std::string &tags, const std::string &src) const { sendReply(type, std::vector<std::string>(1, param), cmd, tags, src); }
void User::sendReply(Replies type, const std::vector<std::string> &params, const std::string &cmd, const std::string &tags, const std::string &src) const
{
	std::string reply;

	if (!tags.empty())
		reply += "@" + tags + " ";

	if (!src.empty())
		reply += ":" + src + " ";

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
		reply += rpl_umodeis(params[0]);
		break;
	case RPL_AWAY:
		reply += rpl_away(this, params[0]);
		break;
	case RPL_WHOISUSER:
		reply += rpl_whoisuser(this, params[0]);
		break;
	case RPL_WHOISOPERATOR:
		reply += rpl_whoisoperator(this, params[0]);
		break;
	case RPL_ENDOFWHO:
		reply += rpl_endofwho(this, params[0]);
		break;
	case RPL_WHOISIDLE:
		reply += rpl_whoisidle(this, params[0]);
		break;
	case RPL_ENDOFWHOIS:
		reply += rpl_endofwhois(this, params[0]);
		break;
	case RPL_WHOISCHANNELS:
		reply += rpl_whoischannels(this, params[0]);
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
	case RPL_TOPICWHOTIME:
		reply += rpl_topicwhotime(this, params[0]);
		break;
	case RPL_INVITING:
		reply += rpl_inviting(this, params[0], params[1]);
		break;
	case RPL_WHOREPLY:
		reply += rpl_whoreply(this, params);
		break;
	case RPL_NAMREPLY:
		reply += rpl_namreply(this, params[0]);
		break;
	case RPL_ENDOFNAMES:
		reply += rpl_endofnames(this, params[0]);
		break;
	case RPL_WHOISMODES:
		reply += rpl_whoismodes(this, params[0]);
		break;
	default:
		return error("Missing reply for numeric " + toString(type), CONTINUE);
	}
	(void)cmd;
	server->sendMsg(this, reply);
}


void User::sendError(Errors type) const { sendError(type, "", ""); }
void User::sendError(Errors type, const std::string &param) const { sendError(type, param, ""); }
void User::sendError(Errors type, const std::string &param, const std::string &cmd) const { sendError(type, std::vector<std::string>(1, param), cmd); }
void User::sendError(Errors type, const std::vector<std::string> &params) const { sendError(type, params, ""); }
void User::sendError(Errors type, const std::vector<std::string> &params, const std::string &cmd) const
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
	case ERR_NOTONCHANNEL:
		reply += err_notonchannel(this, params[0]);
		break;
	case ERR_USERONCHANNEL:
		reply += err_useronchannel(this, params[0], params[1]);
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
		return error("Missing error for numeric " + toString(type), CONTINUE);
	}
	// (void)tags;
	// (void)src;
	server->sendMsg(this, reply);
}
