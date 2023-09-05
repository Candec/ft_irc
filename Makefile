CC	=				c++
CFLAGS =			-Wall -Werror -Wextra -std=c++98 -g
RM =				rm -rf

NAME =				ircserv
SERVER_HEADERS =	./Server/includes/$(wildcard *.hpp)
USER_HEADERS =		./User/includes/$(wildcard *.hpp)

SRCS_DIR = 			srcs
OBJS_DIR =			objs

SRCS =				$(wildcard *.cpp)
OBJS =				$(addprefix $(OBJS_DIR)/,$(SRCS:.cpp=.o))

all:				$(NAME)

$(NAME):			$(OBJS)
	@echo $(ANSI_B_BGREEN) "compiling $(NAME)" $(ANSI_RESET)$(ANSI_F_BBLACK)
	@$(CC) $(CFLAGS) -o $(NAME) -I $(USER_HEADERS) -I $(SERVER_HEADERS)  $(OBJS)
	@echo "$(NAME) mandatory successfully compiled!"

$(OBJS_DIR)/%.o:	%.cpp
	@echo $(ANSI_B_BGREEN) "compiling objects" $(ANSI_RESET)$(ANSI_F_BBLACK)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I $(SERVER_HEADERS)  -c $< -o $@
	@$(CC) $(CFLAGS) -I $(USER_HEADERS) -c $< -o $@

clean:
	@echo $(ANSI_B_RED) "Cleaning" $(ANSI_RESET)$(ANSI_F_BBLACK)
	@$(RM) $(OBJS_DIR)
	@echo "Successfuly cleaned all object files of $(NAME)!"

fclean:				clean
	@echo $(ANSI_B_RED) "Fully Cleaning" $(ANSI_RESET)$(ANSI_F_BBLACK)
	@$(RM) $(NAME)
	@echo "Successfuly cleaned all executable files of $(NAME)!"

re:					fclean	all

rebonus:			fclean	bonus

.PHONY:				all	clean	fclean	re

# colors
ANSI_RESET = "\033[0m"
ANSI_B_RED = "\033[41m"
ANSI_B_BGREEN = "\033[42;1m"
ANSI_F_BRED = "\033[31;1m"
ANSI_F_BBLACK = "\033[30;1m"