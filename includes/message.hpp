/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 11:06:59 by jibanez-          #+#    #+#             */
/*   Updated: 2023/10/21 10:14:49 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include "main.hpp"

# define WELCOME_MSG "Welcome to 42IRC server.\n"
# define SEPARATOR_MSG "<++++++++++++++++++++++++++++++>\n"
# define COMMAND_MSG "Use one of the following commands:"
# define CMD_LIST_MSG "\nNICK [nickname]\nNAME [name]\nCOLOR [color]\nJOIN [channel]\nKICK [user]"
# define WLC_CH_MSG "Welcome to "

# define COLOR_ERR "Color not found\nTry: red, black, green, yellow, blue, pink cyan, white, none."
# define CH_NAMING_ERR "Channel name format is not valid."
# define TIMEOUT_ERR "You have been disconnected from the server due to innactivity. Press <Enter> to close the client."

# define CLEAR_MSG "\033[2J"

#endif
