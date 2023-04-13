/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: waelhamd <waelhamd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 20:22:03 by mabdelba          #+#    #+#             */
/*   Updated: 2023/04/13 09:52:47 by waelhamd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.hpp"

bool check_double(std::map<int, Client> Map, std::string str)
{
	std::map<int, Client>::iterator it;
	if(Map.size() > 0)
	{
		for(it = Map.begin(); it != Map.end(); it++)
		{
			if(it->second.get_nick() == str)
				return true;
		}
	}
	return false;
}

bool check_nickname(std::string nick)
{
	for(size_t i = 0; nick[i]; i++)
	{
		if(!std::isalnum(nick[i]) && nick[i] != '_' && nick[i] != '-' && nick[i] != '.' && nick[i] != '~')
			return false;
	}
	return true;
}

std::string server::nick_response(std::vector<std::string>  split, Client &client)
{
	if(client.get_pass())
	{
		if(split.size() < 2)
			return(":localhost 431 :No nickname given\r\n");
		if(!check_nickname(split[1]))
			return (":localhost 432 :Erroneous nickname\r\n");
		if(check_double(this->_map, split[1]))
			return (":localhost 433  :Nickname already in use\r\n");
		// if(check_double(this->_map, split[1]))
		// 	return(client.get_nick() + " " + split[1] + " :Nickname collision\n");
		client.set_nick(split[1]);
		client.set_reg(true);
		if(!client.get_print() && client.get_auth())
		{
			client.set_print(true);
			welcome_message(client.get_fd() , client);
		}
	}
	return ("");
}