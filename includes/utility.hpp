#ifndef UTILITY_HPP
# define UTILITY_HPP

# include <string>
# include <ostream>
# include <iostream>
# include <stdlib.h>

enum {CONTINUE, EXIT};

void error(std::string str, bool quit);

#endif