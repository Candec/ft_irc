/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:22 by tpereira          #+#    #+#             */
/*   Updated: 2023/09/16 18:17:23 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "main.hpp"

class User
{
	private:
		std::string		_name;
		std::string		_password;
		std::string		_email;
		std::string		_phone;
		std::string		_address;
		std::string		_city;
		std::string		_postalCode;
		std::string		_country;
		std::string		_id;
		std::string		_role;
		std::string		_status;
		std::string		_createdAt;
		std::string		_updatedAt;

	public:
		User();
		User(
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
		);
		User(const User &src);
		~User();

		bool operator==(const User &rhs) const;

};

#endif
