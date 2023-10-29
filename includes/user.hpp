/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:22 by jibanez-          #+#    #+#             */
/*   Updated: 2023/10/29 15:50:13 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "numericreplies.hpp"
# include "main.hpp"

class Server;
class Channel;

enum Replies {
	RPL_WELCOME = 001,
	RPL_ISUPPORT = 005,
	RPL_UMODEIS = 221,
	RPL_AWAY = 301,
	RPL_LIST = 322,
	RPL_CHANNELMODEIS = 324,
	RPL_NOTOPIC = 331,
	RPL_TOPIC = 332,
	RPL_NAMREPLY = 353,
	RPL_ENDOFNAMES = 366
};

enum Errors {
	ERR_NOSUCHNICK = 401,
	ERR_NOSUCHSERVER = 402,
	ERR_NOSUCHCHANNEL = 403,
	ERR_CANNOTSENDTOCHAN = 404,
	ERR_TOOMANYCHANNELS = 405,
	// ERR_NOORIGIN = 409,
	// ERR_NORECIPIENT = 411,
	ERR_NOTEXTTOSEND = 412,
	ERR_INPUTTOOLONG = 417,
	// ERR_UNKNOWNCOMMAND = 421,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_PASSWDMISMATCH = 464,
	ERR_CHANNELISFULL = 471,
	ERR_BADCHANNELKEY = 475,
	ERR_BADCHANMASK = 476,
	ERR_CHANOPRIVSNEEDED = 482,
	ERR_UMODEUNKNOWNFLAG = 501,
	ERR_USERSDONTMATCH = 502,
	ERR_INVALIDKEY = 525
	// ERR_INVALIDMODEPARAM = 696
};

namespace UserFlags {

enum Status {
	UNVERIFY,
	ACCEPT,
	ONLINE,
	OFFLINE
};

// When making changes, certify that they're reflected in isModeImplemented()
enum Mode {
	INVISIBLE = 'i',
	OPERATOR = 'o'
};

}

class User
{
	private:
		// string	_name;
		string	_password;
		string	_email;
		string	_phone;
		string	_address;
		string	_city;
		string	_postalCode;
		string	_country;
		string	_id;
		// string	_role;
		// string	_status;
		string	_createdAt;
		string	_updatedAt;

		int		_fd;
		int		_status;
		time_t	_previousPing;
		string	_hostaddr;
		string	_hostname;
		string	_servername;
		string	_nick;
		string	_user;
		string	_name;
		string	_role;
		string	_color;
		string	_previousNick;
		string	_awayMsg;
		string	_modes;
		// string	_atChannel;
		Channel	*_channel;
		map<string, Channel *> _joinedChannels;

		uint16_t _hostport; // Not set

		bool _capable;

		vector<string> _waitToSend;

	public:
		User(const int fd, struct sockaddr_in addr);
		~User();

		string buffer;
		void sendPrivateMessage(User *To, const string &Message);
		void write(const string Message);
		void push();
		bool isRegistered() const;

		// Setters
		void setPassword(const string &passwd);
		void setStatus(const int status);
		void setPreviousPing(const time_t ping);
		void setHostaddr(const string &hostaddr);
		void setHostname(const string &hostname);
		void setServername(const string &servername);
		void setNick(const string &nick);
		void setUser(const string &user);
		void setName(const string &name);
		void setRole(const string &role);
		void setColor(const string &color);
		void setPreviousNick(const string &previousNick);
		void setAway(const string &msg);
		void setModes(const string &modes);
		void setAtChannel(const string &atChannel);
		// void setChannel(Channel *channel);
		void setCapable(bool capable);

		// Getters
		int				getFd() const;
		int				getStatus() const;
		time_t			getPreviousPing() const;
		const string	getHostaddr() const;
		const string	getHostname() const;
		const string	getServername() const;
		const string	getHost() const;
		const string	getNick() const;
		const string	getUser() const;
		const string	getName() const;
		const string	getRole() const;
		const string	getColor() const;
		const string	getPreviousNick() const;
		const string	getAway() const;
		const string	getModes() const;
		const string	getAtChannel() const;
		Channel			*getChannel() const;
		uint16_t		getPort() const;
		bool			isCapable() const;

		const vector<Channel *> getJoinedChannels() const;

		bool			isChannelMember(const string &channelName);
		void			joinChannel(const string &channelName);
		void			joinChannel(const string &channelName, const string &key);
		// void			leaveChannel(const string &channelName);
		void			leaveChannel(Channel *channel);
		void			leaveAllChannels();

		bool			isModeImplemented(UserFlags::Mode mode) const;
		void			addMode(UserFlags::Mode modeLetter);
		void			removeMode(UserFlags::Mode modeLetter);
		bool			isInvisible() const;

		void			sendReply(Replies type, const std::string &cmd, const std::string &param);
		void			sendReply(Replies type, const std::string &cmd, const std::vector<std::string> &params);
		void			sendReply(Replies type, const std::string &tags, const std::string &src, const std::string &cmd, const std::string &param);
		void			sendReply(Replies type, const std::string &tags, const std::string &src, const std::string &cmd, const std::vector<std::string> &params);

		// void sendError(Errors type, const User *dest, const std::string &tags, const std::string &src, const std::string &cmd, const std::vector<std::string> &params);
		void			sendError(Errors type, const std::string &cmd, const std::string &param) const ;
		void			sendError(Errors type, const std::string &cmd, const std::vector<std::string> &params) const ;
};

#endif
