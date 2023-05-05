NAME = ircserv

OBJ_PATH	= objs

HEADERS		= includes

SRCSPATH 	= srcs

SRCS =  main.cpp  \
		Server.cpp

SERV_PATH = server

CLIENT_PATH = client

CHANNEL_PATH = channel

CXX = c++
CXXFLAGS = -MMD -Wall -Wextra -Werror -std=c++98 -g3
RM = rm -rf
OBJS		=   $(addprefix $(OBJ_PATH)/,$(SRCS:.cpp=.o))	 \
		
DEPS		= $(OBJS:.o=.d)

vpath %.h $(HEADERS)
vpath %.cpp $(SRCSPATH) 					\
			$(SRCSPATH)/$(SERV_PATH)		\
			$(SRCSPATH)/$(CLIENT_PATH)		\
			$(SRCSPATH)/$(CHANNEL_PATH) 	\
vpath %.o $(OBJ_PATH)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) -I $(HEADERS) -o $(NAME)

$(OBJS)			: | $(OBJ_PATH)

$(OBJ_PATH)/%.o	: %.cpp Makefile
			$(CXX) $(CXXFLAGS) -I $(HEADERS) -c $< -o $@


$(OBJ_PATH)		:
	@mkdir -p $(OBJ_PATH)

clean:
	@$(RM)  $(OBJ_PATH)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re	