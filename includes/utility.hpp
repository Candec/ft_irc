/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:19 by tpereira          #+#    #+#             */
/*   Updated: 2023/10/01 03:06:32 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILITY_HPP
# define UTILITY_HPP

# include "main.hpp"

enum {CONTINUE, EXIT};

void error(std::string str, bool exit);
template <typename T> bool expectedArgs(T args, size_t n);


#endif
