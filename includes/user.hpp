/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:22 by jibanez-          #+#    #+#             */
/*   Updated: 2023/10/23 16:38:44 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "main.hpp"
using std::map;
using std::string;
using std::vector;

class Server;
class Channel;

enum UserStatus {
	VERIFY,
	ACCEPT,
	ONLINE,
	OFFLINE
};

enum UserModes {
	OPERATOR = 'o'
};

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
		string	_atChannel;
		Channel	*_channel;
		Server	*_server;

		uint16_t _hostport; // Not set

		bool _capable;

		vector<string> _waitToSend;

	public:
		User(const int fd, struct sockaddr_in addr, Server *server);
		~User();

		string buffer;
		void sendPrivateMessage(User *To, const string Message);
		void write(const string Message);
		void push();
		bool isRegistered() const;
		bool isOperator();

		// Setters
		void setPassword(const string passwd);
		void setStatus(const int status);
		void setPreviousPing(const time_t ping);
		void setHostaddr(const string hostaddr);
		void setHostname(const string hostname);
		void setServername(const string servername);
		void setNick(const string nick);
		void setUser(const string user);
		void setName(const string name);
		void setRole(const string role);
		void setColor(const string color);
		void setPreviousNick(const string previousNick);
		void setAtChannel(const string atChannel);
		void setChannel(Channel *);
		void setCapable(bool capable);

		// Getters
		int getFd() const;
		int getStatus() const;
		time_t getPreviousPing() const;
		const string getHostaddr() const;
		const string getHostname() const;
		const string getServername() const;
		const string getHost() const;
		const string getNick() const;
		const string getUser() const;
		const string getName() const;
		const string getRole() const;
		const string getColor() const;
		const string getPreviousNick() const;
		const string getAtChannel() const;
		Channel *getChannel() const;
		uint16_t getPort() const;
		bool getCapable() const;
};

#endif
