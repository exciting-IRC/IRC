NAME := msbot
MAKEFLAGS += -j

CXX := c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic -g -fsanitize=address
CXXFLAGS += -I . -I ../src
CXXFLAGS += -MMD -MP

SRC := $(shell find ../src -name '*.cpp' ! -name 'main.cpp') $(shell find . -name '*.cpp')
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

.PHONY: all clean fclean re run

include ../color.mk
