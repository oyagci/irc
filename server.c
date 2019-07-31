#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define MAX_CONN			10
#define COMMAND_LENGTH		512
//#define CRLF				"\x0d\x0a"
#define CRLF				"EOF"
#define CLIENT_BUFFER_SIZE	512

struct s_client_buffer {
	char	data[CLIENT_BUFFER_SIZE];
	int		len;
	short	is_complete;
};

int	read_client_command(int cfd, struct s_client_buffer *buffer)
{
	int	ret;

	/*
	** RFC2812 limits the maximum length of a command sent by
	** the client to 512 bytes at most.
	*/
	if ((ret = read(cfd, buffer->data + buffer->len,
					COMMAND_LENGTH - buffer->len)) > 0)
	{
		buffer->len += ret;
		if (strstr(buffer->data, CRLF)) {
			buffer->is_complete = 1;
		}
	}
	return (ret);
}

struct s_prefix {
	struct s_servername	*servername;
	struct s_nickname	*nickname;
	struct s_user		*user;
	struct s_host		*host;
};

struct s_message {
	struct s_prefix		*prefix;
};

struct s_prefix		*parse_prefix(char *data) {
	struct s_prefix *p;

	p = malloc(sizeof(*p));
	if (!p) {
		return (NULL);
	}
	p->servername = parse_servername(data);
	return (p);
}

struct s_message	*parse_message(char *data)
{
	struct s_message *m;

	m = malloc(sizeof(struct s_message));
	if (!m) {
		return (NULL);
	}
	if (data[0] == ':')
	{
		data += 1;
		m->prefix = parse_prefix(data);
	}
	return (m);
}

int	execute_command(char *data)
{
	struct s_message	*msg;

	msg = parse_message(data);
	return (0);
}

int	main(int ac, char *av[])
{
	struct s_client_buffer	clients_buffer[MAX_CONN];
	int						clients[MAX_CONN];
	struct sockaddr_in		serv_addr;
	struct sockaddr_in		cli_addr;
	socklen_t				cli_len;
	fd_set					readfds;
	int						sockfd;
	int						max_sd;
	int						confd;

	if (ac < 2) {
		printf("Usage: %s <port>\n", av[0]);
		exit(EXIT_FAILURE);
	}

	printf("Creating server on port %s\n", av[1]);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(atoi(av[1]));

	bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	listen(sockfd, MAX_CONN);

	memset(clients, 0, MAX_CONN);
	memset(clients_buffer, 0, sizeof(struct s_client_buffer) * MAX_CONN);

	while (42) {
		cli_len = sizeof(cli_addr);

		FD_ZERO(&readfds);
		FD_SET(sockfd, &readfds);

		max_sd = sockfd;

		for (int i = 0; i < MAX_CONN; i++) {
			if (clients[i] >= 0) {
				FD_SET(clients[i], &readfds);

				// Set the highest fd number for select
				if (clients[i] > max_sd) {
					max_sd = clients[i];
				}
			}
		}

		// Watch incoming data on all clients
		select(max_sd + 1, &readfds, NULL, NULL, NULL);

		// Incomming connection
		if (FD_ISSET(sockfd, &readfds)) {
			confd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_len);

			printf("[INFO] %s: connected\n", inet_ntoa(cli_addr.sin_addr));

			// Add connection to the clients list
			for (int i = 0; i < MAX_CONN; i++) {
				if (clients[i] == 0) {
					clients[i] = confd;
					printf("new connection on fd %d\n", confd);
					break ;
				}
			}
		}

		int ret = 0;
		for (int i = 0; i < MAX_CONN; i++) {
			// Client is sending data
			if (FD_ISSET(clients[i], &readfds)) {
				ret = read_client_command(clients[i], &clients_buffer[i]);
				if (clients_buffer[i].is_complete) {
					execute_command(clients_buffer[i].data);
				}
			}
		}
	}
	return (0);
}
