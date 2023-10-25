/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 11:26:25 by fporto            #+#    #+#             */
/*   Updated: 2023/10/24 17:50:41 by fporto           ###   ########.fr       */
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
	MODE
};

}

const std::string	cmdToString(int cmd);
int					cmdToEnum(const std::string &cmd);

// using Commands::e_Cmds;

#endif
