/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:27 by tpereira          #+#    #+#             */
/*   Updated: 2023/09/21 12:13:34 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_HPP
# define HISTORY_HPP

// # include "main.hpp"


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

class History
{
	public:
		History();

		void set(unsigned int pos, std::string line);
		void remove(unsigned int pos);

	private:
		std::map<unsigned int, std::string> history;

		void update();
		void clear();
};

#endif
