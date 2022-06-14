
NAME := ircserv

MAKEFLAGS += -j

CXX := c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic

CXXFLAGS += -I src
CXXFLAGS += -MMD -MP

SRC_EVENT := \
	$(wildcard src/event/*.cpp) \
	$(foreach dir, kqueue, $(wildcard src/event/$(dir)/*.cpp))
SRC_UTIL := $(foreach dir, strutil util logger color general, $(wildcard src/util/$(dir)/*.cpp))
SRC_SOCKET := $(wildcard src/socket/*.cpp)

SRC := src/main.cpp $(SRC_UTIL) $(SRC_EVENT) $(SRC_SOCKET) $(wildcard src/*.cpp)
OBJ := $(SRC:%.cpp=%.o)

$(NAME): $(OBJ)
	@echo "\n$(GRN)Linking...$(END)"
	@$(CXX) $(CXXFLAGS) -o $@ $^

-include $(OBJ:%.o=%.d)

%.o: %.cpp
	@echo "$(YEL)$@$(END)"
	@$(CXX) $(CXXFLAGS) -o $@ -c $<

all: $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean
	$(MAKE)

run: all
	./$(NAME)

say:
	@echo $(SRC)

include color.mk
