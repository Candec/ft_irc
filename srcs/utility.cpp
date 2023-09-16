/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:06 by tpereira          #+#    #+#             */
/*   Updated: 2023/09/16 13:20:39 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utility.hpp"

void error(std::string str, bool exit)
{
	std::cout << "Error: " << str << std::endl;
	if (exit)
		exit(EXIT_FAILURE);
}