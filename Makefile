SERVER_SOURCES := \
	server.c \
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
	logger.c \
	set_fds.c \
	accept_new_clients.c \
	server_queue.c \
	nick.c \
	set_usermode.c \
	irc_join.c \
	server_channel.c \
	server_loop.c \
	server_init.c \
	channel.c

CLIENT_SOURCES := \
	client.c \
	parse_input.c \
	logger.c \
	parser/message.c \
	parser/prefix.c \
	parser/command.c \
	parser/crlf.c \
	parser/params.c \
	parser/nickname.c \
	parser/digit.c \
	parser/letter.c\
	parser/special.c \
	channels.c \

SERVER_OBJS := $(SERVER_SOURCES:.c=.o)
CLIENT_OBJS := $(CLIENT_SOURCES:.c=.o)

SERVER_NAME := server
CLIENT_NAME := client

CFLAGS := -Wall -Wextra -Werror

CC := gcc

SERVER_HEADERS := \
	server.h \
	logger.h \
	reply_codes.h \
	parser/parser.h

CLIENT_HEADERS := \
	client.h \
	reply_codes.h \
	logger.h \
	parser/parser.h \
	channels.h

.PHONY: all clean fclean

all: $(SERVER_NAME) $(CLIENT_NAME)

$(SERVER_NAME): $(SERVER_OBJS) $(SERVER_HEADERS)
	$(CC) $(SERVER_OBJS) -o $@ -L libft -lft -g -lbsd

$(CLIENT_NAME): $(CLIENT_OBJS) $(CLIENT_HEADERS)
	$(CC) $(CLIENT_OBJS) -o $@ -g -L libft -lft -g

%.o: %.c $(SERVER_HEADERS) $(CLIENT_HEADERS)
	$(CC) $(CFLAGS) $< -c -o $@ -I libft/includes -g

clean:
	rm -f $(SERVER_OBJS) $(CLIENT_OBJS)

fclean: clean
	rm -f $(SERVER_NAME) $(CLIENT_NAME)

re: fclean all
