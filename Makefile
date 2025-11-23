NAME = n_puzzle
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17

SRCS = npzzle.cpp
OBJS = $(SRCS:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean : 
	rm -f $(OBJS)

fclean : clean
	rm -f $(NAME)

re : 
	make fclean
	make all

.PHONY: all clean fclean re

