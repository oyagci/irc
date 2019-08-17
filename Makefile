.PHONY: all clean fclean server client

all: server client

server:
	make -C server

client:
	make -C client

clean:
	make -C client clean
	make -C server clean

fclean: clean
	make -C server fclean || (exit 0)
	make -C client fclean || (exit 0)

re: fclean all
