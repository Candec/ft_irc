NAME 		= ircserv
CXX			= g++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98 -pedantic -g -fsanitize=address
SRCS		= ./srcs/*.cpp
INC			= ./includes

CXXFLAGS += -Wpedantic -Werror=pedantic -pedantic-errors
CXXFLAGS += -Wcast-qual -Wdisabled-optimization -Wformat=2 -Wuninitialized
CXXFLAGS += -Winit-self -Wmissing-include-dirs -Wredundant-decls -Wshadow
CXXFLAGS += -Wstrict-overflow=5 -Wundef -fdiagnostics-show-option
CXXFLAGS += -fstack-protector-all
# CXXFLAGS += -Wctor-dtor-privacy -Wold-style-cast -Woverloaded-virtual
CXXFLAGS += -Wsign-promo
CXXFLAGS += -fno-omit-frame-pointer -fno-common
CXXFLAGS += -fsanitize=pointer-subtract -fsanitize=pointer-compare
CXXFLAGS += -fsanitize=undefined

all : $(NAME)

$(NAME) : $(SRCS) $(INC)
	@$(CXX) $(CXXFLAGS) $(SRCS) -I$(INC) -o $(NAME)
	@printf "\e[38;5;46m ╔════════════════════════════════════╗\e[0m\n"
	@printf "\e[38;5;46m█▓▒░       Compiled IRC Server      ░▒▓█ \e[0m\n"
	@printf "\e[38;5;46m ╚════════════════════════════════════╝\e[0m\n"

clean :
	@rm -rf $(NAME) *.dSYM *.out
	@printf "\e[31;5;200m ╔════════════════════════════════════╗\e[0m\n"
	@printf "\e[31;5;200m░▒▓█         Clean complete         █▓▒░\e[0m\n"
	@printf "\e[31;5;200m ╚════════════════════════════════════╝\e[0m\n"

fclean : clean
	@rm -rf $(NAME)

re : fclean all
