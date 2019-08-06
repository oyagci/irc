SERVER_SOURCES := \
	server.c \
	message.c \
	prefix.c \
	command.c \
	crlf.c \
	logger.c \
	params.c \
	set_fds.c \
	handle_new_clients.c \
	handle_io_clients.c

CLIENT_SOURCES := client.c

SERVER_OBJS := $(SERVER_SOURCES:.c=.o)
CLIENT_OBJS := client.o

SERVER_NAME := server
CLIENT_NAME := client

CFLAGS := -Wall -Wextra -Werror

CC := gcc

HEADERS := \
	irc.h \
	logger.h

.PHONY: all clean fclean

all: $(SERVER_NAME) $(CLIENT_NAME)

$(SERVER_NAME): $(SERVER_OBJS) $(HEADERS)
	$(CC) $(SERVER_OBJS) -o $@ -L libft -lft -g

$(CLIENT_NAME): $(CLIENT_OBJS) $(HEADERS)
	$(CC) $(CLIENT_OBJS) -o $@ -g

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $< -c -o $@ -I libft/includes -g

clean:
	rm -f $(SERVER_OBJS) $(CLIENT_OBJS)

fclean: clean
	rm -f $(SERVER_NAME) $(CLIENT_NAME)
