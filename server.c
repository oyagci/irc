#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CONN	10

int	main(int ac, char *av[])
{
	char				buffer[512];
	struct sockaddr_in	serv_addr;
	struct sockaddr_in	cli_addr;
	socklen_t			cli_len;
	fd_set				readfds;
	int					sockfd;
	int					confd;
	int					clients[MAX_CONN];

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

	cli_len = sizeof(cli_addr);

	while (42) {
		FD_ZERO(&readfds);
		FD_SET(sockfd, &readfds);

		for (int i = 0; i < MAX_CONN; i++) {
			if (clients[i] != 0) {
				FD_SET(clients[i], &readfds);
			}
		}

		select(sockfd + 1, &readfds, NULL, NULL, NULL);

		// Incomming new connection
		if (FD_ISSET(sockfd, &readfds)) {
			printf("Incomming connection...\n");
			confd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_len);

			// Add new connection to the clients list
			for (int i = 0; i < MAX_CONN; i++) {
				if (clients[i] == 0) {
					clients[i] = confd;
				}
			}

			printf("Client said: %s\n", buffer);
		}

		for (int i = 0; i < MAX_CONN; i++) {
			if (clients[i] > 0 && FD_ISSET(clients[i], &readfds)) {
				memset(buffer, 0, 512);
				read(confd, buffer, 512);
			}
		}
	}


	close(sockfd);
	return (0);
}
