NAME := webserv
SRCS := srcs/main.cpp \
		srcs/engine/Connection.cpp \
		srcs/engine/Listener.cpp \
		srcs/engine/PollHandler.cpp \
		srcs/http/Request.cpp \
		srcs/http/http_utils.cpp \
		srcs/parser/CDLocation.cpp \
		srcs/parser/CDParserFile.cpp \
		srcs/parser/CDServer.cpp \
		srcs/parser/parserUtils.cpp \
		
OBJS := $(SRCS:srcs/%.cpp=objs/%.o)
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -I inc -std=c++98
LDFLAGS = -std=c++98
RM = /bin/rm -rf

ARG = "/test/nginxTesting/conf/ngix0.conf"

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $(NAME)

objs:
	@mkdir	objs \
			objs/engine \
			objs/http \
			objs/parser \

objs/%.o: srcs/%.cpp | objs
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) objs
fclean: clean
	$(RM) $(NAME)
re: fclean all

exe:
	./$(NAME) $(ARG)

#check sanitize#
sanitize:: CXXFLAGS += -fsanitize=address -g3
sanitize:: LDFLAGS += -fsanitize=address
sanitize:: re

noflag:: CXXFLAGS = -I inc -std=c++98
noflag:: re

client: test/cli/client.o
	$(CXX) $(LDFLAGS) test/cli/client.o -o client

.PHONY: all clean fclean re sanitize