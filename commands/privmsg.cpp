/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouhadou <aouhadou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 07:01:49 by waelhamd          #+#    #+#             */
/*   Updated: 2023/09/29 21:08:36 by aouhadou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/headers.hpp"

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

std::string		server::privmsg_response(std::string buff, Client &client)
{
	std::vector<std::string>	split;
	std::vector<std::string>	recipient;

	if(!client.get_print())
		return (":localhost 451 * PRIVMSG :You must finish connecting with nickname first.\r\n");
	split_command(buff, split);
	if (split.size() == 1)
		return (client.get_fd(), ":" + client.get_nick() + " 461 PRIVMSG :Not enough parameters\r\n");
	else if (split.size() == 2)
		return (client.get_fd(), ":" + client.get_nick() + " 412 PRIVMSG :No text to send\r\n");
	else
	{
		recipient = ft_split(split[1], ','); //split recipient
		std::vector<std::string>::iterator it = recipient.begin();
		for(; it != recipient.end(); it++)
		{
			if (this->_channels.find(*it) != this->_channels.end() && client.check_member(*it))//if recipient is a channel
				this->_channels[*it].broadcast_message(":" + client.get_nick() + " PRIVMSG " + *it + " :" + split[2] + "\r\n", client.get_fd());
			else if (getClientFd(this->_map, *it))//if recipient is a user
			{
				std::string tmp = ":" + client.get_nick() + " PRIVMSG " + *it + " :" + split[2] + "\r\n";
				send(getClientFd(this->_map, *it), tmp.c_str(), tmp.size(), 0);
			}
			else
				mysend(client.get_fd(), "401 " + client.get_nick() + " " + *it + " :No such nick/channel\r\n");
		}
	}
	return ("");
}