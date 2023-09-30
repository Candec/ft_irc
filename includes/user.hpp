/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:22 by tpereira          #+#    #+#             */
/*   Updated: 2023/09/25 21:33:13 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "main.hpp"

enum { VERIFY, ACCEPT, ONLINE, OFFLINE };

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
		string previousNick;
		string previousChannel;

		uint16_t hostport;

		vector<string> waitToSend;

	public:
		User(int fd, struct sockaddr_in addr);
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
		void setPreviousNick(string previousNick);
		void setPreviousChannel(string previousChannel);

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
		string getPreviousNick();
		string getPreviousChannel();

		uint16_t getPort() const;

};

#endif
