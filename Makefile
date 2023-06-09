HEADER = headers.hpp server.hpp Client.hpp Channel.hpp
CC=c++
FLAGS= -std=c++98 -Wall -Wextra -Werror
LDLIBS = -lcurl

SRCS=main.cpp server.cpp helper_func.cpp pass.cpp Client.cpp nick.cpp user.cpp Channel.cpp join.cpp Part.cpp Topic.cpp\
	Quit.cpp invite.cpp privmsg.cpp notice.cpp mode.cpp kick.cpp Bot.cpp
	
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