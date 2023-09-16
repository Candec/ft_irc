/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:30 by tpereira          #+#    #+#             */
/*   Updated: 2023/09/16 13:20:58 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "User.hpp"

# include <string>
# include <vector>
# include <map>

class Channel
{
	public:
		Channel();
		~Channel();

		// void setName(std::string name);
		// std::string getName();

		// void setMode(std::string Mode);
		// std::string getMode();

		// void setDescription(std::string Description);
		// std::string getDescription();

		// void setkey(std::string key);
		// std::string getkey();

		// void addUser(User &user);
		// void removeUser(User &user);
		// void removeUser(const std::string &nick);
		// std::vector<User *> getUsers();

		// bool isUser(User &user);
		// bool isOnChannel(User &user);

		//void setUserMax(uint16_t max);

		// void addInvitedUser(User &user);
		// void isInvitedUser(User &user);
		// void revokeInvitation(User &user);

		//void brodcast(User &user, std::string brodcast);

	private:
		std::string name;
		std::string mode;
		std::string description;
		std::string key;

		std::map<int, User *> users;
		std::map<int, User *> user_mode;
		std::string user_max;

		std::vector<User *> invitations;
};

#endif