/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_func.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouhadou <aouhadou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 21:09:24 by aouhadou          #+#    #+#             */
/*   Updated: 2023/09/29 21:15:33 by aouhadou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/headers.hpp"

bool is_valid_arg(int ac, char **av)
{
    if (ac != 3 || av[1][0] == '\0' || av[2][0] == '\0')
    {
        std::cout << "number of argument invalid: try <port> <password>" << std::endl;
        return false;
    }
    return (true);
}

bool has_comma(std::string str)
{
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == ',')
            return true;
    }
    return false;
}

bool    has_semi_colon(std::string str)
{
    size_t pos = str.find(":");
    if (pos != std::string::npos)
        return (true);
    return (false);
}

void	mysend(int fd, std::string message)
{
	send(fd, message.c_str(), message.size(), 0);

}

std::string get_adderss()
{
    char hostname[1024];
    hostname[1023] = '\0';
    gethostname(hostname, 1023);
    struct hostent *host = gethostbyname(hostname);
    if (host)
        return std::string(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    return "localhost";
}