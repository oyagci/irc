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
	client.c \
	parse_input.c \
	channels.c \
	client_run.c \
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
	../logger.c

HEADERS		:= \
	client.h \
	channels.h \
	../parser/parser.h \
	../reply_codes.h \
	../logger.h

OBJS 		:= $(SOURCES:.c=.o)
OUTDIR		:= ../bin
INCLUDES	:= -I./ -I../ -I../libft/includes -I../cbuf/
LIBS		:= -L../libft -lft -L../cbuf -lcbuf

NAME		:= $(OUTDIR)/client

all: $(NAME)

$(NAME): ../cbuf/libcbuf.a ../libft/libft.a $(OBJS) $(HEADERS)
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS) -lbsd

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) $< -c -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
	rmdir $(OUTDIR)

re: fclean all

../cbuf/libcbuf.a:
	make -C ../cbuf

../libft/libft.a:
	make -C ../libft

