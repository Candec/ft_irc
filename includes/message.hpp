/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/11/05 03:38:18 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include "main.hpp"

# define WELCOME_MSG "Welcome to 42IRC server.\n"
# define NETWORK_NAME "42IRC"
# define SEPARATOR_MSG "<++++++++++++++++++++++++++++++>\n"
# define COMMAND_MSG "Use one of the following commands:"
# define CMD_LIST_MSG "\n-NICK [nickname]\n-NAME [name]\n-COLOR [color]\n-JOIN [channel]\n-KICK [user]"
# define WLC_CH_MSG "Welcome to "

# define COLOR_ERR "Color not found\nTry: red, black, green, yellow, blue, pink cyan, white, none."
# define CH_NAMING_ERR "Channel name format is not valid."
# define TIMEOUT_ERR "You have been disconnected from the server due to innactivity. Press <Enter> to close the client."

# define CLEAR_MSG "\033[2J"

#endif
