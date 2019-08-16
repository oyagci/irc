CC			= clang
CFLAGS		= -Wall -Wextra -Werror

OBJDIR		= objs
SRCDIR		= .
DEPSDIR		= .

SOURCES_RAW	= cbuf_capacity.c \
			  cbuf_empty.c \
			  cbuf_full.c \
			  cbuf_init.c \
			  cbuf_put.c \
			  cbuf_reset.c \
			  cbuf_size.c \
			  cbuf_get.c
OBJECTS_RAW	= $(SOURCES_RAW:.c=.o)
DEPS_RAW	= cbuf.h

SOURCES		= $(addprefix $(SRCDIR)/,$(SOURCES_RAW))
OBJECTS		= $(addprefix $(OBJDIR)/,$(OBJECTS_RAW))
DEPS		= $(addprefix $(DEPSDIR)/,$(DEPS_RAW))

INCLUDES	= -I./

NAME		= libcbuf.a

# ============================================================================ #

.PHONY: all clean fclean re
all: $(NAME)

$(NAME): $(OBJECTS) $(DEPS)
	@echo "Creating $(NAME)..."
	ar rc $(NAME) $(OBJECTS)
	ranlib $(NAME)
	@echo "Done $(NAME)!"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDES)

clean:
	@$(RM) $(OBJECTS)
	@$(RM) -R $(OBJDIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean all
