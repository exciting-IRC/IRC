
NAME := ircserv

MAKEFLAGS += -j

CXX := c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic

CXXFLAGS += -I src
CXXFLAGS += -MMD -MP

SRC := $(shell find src -name '*.cpp')
OBJ := $(SRC:%.cpp=%.o)
DEP := $(OBJ:%.o=%.d)

$(NAME): $(OBJ)
	@echo "\n$(GRN)Linking...$(END)"
	@$(CXX) $(CXXFLAGS) -o $@ $^

-include $(DEP)

%.o: %.cpp
	@echo "$(YEL)$@$(END)"
	@$(CXX) $(CXXFLAGS) -o $@ -c $<

all: $(NAME)

clean:
	rm -f $(OBJ) $(DEP)

fclean: clean
	rm -f $(NAME)

re: fclean
	$(MAKE)

run: all
	./$(NAME)

say:
	@echo $(SRC)

include color.mk
