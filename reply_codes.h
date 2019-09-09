/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reply_codes.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 15:13:41 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 15:13:42 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLY_CODES_H
# define REPLY_CODES_H

# define ERR_NOSUCHNICK			401
# define ERR_NOSUCHCHANNEL		403
# define ERR_CANNOTSENDTOCHAN	404
# define ERR_NORECIPIENT		411
# define ERR_NOTEXTTOSEND		412
# define ERR_UNKNOWNCOMMAND		421
# define ERR_NONICKNAMEGIVEN	431
# define ERR_ERRONEUSNICKNAME	432
# define ERR_NICKNAMEINUSE		433
# define ERR_NOTONCHANNEL		442
# define ERR_NEEDMOREPARAM		461
# define ERR_ALREADYREGISTRED	462
# define ERR_PASSWDMISMATCH		464

# define RPL_WELCOME			  1
# define RPL_ENDOFWHO			315
# define RPL_WHOREPLY			352

# define SRPL_WELCOME			"001"
# define SRPL_ENDOFWHO			"315"
# define SRPL_WHOREPLY			"352"

# define SERR_NOSUCHNICK		"401"
# define SERR_NOSUCHCHANNEL		"403"
# define SERR_CANNOTSENDTOCHAN	"404"
# define SERR_NORECIPIENT		"411"
# define SERR_NOTEXTTOSEND		"412"
# define SERR_UNKNOWNCOMMAND	"421"
# define SERR_NONICKNAMEGIVEN	"431"
# define SERR_ERRONEUSNICKNAME	"432"
# define SERR_NICKNAMEINUSE		"433"
# define SERR_NOTONCHANNEL		"442"
# define SERR_NEEDMOREPARAM		"461"
# define SERR_ALREADYREGISTRED	"462"
# define SERR_PASSWDMISMATCH	"464"

#endif
