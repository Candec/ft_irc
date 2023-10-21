/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:19 by tpereira          #+#    #+#             */
/*   Updated: 2023/10/21 10:16:43 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILITY_HPP
# define UTILITY_HPP

# include "main.hpp"

enum {CONTINUE, EXIT};

void error(const string str, bool exit);
const string toString(const int n);
const string timestamp();
bool isValidChannelName(const string& name);

#endif
