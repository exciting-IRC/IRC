NAME := ircserv

SRC := $(wildcard *.cpp)
OBJ := $(SRC:.cpp=.o)
# DEP := $(SRC:.o=.d)

$(NAME): $(OBJ)
	$(CXX) $^ -o $(NAME)
