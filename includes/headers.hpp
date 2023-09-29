/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aouhadou <aouhadou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 21:09:03 by aouhadou          #+#    #+#             */
/*   Updated: 2023/09/29 21:09:04 by aouhadou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADERS_HPP
# define HEADERS_HPP

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h> 
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
# include <poll.h>
# include <sys/select.h>
# include <string.h>
# include <string>
# include<sstream>
# include <stdexcept>
#include <stdlib.h>
#include<map>
#include<vector>
#include <stack>
#include <unordered_map>
# include <algorithm>
#include <curl/curl.h>
#include <cstring>
# include "Client.hpp"
# include "Channel.hpp"
# include "server.hpp"

bool    has_comma(std::string str);
bool    has_semi_colon(std::string str);

#endif