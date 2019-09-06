#ifndef REPLY_CODES
# define REPLY_CODES

# define ERR_NOSUCHCHANNEL		403
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

# define SERR_NOSUCHCHANNEL		"403"
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
