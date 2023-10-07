/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:22 by jibanez-          #+#    #+#             */
/*   Updated: 2023/10/07 16:03:16 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "main.hpp"

enum { VERIFY, ACCEPT, ONLINE, OFFLINE };

class Server;
class Channel;

class User
{
	private:
		string		_name;
		string		_password;
		string		_email;
		string		_phone;
		string		_address;
		string		_city;
		string		_postalCode;
		string		_country;
		string		_id;
		string		_role;
		string		_status;
		string		_createdAt;
		string		_updatedAt;

		int fd;
		int status;
		time_t previousPing;
		string hostaddr;
		string hostname;
		string nick;
		string user;
		string name;
		string role;
		string color;
		string previousNick;
		string atChannel;
		Channel *channel;
		Server *server;

		uint16_t hostport;

		vector<string> waitToSend;

	public:
		User(int fd, struct sockaddr_in addr, Server *server);
		~User();

		string buffer;
		void sendPrivateMessage(User &To, string Message);
		void write(string Message);
		void push();

		// Setters
		void setPassword(string passwd);

		void setStatus(int status);
		void setPreviousPing(time_t ping);
		void setNick(string nick);
		void setUser(string user);
		void setName(string name);
		void setRole(string role);
		void setColor(string color);
		void setPreviousNick(string previousNick);
		void setAtChannel(string atChannel);
		void setChannel(Channel *);

		// Getters
		int getFd();
		int getStatus();
		time_t getPreviousPing();
		string getHostaddr();
		string getHostname();
		string getHost();
		string getNick();
		string getUser();
		string getName();
		string getRole();
		string getColor();
		string getPreviousNick();
		const string getAtChannel();
		Channel *getChannel();


		uint16_t getPort() const;

};

#endif
