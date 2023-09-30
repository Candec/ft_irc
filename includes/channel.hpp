/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:30 by tpereira          #+#    #+#             */
/*   Updated: 2023/09/30 13:07:12 by jibanez-         ###   ########.fr       */
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
		Channel(std::string name);

		~Channel();

		void setName(string name);
		string getName();

		void setMode(string mode);
		string getMode();

		void setUserMode(User &user, string mode);
		string getUserMode(User &user);

		void setMaxUsers(string users_max);
		string getMaxUsers();

		void setDescription(string Description);
		string getDescription();

		void setPass(string key);
		string getPass();

		void addUser(User &user);
		void removeUser(User &user);
		void removeUser(const string &nick);
		vector<User *> getUsers();

		bool isUser(User &user);
		bool isOnChannel(string const &nick);

		void addInvitedUser(User &user);
		bool isInvitedUser(User &user);
		void revokeInvitation(User &user);

		void broadcast(User &user, string message);


	private:
		string _name;
		string _mode;
		string _description;
		string _pass;

		map<int, string> _user_mode;
		map<int, User *> _users;
		string _users_max;

		vector<User *> _invitations;

		History _history;
};

#endif
