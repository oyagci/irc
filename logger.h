/*                                                        :::      ::::::::   */
/*   logger.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 13:19:55 by oyagci            #+#    #+#             */
/*   Updated: 2019/08/01 14:01:27 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** 2: Error
** 1: Warning
** 0: Debug
*/
#define LOGLEVEL		0

#define LOGDEBUG		0
#define LOGWARN			1
#define LOGERR			2

#define PREFIX_ERR		"[  ERROR]"
#define PREFIX_WARN		"[WARNING]"
#define PREFIX_DEBUG	"[  DEBUG]"
#define PREFIX_DEFAULT	"[    LOG]"

# include <stdio.h>

# define LOG(lvl, ...) \
	do {\
		if (lvl >= LOGLEVEL) {\
			printprefix(lvl);\
			printf(__VA_ARGS__);\
			printf("\n");\
		}\
	} while (0);

void	msglog(int level, char *s);

void	printprefix(int level);