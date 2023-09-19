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

		void set(unsigned int pos, std::string line);
		void remove(unsigned int pos);

	private:
		std::map<unsigned int, std::string> history;

		void update();
		void clear();
};

#endif
