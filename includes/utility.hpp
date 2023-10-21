/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:19 by tpereira          #+#    #+#             */
/*   Updated: 2023/10/21 10:03:41 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILITY_HPP
# define UTILITY_HPP

# include "main.hpp"
using std::map;
using std::string;
using std::vector;
using std::ostringstream;
using std::cout;
using std::endl;
using std::flush;
using std::ifstream;
using std::ios;
using std::istringstream;
using std::pair;

enum {CONTINUE, EXIT};

void error(const string str, bool exit);
const string toString(const int n);
const string timestamp();
bool isValidChannelName(const string& name);

#endif
