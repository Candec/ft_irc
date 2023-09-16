/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:27 by tpereira          #+#    #+#             */
/*   Updated: 2023/09/16 17:09:27 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_HPP
# define HISTORY_HPP

# include "main.hpp"

class History
{
	public:
		History();

		void set(unsigned char pos, std::string line);
		void remove(unsigned char pos);

	private:
		std::map<unsigned char, std::string> history;

		void update();
		void clear();
};

#endif
