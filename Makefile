NAME := ircserv
MAKEFLAGS += -j

CXX := c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic -g
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

test:
	./test/joinpart.sh
	./test/change_nick.sh
	./test/quit.sh
	./test/kill.sh
	./test/stress.sh
	./test/manual.sh

bot:
	$(MAKE) -C bot run

.PHONY: all clean fclean re run test bot

include color.mk
