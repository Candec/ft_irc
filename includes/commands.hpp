/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 11:26:25 by fporto            #+#    #+#             */
/*   Updated: 2023/10/31 09:55:32 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef COMMANDS_HPP
# define COMMANDS_HPP

// # include "main.hpp"
# include <string>
# include "utility.hpp"

namespace Commands {

enum {
	UNKNOWN = -1,
	PASS,
	NICK,
	USER,
	COLOR,
	JOIN,
	QUIT,
	CAP,
	TOPIC,
	PRIVMSG,
	AWAY,
	PING,
	MODE,
	KICK,
	INVITE,
	PART,
	WHO,
	WHOIS
};

}

const std::string	cmdToString(int cmd);
int					cmdToEnum(const std::string &cmd);

#endif
