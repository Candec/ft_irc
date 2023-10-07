/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:27 by tpereira          #+#    #+#             */
/*   Updated: 2023/10/07 22:29:23 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_HPP
# define HISTORY_HPP

// # include "main.hpp"

# define CLEAR "\033[2J"
# define RESET "\033[0m"


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
# include <fcntl.h>
# include <netdb.h>
# include <poll.h>

# include <cstdlib>
# include <cstdio>
# include <ctime>

using namespace std;

class Server;
class Channel;

class History
{
	public:
		History();

		void set(string line);

		void remove(unsigned int pos);
		void clear();

		// Server *getServer();
		// Channel *getChannel();


	private:
		map<unsigned int, string> _history;
		Server *_server;
		Channel *_channel;

		void update();
};

#endif
