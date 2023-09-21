/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:22 by tpereira          #+#    #+#             */
/*   Updated: 2023/09/21 17:26:12 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "main.hpp"

enum { VERIFY, ACCEPT, ONLINE, OFFLINE };

class User
{
	private:
		std::string		_name;
		std::string		_password;
		std::string		_email;
		std::string		_phone;
		std::string		_address;
		std::string		_city;
		std::string		_postalCode;
		std::string		_country;
		std::string		_id;
		std::string		_role;
		std::string		_status;
		std::string		_createdAt;
		std::string		_updatedAt;
		
		int fd;
		int status;
		time_t previousPing;
		std::string hostaddr;
		std::string hostname;
		std::string nick;
		std::string user;
		std::string name;
		std::string role;
		std::string previousNick;
		std::string previousChannel;

		std::vector<std::string> waitToSend;

	public:
		User(int fd, struct sockaddr_in addr);
		~User();

		std::string buffer;
		void sendPrivateMessage(User &To, std::string Message);
		void write(std::string Message);
		void push();

 		// Setters
		void setStatus(int status);
		void setPreviousPing(time_t ping);
		void setNick(std::string nick);
		void setUser(std::string user);
		void setName(std::string name);
		void setRole(std::string role);
		void setPreviousNick(std::string previousNick);
		void setPreviousChannel(std::string previousChannel);

		// Getters
		int getFd();
		int getStatus();
		time_t getPreviousPing();
		std::string getHostaddr();
		std::string getHostname();
		std::string getNick();
		std::string getUser();
		std::string getName();
		std::string getRole();
		std::string getPreviousNick();
		std::string getPreviousChannel();

};

#endif
