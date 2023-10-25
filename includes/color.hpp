#ifndef COLOR_HPP
# define COLOR_HPP

# define RESET "\033[0m"
// # define CLEAR "\033[2J"

enum {red, black, green, yellow, blue, pink, cyan, white, none};

# define RED_BG "\033[41m"
# define GREEN_BG "\033[42;1m"
# define YELLOW_BG "\033[43;1m"
# define BLUE_BG "\033[44;1m"
# define MAGENTA_BG "\033[45;1m"
# define CYAN_BG "\033[46;1m"
# define WHITE_BG "\033[47;1m"
# define BLACK_BG "\033[40;1m"

# define RED "\033[31m"
# define BLACK "\033[30m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define WHITE "\033[37m"

# define RED_BOLD "\033[31;1m"
# define BLACK_BOLD "\033[30;1m"
# define GREEN_BOLD "\033[32;1m"
# define YELLOW_BOLD "\033[33;1m"
# define BLUE_BOLD "\033[34;1m"
# define MAGENTA_BOLD "\033[35;1m"
# define CYAN_BOLD "\033[36;1m"
# define WHITE_BOLD "\033[37;1m"

# define BOLD "\033[1m"
# define UNDERLINE "\033[4m"
# define BLINK "\033[5m"
# define REVERSE "\033[7m"

#endif
