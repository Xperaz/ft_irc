/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: waelhamd <waelhamd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 10:17:42 by waelhamd          #+#    #+#             */
/*   Updated: 2023/04/13 06:06:41 by waelhamd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.hpp"

static void 	split_command(std::string buff, std::vector<std::string> &split)
{
	std::vector<std::string> tmp;
	size_t pos = buff.find(":");
	if (pos != std::string::npos){
		tmp.push_back(buff.substr(0, pos));
		tmp.push_back(buff.substr(pos + 1));
	}
	else
		tmp.push_back(buff);
	split = ft_split(tmp[0], ' ');
	if (tmp.size() > 1)
		split.push_back(tmp[1]);
}

std::string server::notice_response(std::string buff, Client &client)
{
	std::vector<std::string>	split;
	std::vector<std::string>	recipient;

	split_command(buff, split);
	if (split.size() == 1)
		return (client.get_fd(), ":" + client.get_nick() + " 461 NOTICE :Not enough parameters\n");
	else if (split.size() == 2)
		return (client.get_fd(), ":" + client.get_nick() + " 412 NOTICE :No text to send\n");
	else
	{
		recipient = ft_split(split[1], ','); //split recipient
		std::vector<std::string>::iterator it = recipient.begin();
		for(; it != recipient.end(); it++)
		{
			if (this->_channels.find(*it) != this->_channels.end())//if recipient is a channel
			{
				this->_channels[*it].broadcast_message(split[2], client.get_nick(), client.get_fd());
			}
			else if (getClientFd(this->_map, *it))//if recipient is a user
			{
				std::string tmp = ":" + client.get_nick() + " NOTICE " + *it + " :" + split[2] + "\r\n";
				send(getClientFd(this->_map, *it), tmp.c_str(), tmp.size(), 0);
			}
		}
	}
	return ("");
}