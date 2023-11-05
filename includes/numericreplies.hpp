/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numericreplies.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 19:20:00 by fporto            #+#    #+#             */
/*   Updated: 2023/11/05 08:07:56 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICREPLIES_HPP
# define NUMERICREPLIES_HPP

# include <string>
# include <vector>

# include "user.hpp"
# include "main.hpp"

const std::string rpl_welcome(const User *dest);
const std::string rpl_issupport(const User *dest);
const std::string rpl_umodeis(const std::string &nick);
const std::string rpl_away(const User *dest, const std::string &targetNick);
const std::string rpl_whoisuser(const User *dest, const std::string &targetNick);
const std::string rpl_whoisoperator(const User *dest, const std::string &targetNick);
const std::string rpl_endofwho(const User *dest, const std::string &mask);
const std::string rpl_whoisidle(const User *dest, const std::string &targetNick);
const std::string rpl_endofwhois(const User *dest, const std::string &targetNick);
const std::string rpl_whoischannels(const User *dest, const std::string &targetNick);
const std::string rpl_list(const User *dest);
// const std::string rpl_channelmodeis(const User *dest, const std::vector<std::string> &params);
const std::string rpl_channelmodeis(const User *dest, const std::string &channelName);
const std::string rpl_notopic(const User *dest, const std::string &channelName);
const std::string rpl_topic(const User *dest, const std::string &channelName);
const std::string rpl_topicwhotime(const User *dest, const std::string &channelName);
const std::string rpl_inviting(const User *dest, const std::string &invitedNick, const std::string &channelName);
const std::string rpl_whoreply(const User *dest, const std::vector<std::string> &params);
const std::string rpl_namreply(const User *dest, const std::string &channelName);
const std::string rpl_endofnames(const User *dest, const std::string &channelName);
const std::string rpl_whoismodes(const User *dest, const std::string &targetNick);

const std::string err_nosuchnick(const User *dest, const std::string &nick);
const std::string err_nosuchserver(const User *dest, const std::string &serverName);
const std::string err_nosuchchannel(const User *dest, const std::string &channelName);
const std::string err_cannotsendtochan(const User *dest, const std::string &channelName);
const std::string err_norecipient(const User *dest, const std::string &cmd);
const std::string err_notexttosend(const User *dest);
const std::string err_inputtoolong(const User *dest);
const std::string err_toomanychannels(const User *dest, const std::string &attemptedChannel);
const std::string err_nonicknamegiven(const User *dest);
const std::string err_erroneusnickname(const User *dest, const std::string &nick);
const std::string err_nicknameinuser(const User *dest, const std::string &nick);
const std::string err_notonchannel(const User *dest, const std::string &channelName);
const std::string err_useronchannel(const User *dest, const std::string &targetNick, const std::string &channelName);
const std::string err_needmoreparams(const User *dest, const std::string &command);
const std::string err_alreadyregistered(const User *dest);
const std::string err_passwdmismatch(const User *dest);
const std::string err_channelisfull(const User *dest, const std::string &channelName);
const std::string err_badchannelkey(const User *dest, const std::string &channelName);
const std::string err_badchanmask(const std::string &channelName);
const std::string err_chanoprivsneeded(const User *dest, const std::string &channelName);
const std::string err_umodeunknownflag(const User *dest);
const std::string err_usersdontmatch(const User *dest);
const std::string err_invalidkey(const User *dest, const std::string &targetChannel);
const std::string err_inviteonlychan(const User *dest, const std::string &targetChannel);

#endif
