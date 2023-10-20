/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numericreplies.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 19:20:00 by fporto            #+#    #+#             */
/*   Updated: 2023/10/12 16:21:26 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICREPLIES_HPP
# define NUMERICREPLIES_HPP

# include <string>
# include <vector>

# include "User.hpp"
// # include "main.hpp"

enum Replies {
	RPL_WELCOME = 001,
	RPL_ISUPPORT = 005,
	RPL_LIST = 322,
	RPL_TOPIC = 332,
	RPL_NAMREPLY = 353,
	RPL_ENDOFNAMES = 366
};

const std::string getReply(Replies type, User *dest, const std::string &tags, const std::string &src, const std::string &cmd, const vector<std::string> &params);

const std::string rpl_welcome(User *dest);
const std::string rpl_issupport(User *dest);
const std::string rpl_list(User *dest);
const std::string rpl_topic(User *dest);
const std::string rpl_namreply(User *dest);
const std::string rpl_endofnames(User *dest, const std::string &channelName);

enum Errors {
	ERR_NOSUCHCHANNEL = 403,
	ERR_INPUTTOOLONG = 417,
	ERR_TOOMANYCHANNELS = 405,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_PASSWDMISMATCH = 464,
	ERR_BADCHANNELKEY = 475,
	ERR_INVALIDKEY = 525
};

void sendError(Errors type, User *dest, const std::string &tags, const std::string &src, const std::string &cmd, const vector<std::string> &params);

const std::string err_nosuchchannel(User *dest, const std::string &channelName);
const std::string err_inputtoolong(User *dest);
const std::string err_toomanychannels(User *dest);
const std::string err_nonicknamegiven(User *dest);
const std::string err_erroneusnickname(User *dest, const std::string &nick);
const std::string err_nicknameinuser(User *dest, const std::string &nick);
const std::string err_needmoreparams(User *dest, const std::string &command);
const std::string err_alreadyregistered(User *dest);
const std::string err_passwdmismatch(User *dest);
const std::string err_badchannelkey(User *dest);
const std::string err_invalidkey(User *dest, const std::string targetChannel);

#endif
