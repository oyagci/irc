CC			:= gcc
CFLAGS		:= -Wall -Wextra -Werror -g

SOURCES		:= \
	../parser/message.c \
	../parser/prefix.c \
	../parser/command.c \
	../parser/crlf.c \
	../parser/params.c \
	../parser/channels.c \
	../parser/digit.c \
	../parser/letter.c\
	../parser/special.c \
	../parser/msgto.c \
	../parser/chanstr.c \
	../parser/nickname.c \
	../parser/is_nospcrlfcl.c \
	server.c \
	accept_new_clients.c \
	server_queue.c \
	read_client_commands.c \
	nick.c \
	set_usermode.c \
	irc_join.c \
	irc_nick.c \
	irc_part.c \
	irc_pass.c \
	irc_privmsg.c \
	irc_user.c \
	irc_who.c \
	irc_ping.c \
	irc_pong.c \
	irc_quit.c \
	server_channel.c \
	run.c \
	server_init.c \
	channel.c \
	notify.c \
	pinginactive.c \
	quit.c \
	get_client.c \
	del_client.c \
	update_clients.c \
	replies/rpl_welcome.c \
	replies/err_unknowncmd.c \
	replies/err_nickinuse.c \
	replies/err_erroneusnick.c
HEADERS		:= \
	server.h \
	../reply_codes.h \
	../parser/parser.h
OBJS 		:= $(SOURCES:.c=.o)
OUTDIR		:= ..
INCLUDES	:= -I./ -I../ -I../libft/includes -I../cbuf/
LIBS		:= -L../libft -lft -L../cbuf -lcbuf

NAME		:= $(OUTDIR)/server

all: $(NAME)

$(NAME): ../cbuf/libcbuf.a ../libft/libft.a $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) $< -c -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

../cbuf/libcbuf.a:
	make -C ../cbuf

../libft/libft.a:
	make -C ../libft
