/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 10:08:25 by oyagci            #+#    #+#             */
/*   Updated: 2019/08/06 15:10:57 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define CRLF	"\x0d\x0a"

int	main(int ac, char *av[])
{
	char				buffer[512];
	struct sockaddr_in	serv_addr;
	struct hostent		*server;
	int					portno;
	int					sockfd;
	fd_set				readfds;

	if (ac < 3)
	{
		printf("Usage: %s <address> <port>\n", av[0]);
		exit(EXIT_FAILURE);
	}
	portno = atoi(av[2]);

	printf("Opening connection to %s:%d\n", av[1], portno);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server = gethostbyname(av[1]);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	memcpy(server->h_addr, &serv_addr.sin_addr.s_addr,
			server->h_length);
	serv_addr.sin_port = htons(portno);

	if (connect(sockfd, (struct sockaddr *)&serv_addr,
				sizeof(serv_addr)) < 0) {
		printf("Could not connect\n");
		exit(1);
	}

	struct timeval t;

	int run = 1;
	while (run) {
		memset(buffer, 0, 512);
		printf("> ");

		fgets(buffer, 510, stdin);

		char *c = strchr(buffer, '\n');
		if (c) {
			*c = '\0';
		}
		strcat(buffer, CRLF);

		if (send(sockfd, buffer, 512, 0) < 0) {
			printf("Connection lost!\n");
			run = 0;
		}
		printf("data sent\n");

		t.tv_usec = 1000;

		FD_ZERO(&readfds);
		FD_SET(sockfd, &readfds);
		select(sockfd + 1, &readfds, NULL, NULL, &t);

		printf("ok\n");
		if (FD_ISSET(sockfd, &readfds))
		{
			printf("receiving data...\n");
			recv(sockfd, buffer, 512, MSG_WAITALL);
			printf("%.512s\n", buffer);
		}
	}
	close(sockfd);
	return (0);
}
