/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:22 by jibanez-          #+#    #+#             */
/*   Updated: 2023/11/09 08:09:08 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "numericreplies.hpp"
# include "main.hpp"
using std::map;
using std::string;
using std::vector;

class Server;
class Channel;

enum Replies {
	RPL_WELCOME = 001,
	RPL_ISUPPORT = 005,
	RPL_UMODEIS = 221,
	RPL_AWAY = 301,
	RPL_WHOISUSER = 311,
	RPL_WHOISOPERATOR = 313,
	RPL_ENDOFWHO = 315,
	RPL_WHOISIDLE = 317,
	RPL_ENDOFWHOIS = 318,
	RPL_WHOISCHANNELS = 319,
	RPL_LIST = 322,
	RPL_CHANNELMODEIS = 324,
	RPL_NOTOPIC = 331,
	RPL_TOPIC = 332,
	RPL_TOPICWHOTIME = 333,
	RPL_INVITING = 341,
	RPL_WHOREPLY = 352,
	RPL_NAMREPLY = 353,
	RPL_ENDOFNAMES = 366,
	RPL_WHOISMODES = 379
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
	ERR_NOTONCHANNEL = 442,
	ERR_USERONCHANNEL = 443,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_PASSWDMISMATCH = 464,
	ERR_CHANNELISFULL = 471,
	ERR_INVITEONLYCHAN = 473,
	ERR_BADCHANNELKEY = 475,
	ERR_BADCHANMASK = 476,
	ERR_CHANOPRIVSNEEDED = 482,
	ERR_UMODEUNKNOWNFLAG = 501,
	ERR_USERSDONTMATCH = 502,
	ERR_INVALIDKEY = 525,
	ERR_INVALIDMODEPARAM = 696
};

namespace UserFlags {
	enum Status {
		UNVERIFY,
		ACCEPT,
		ONLINE,
		OFFLINE,
		WAITING // Received incomplete message
	};

	// When making changes, certify that they're reflected in isModeImplemented()
	enum ModeLetter {
		INVISIBLE = 'i',
		OPERATOR = 'o'
	};
}

class User
{
	private:
		// string	_name;
		std::string	_password;
		std::string	_email;
		std::string	_phone;
		std::string	_address;
		std::string	_city;
		std::string	_postalCode;
		std::string	_country;
		std::string	_id;
		// string	_role;
		// string	_status;
		std::string	_createdAt;
		std::string	_updatedAt;

		const time_t	_joinTime;

		time_t		_idleSince;

		const int	_fd;
		int			_status;
		time_t		_previousPing;
		std::string	_hostaddr;
		std::string	_hostname;
		std::string	_servername;
		std::string	_nick;
		std::string	_user;
		std::string	_name;
		std::string	_role;
		std::string	_color;
		std::string	_previousNick;
		std::string	_awayMsg;
		std::string	_modes;
		// string	_atChannel;
		Channel		*_channel;
		std::map<std::string, Channel *> _joinedChannels;

		uint16_t	_hostport; // Not set

		bool		_capable;

		std::vector<std::string> _waitToSend;

	public:
		User(const int fd, struct sockaddr_in addr);
		~User();

		std::string recv_buffer;
		std::string send_buffer;
		void sendPrivateMessage(User *To, const std::string &Message);
		void write(const std::string Message);
		void push();
		bool isRegistered() const;
		bool isOperator();

		// Setters
		void setPassword(const std::string &passwd);
		void updateIdleTime();
		void setStatus(UserFlags::Status status);
		void setPreviousPing(const time_t ping);
		void setHostaddr(const std::string &hostaddr);
		void setHostname(const std::string &hostname);
		void setServername(const std::string &servername);
		void setNick(const std::string &nick);
		void setUser(const std::string &user);
		void setName(const std::string &name);
		void setRole(const std::string &role);
		void setColor(const std::string &color);
		void setPreviousNick(const std::string &previousNick);
		void setAway(const std::string &msg);
		void setModes(const std::string &modes);
		// void setChannel(Channel *channel);
		void setCapable(bool capable);

		// Getters
		time_t				getJoinTime() const;
		time_t				getIdleSince() const;
		int					getFd() const;
		int					getStatus() const;
		time_t				getPreviousPing() const;
		const std::string	getHostaddr() const;
		const std::string	getHostname() const;
		const std::string	getServername() const;
		const std::string	getHost() const;
		const std::string	getNick() const;
		const std::string	getUser() const;
		const std::string	getName() const;
		const std::string	getRole() const;
		const std::string	getColor() const;
		const std::string	getPreviousNick() const;
		const std::string	getAwayMsg() const;
		const std::string	getModes() const;
		uint16_t			getPort() const;
		bool				isCapable() const;

		const std::vector<Channel *> getJoinedChannels() const;

		bool			isChannelMember(const std::string &channelName) const;
		void			joinChannel(const std::string &channelName, const std::string &key);
		// void			leaveChannel(const std::string &channelName);
		void			leaveChannel(Channel *channel);
		void			leaveChannel(Channel *channel, const std::string reason);
		void			leaveAllChannels();

		bool			isModeImplemented(UserFlags::ModeLetter mode) const;
		void			addMode(UserFlags::ModeLetter modeLetter);
		void			removeMode(UserFlags::ModeLetter modeLetter);
		bool			isInvisible() const;
		bool			isAway() const;

		void			sendReply(Replies type);
		void			sendReply(Replies type, const std::string &param);
		void			sendReply(Replies type, const std::vector<std::string> &params);
		void			sendReply(Replies type, const std::string &param, const std::string &cmd);
		void			sendReply(Replies type, const std::vector<std::string> &params, const std::string &cmd);
		void			sendReply(Replies type, const std::string &param, const std::string &cmd, const std::string &tags, const std::string &src);
		void			sendReply(Replies type, const std::vector<std::string> &params, const std::string &cmd, const std::string &tags, const std::string &src);

		// void sendError(Errors type, const User *dest, const std::string &tags, const std::string &src, const std::string &cmd, const std::vector<std::string> &params);
		void			sendError(Errors type);
		void			sendError(Errors type, const std::string &param);
		void			sendError(Errors type, const std::string &param, const std::string &cmd);
		void			sendError(Errors type, const std::vector<std::string> &params);
		void			sendError(Errors type, const std::vector<std::string> &params, const std::string &cmd);
};

#endif
