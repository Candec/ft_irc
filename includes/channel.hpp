/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:30 by tpereira          #+#    #+#             */
/*   Updated: 2023/09/30 12:34:18 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "main.hpp"

class User;

class Channel
{
	public:
		Channel();
		~Channel();

		void setName(std::string name);
		std::string getName();

		void setMode(std::string mode);
		std::string getMode();

		void setUserMode(User &user, std::string mode);
		std::string getUserMode(User &user);

		void setMaxUsers(std::string users_max);
		std::string getMaxUsers();

		void setDescription(std::string Description);
		std::string getDescription();

		void setPass(std::string key);
		std::string getPass();

		void addUser(User &user);
		void removeUser(User &user);
		void removeUser(const std::string &nick);
		std::vector<User *> getUsers();

		bool isUser(User &user);
		bool isOnChannel(std::string const &nick);

		void addInvitedUser(User &user);
		bool isInvitedUser(User &user);
		void revokeInvitation(User &user);

		void broadcast(User &user, std::string message);


	private:
		std::string _name;
		std::string _mode;
		std::string _description;
		std::string _pass;

		std::map<int, std::string> _user_mode;
		std::map<int, User *> _users;
		std::string _users_max;

		std::vector<User *> _invitations;
};

#endif
