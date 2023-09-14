
# CC	=				c++
# CFLAGS =			-Wall -Werror -Wextra -std=c++98 -g
# RM =				rm -rf

# NAME =				ircserv
# HEADERS =			includes

# SRCS_DIR = 			srcs
# OBJS_DIR =			objs

# SRCS =				$(addprefix $(SRCS_DIR)/,$(wildcard *.cpp))
# OBJS =				$(addprefix $(OBJS_DIR)/,$(:.cpp=.o))

# all:				$(NAME)

# $(NAME):			$(OBJS)
# 	@$(CC) $(CFLAGS) -o $(NAME) -I $(HEADERS) $(OBJS)

# $(OBJS_DIR)/%.o:	%.cpp
# 	@mkdir -p $(dir $@)
# 	@$(CC) $(CFLAGS) -I $(HEADERS) -c $< -o $@

# clean:
# 	@$(RM) $(OBJS_DIR)

# fclean:				clean
# 	@echo $(ANSI_B_RED) "Fully Cleaning" $(ANSI_RESET)$(ANSI_F_BBLACK)
# 	@$(RM) $(NAME)
# 	@echo "Successfuly cleaned all executable files of $(NAME)!"

# re:					fclean	all

# rebonus:			fclean	bonus


# colors
ANSI_RESET = "\033[0m"
ANSI_B_RED = "\033[41m"
ANSI_B_BGREEN = "\033[42;1m"
ANSI_F_BRED = "\033[31;1m"
ANSI_F_BBLACK = "\033[30;1m"

NAME =				ircserv
CC =				c++
CFLAGS =			-Wall -Werror -Wextra  -g
CPPFLAGS =			-std=c++98 -MMD -MP -I$(HEADERS_DIR)
RM =				rm -rf


OBJECTS_DIR =		objs
SOURCES_DIR =		srcs
HEADERS_DIR =		includes

# OBJECTS = $(addsuffix .o, $(addprefix $(OBJECTS_DIR)/, $(basename $(SOURCES:$(SOURCES_DIR)/%=%))))
OBJECTS =			$(patsubst $(SOURCES_DIR)/%.cpp,$(OBJECTS_DIR)/%.o,$(wildcard $(SOURCES_DIR)/*.cpp))
SOURCES =			$(shell find $(SOURCES_DIR) -name "*.cpp")
HEADERS =			$(wildcard *.hpp)

all: $(NAME)

$(OBJECTS_DIR)/%.o: $(SOURCES_DIR)/%.cpp $(HEADERS:%=$(HEADERS_DIR)/%)
# $(OBJECTS_DIR)/%.o: $(SOURCES_DIR)/%.cpp
	@echo $(ANSI_B_BGREEN) "compiling $@" $(ANSI_RESET)$(ANSI_F_BBLACK)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(NAME): $(OBJECTS)
	@echo $(ANSI_B_BGREEN) "compiling $(NAME)" $(ANSI_RESET)$(ANSI_F_BBLACK)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^
	@echo "$(NAME) mandatory successfully compiled!"

clean:
	@echo $(ANSI_B_RED) "Cleaning" $(ANSI_RESET)$(ANSI_F_BBLACK)
	$(RM) $(OBJECTS_DIR)
	@echo "Successfuly cleaned all object files of $(NAME)!"

fclean: clean
	@echo $(ANSI_B_RED) "Fully Cleaning" $(ANSI_RESET)$(ANSI_F_BBLACK)
	@$(RM) $(NAME)
	@echo "Successfuly cleaned all executable files of $(NAME)!"

re: fclean all

.PHONY: all clean fclean re