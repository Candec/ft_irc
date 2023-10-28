/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:19 by tpereira          #+#    #+#             */
/*   Updated: 2023/10/28 18:09:54 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILITY_HPP
# define UTILITY_HPP

# include <string>
# include <vector>
# include <iostream>
# include <fstream>
# include <sstream>

# include "color.hpp"
# include "channel.hpp"
# include "user.hpp"
// # include "main.hpp"

enum {CONTINUE, EXIT};

void log(const std::string &info);
void error(const std::string &str, const bool exit);

// template<typename T>
// const std::string toString(const T n);
const std::string	toString(const int n);
const std::string	toString(const size_t n);
const std::string	codeToString(const int code);

const std::string	timestamp();
bool				isValidChannelName(const std::string& name);

const std::vector<std::string>	splitString(std::string str, const std::string &delimiter);
const std::string				joinStrings(const std::vector<std::string> &strings);

#endif
