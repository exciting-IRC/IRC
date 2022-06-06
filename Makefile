NAME := ircserv

MAKEFLAGS += -j

CXX := c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic

CXXFLAGS += -I src
CXXFLAGS += -MMD

SUBDIRS := strutil util logger main

SRC := $(foreach dir, $(SUBDIRS), $(wildcard src/$(dir)/*.cpp))
OBJ := $(SRC:%.cpp=%.o)
-include $(OBJ:%.o=%.d)

include color.mk

%.o: %.cpp
	@echo "$(YEL)$@$(END)"
	@$(CXX) $(CXXFLAGS) -o $@ -c $<

$(NAME): $(OBJ)
	@echo "\n$(GRN)Linking...$(END)"
	@$(CXX) $(CXXFLAGS) -o $@ $^

all: $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean
	$(MAKE)

run: all
	./$(NAME)
