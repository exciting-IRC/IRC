
NAME := ircserv

MAKEFLAGS += -j

CXX := c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic

CXXFLAGS += -I src
CXXFLAGS += -MMD

SUBDIRS := strutil util logger color general

SRC_UTIL := $(foreach dir, $(SUBDIRS), $(wildcard src/util/$(dir)/*.cpp))

SRC := src/main.cpp $(SRC_UTIL)
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
