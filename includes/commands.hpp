/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 11:26:25 by fporto            #+#    #+#             */
/*   Updated: 2023/10/20 12:27:16 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef COMMANDS_HPP
# define COMMANDS_HPP

// # include "main.hpp"
# include <string>
# include "utility.hpp"

namespace Commands {

enum e_Cmds {
	UNKNOWN = -1,
	PASS,
	NICK,
	USER,
	COLOR,
	JOIN,
	QUIT,
	CAP
};

const std::string	toString(e_Cmds cmd);
e_Cmds				toEnum(const std::string &cmd);

}

using Commands::e_Cmds;

#endif
