/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:04 by tpereira          #+#    #+#             */
/*   Updated: 2023/09/16 18:17:57 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

User::User()
{
}

User::User(
	std::string name,
	std::string password,
	std::string email,
	std::string phone,
	std::string address,
	std::string city,
	std::string postalCode,
	std::string country,
	std::string id,
	std::string role,
	std::string status,
	std::string createdAt,
	std::string updatedAt
)
{
	this->_name = name;
	this->_password = password;
	this->_email = email;
	this->_phone = phone;
	this->_address = address;
	this->_city = city;
	this->_postalCode = postalCode;
	this->_country = country;
	this->_id = id;
	this->_role = role;
	this->_status = status;
	this->_createdAt = createdAt;
	this->_updatedAt = updatedAt;
}

User::~User()
{
}

bool User::operator==(const User &rhs) const
{
	return (this->_id == rhs._id);
}
