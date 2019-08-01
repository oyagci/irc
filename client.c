/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 10:08:25 by oyagci            #+#    #+#             */
/*   Updated: 2019/08/01 13:55:13 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

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

		if (write(sockfd, buffer, 512) < 0) {
			printf("Connection lost!\n");
			run = 0;
		}
	}
	close(sockfd);
	return (0);
}
