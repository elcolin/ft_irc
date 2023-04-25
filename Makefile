# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: elise <elise@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/25 12:17:14 by elise             #+#    #+#              #
#    Updated: 2023/04/25 15:10:28 by elise            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = c++

NAME = ircserv

CXXFLAGS = -std=c++98 #-Wall -Wextra -Werror

SRC = main.cpp

OBJ = $(SRC:.cpp=.o)

DEP = ft_irc.hpp

%.o: %.cpp $(DEP)
	$(CXX) $(CXXFLAGS) -c $< 

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all