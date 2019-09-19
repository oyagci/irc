CC			:= gcc
CFLAGS		:= -Wall -Wextra -Werror -g

SOURCES		:= \
	../parser/message.c \
	../parser/prefix.c \
	../parser/command.c \
	../parser/crlf.c \
	../parser/params.c \
	../parser/nickname.c \
	../parser/digit.c \
	../parser/letter.c\
	../parser/special.c \
	../parser/is_nospcrlfcl.c \
	client.c \
	parse_input.c \
	channels.c \
	channels_rmnick.c \
	client_run.c \
	read_notif.c \
	quit.c \
	event.c \
	eventmotd.c \
	eventjoin.c \
	eventpart.c \
	eventping.c \
	eventprivmsg.c \
	messages.c \
	init.c \
	connect.c \
	nick.c \
	user.c \
	leave.c \
	who.c \
	join.c \
	whoreply.c \
	channels_destroy.c

OBJS 		:= $(SOURCES:.c=.o)
OUTDIR		:= ../..
INCLUDES	:= -I../../libs/libft/includes -I../../libs/cbuf/ -I../../inc/common -I../../inc/client -I../../inc/parser
LIBS		:= -L../../libs/libft -lft -L../../libs/cbuf -lcbuf

NAME		:= $(OUTDIR)/client

all: $(NAME)

$(NAME): ../../libs/cbuf/libcbuf.a ../../libs/libft/libft.a $(OBJS)
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $< -c -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

../../libs/cbuf/libcbuf.a:
	make -C ../cbuf

../../libs/libft/libft.a:
	make -C ../libft

depend: .depend

.depend: $(SOURCES)
	rm -f ./.depend
	$(CC) $(CFLAGS) $(INCLUDES) -MM $^ > ./.depend

include .depend
