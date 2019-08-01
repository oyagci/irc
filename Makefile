SERVER_SOURCES := \
	server.c \
	message.c \
	prefix.c \
	command.c \
	crlf.c \
	logger.c \
	params.c

CLIENT_SOURCES := client.c

SERVER_OBJS := $(SERVER_SOURCES:.c=.o)
CLIENT_OBJS := client.o

SERVER_NAME := server
CLIENT_NAME := client

CFLAGS := -Wall -Wextra -Werror

.PHONY: all client server

all: $(SERVER_NAME) $(CLIENT_NAME)

server: $(SERVER_NAME)

client: $(CLIENT_NAME)

$(SERVER_NAME): $(SERVER_OBJS)
	$(CC) $(SERVER_OBJS) -o $(SERVER_NAME) -L libft -lft -g

$(CLIENT_NAME): $(CLIENT_OBJS)
	$(CC) $(CLIENT_OBJS) -o $(CLIENT_NAME)

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@ -I libft/includes
