/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 11:06:59 by jibanez-          #+#    #+#             */
/*   Updated: 2023/10/18 11:41:32 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include "main.hpp"

# define WELCOME_MSG "Welcome to 42IRC server.\n"
# define NETWORK_NAME "42IRC"
# define SEPARATOR_MSG "<++++++++++++++++++++++++++++++>\n"
# define COMMAND_MSG "Use one of the following commands:"
# define CMD_LIST_MSG "\nNICK [nickname]\nNAME [name]\nCOLOR [color]\nJOIN [channel]\nKICK [user]"
# define WLC_CH_MSG "Welcome to "

# define COLOR_ERR "Color not found\nTry: red, black, green, yellow, blue, pink cyan, white, none."
# define CH_NAMING_ERR "Channel name format is not valid."
# define TIMEOUT_ERR "You have been disconnected from the server due to innactivity. Press <Enter> to close the client."

# define CLEAR_MSG "\033[2J"

#endif
