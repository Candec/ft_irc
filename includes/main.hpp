/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 17:05:34 by tpereira          #+#    #+#             */
/*   Updated: 2023/11/05 03:37:46 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_HPP
# define MAIN_HPP

const int SENDING_ERROR = -1;
// const int QUIT = -1;
// # define QUIT -1
# define BUFFER 1024
# define MESSAGE_END "\r\n"

# define LOG_PINGS 0



/*
Include everything required to use the following:
socket, close, setsockopt, getsockname,
getprotobyname, gethostbyname, getaddrinfo,
freeaddrinfo, bind, connect, listen, accept, htons,
htonl, ntohs, ntohl, inet_addr, inet_ntoa, send,
recv, signal, sigaction, lseek, fstat, fcntl, poll
*/

// using namespace std;

# include <string>
# include <map>
# include <vector>
# include <algorithm>
# include <iostream>
# include <ostream>
# include <fstream>
# include <sstream>


# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <stdint.h>
# include <fcntl.h>
# include <netdb.h>
# include <poll.h>

# include <cstdlib>
# include <cstdio>
# include <ctime>
# include <csignal>
# include <cstring>

// # include "history.hpp"
# include "color.hpp"
# include "numericreplies.hpp"
# include "commands.hpp"
# include "user.hpp"
# include "channel.hpp"
# include "server.hpp"
# include "utility.hpp"
# include "message.hpp"

extern bool logging;
extern bool logToFile;
extern std::string startTime;

#endif
