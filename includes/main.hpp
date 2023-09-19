/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 17:05:34 by tpereira          #+#    #+#             */
/*   Updated: 2023/09/19 12:13:03 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_HPP
# define MAIN_HPP

# define RESET "\033[0m"

# define RED_BG "\033[41m"
# define GREEN_BG "\033[42;1m"
# define YELLOW_BG "\033[43;1m"
# define BLUE_BG "\033[44;1m"
# define MAGENTA_BG "\033[45;1m"
# define CYAN_BG "\033[46;1m"
# define WHITE_BG "\033[47;1m"
# define BLACK_BG "\033[40;1m"

# define RED "\033[31m"
# define BLACK "\033[30m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define WHITE "\033[37m"

# define RED_BOLD "\033[31;1m"
# define BLACK_BOLD "\033[30;1m"
# define GREEN_BOLD "\033[32;1m"
# define YELLOW_BOLD "\033[33;1m"
# define BLUE_BOLD "\033[34;1m"
# define MAGENTA_BOLD "\033[35;1m"
# define CYAN_BOLD "\033[36;1m"
# define WHITE_BOLD "\033[37;1m"

# define BOLD "\033[1m"
# define UNDERLINE "\033[4m"
# define BLINK "\033[5m"
# define REVERSE "\033[7m"

/*
Include everything required to use the following:
socket, close, setsockopt, getsockname,
getprotobyname, gethostbyname, getaddrinfo,
freeaddrinfo, bind, connect, listen, accept, htons,
htonl, ntohs, ntohl, inet_addr, inet_ntoa, send,
recv, signal, sigaction, lseek, fstat, fcntl, poll 
*/


# include <iostream>
# include <string>
# include <map>
# include <vector>
# include <algorithm>
# include <ostream>
# include <sstream>
# include <vector>

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <fcntl.h>
# include <poll.h>

# include <cstdlib>
# include <cstdio>
# include <ctime>

# include "utility.hpp"
# include "user.hpp"
# include "channel.hpp"
# include "history.hpp"
# include "server.hpp"
// # include "Message.hpp"


#endif
