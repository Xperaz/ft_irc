# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aouhadou <aouhadou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/29 21:12:29 by aouhadou          #+#    #+#              #
#    Updated: 2023/09/29 21:15:10 by aouhadou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

HEADER = includes/headers.hpp includes/server.hpp includes/Client.hpp includes/Channel.hpp
CC=c++
FLAGS= -std=c++98 -Wall -Wextra -Werror
LDLIBS = -lcurl

SRCS=server/main.cpp server/server.cpp utils/helper_func.cpp commands/pass.cpp client/Client.cpp\
	commands/nick.cpp client/user.cpp client/Channel.cpp commands/join.cpp commands/Part.cpp\
	commands/Topic.cpp commands/Quit.cpp commands/invite.cpp commands/privmsg.cpp\
	commands/notice.cpp commands/mode.cpp commands/kick.cpp commands/Bot.cpp
	
OBJS=$(SRCS:.cpp=.o)
NAME=IRC

all:$(NAME)

$(NAME):$(OBJS) $(HEADER)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME) $(LDLIBS)

add : all clean

%.o:%.cpp $(HEADER)
			$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)
	
fclean: clean
	rm -rf $(NAME)

re: fclean $(NAME) clean
.PHONY: all $(NAME) clean fclean re