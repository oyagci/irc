SERVER_SOURCES := \
	parser/message.c \
	parser/prefix.c \
	parser/command.c \
	parser/crlf.c \
	parser/params.c \
	parser/channels.c \
	parser/digit.c \
	parser/letter.c\
	parser/special.c \
	parser/msgto.c \
	parser/chanstr.c \
	parser/nickname.c \
	server/server.c \
	server/set_fds.c \
	server/accept_new_clients.c \
	server/server_queue.c \
	server/nick.c \
	server/set_usermode.c \
	server/irc_join.c \
	server/irc_nick.c \
	server/irc_part.c \
	server/irc_pass.c \
	server/irc_privmsg.c \
	server/irc_user.c \
	server/irc_who.c \
	server/irc_ping.c \
	server/irc_pong.c \
	server/server_channel.c \
	server/server_loop.c \
	server/server_init.c \
	server/channel.c \
	server/notify.c \
	server/pinginactive.c \
	server/quit.c \
	logger.c

CLIENT_SOURCES := \
	parser/message.c \
	parser/prefix.c \
	parser/command.c \
	parser/crlf.c \
	parser/params.c \
	parser/nickname.c \
	parser/digit.c \
	parser/letter.c\
	parser/special.c \
	client/client.c \
	client/parse_input.c \
	client/channels.c \
	client/client_run.c \
	logger.c \

SERVER_OBJS := $(SERVER_SOURCES:.c=.o)
CLIENT_OBJS := $(CLIENT_SOURCES:.c=.o)

SERVER_NAME := bin/server
CLIENT_NAME := bin/client

CFLAGS := -Wall -Wextra -Werror

CC := gcc

SERVER_HEADERS := \
	server/server.h \
	logger.h \
	reply_codes.h \
	parser/parser.h

CLIENT_HEADERS := \
	client/client.h \
	reply_codes.h \
	logger.h \
	parser/parser.h \
	client/channels.h

SERVER_INCLUDES := -I server -I $(shell pwd)
CLIENT_INCLUDES := -I client -I cbuf/

.PHONY: all clean fclean

all: $(SERVER_NAME) $(CLIENT_NAME)

$(SERVER_NAME): $(SERVER_OBJS) $(SERVER_HEADERS) bin
	$(CC) $(SERVER_OBJS) -o $@ -L libft -L cbuf -lft -g -lbsd -lcbuf

$(CLIENT_NAME): $(CLIENT_OBJS) $(CLIENT_HEADERS) bin
	$(CC) $(CLIENT_OBJS) -o $@ -g -L libft -L cbuf -lft -g -lcbuf

%.o: %.c $(SERVER_HEADERS) $(CLIENT_HEADERS)
	$(CC) $(SERVER_INCLUDES) $(CLIENT_INCLUDES) $(CFLAGS) $< -c -o $@ -I libft/includes -g

clean:
	rm -f $(SERVER_OBJS) $(CLIENT_OBJS)

fclean: clean
	rm -f $(SERVER_NAME) $(CLIENT_NAME)
	rmdir bin

re: fclean all

bin:
	mkdir -p bin
