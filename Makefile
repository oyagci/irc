.PHONY: all clean fclean server client

all: server client

server:
	make -C serversrc

client:
	make -C clientsrc

clean:
	make -C clientsrc clean
	make -C serversrc clean

fclean: clean
	make -C serversrc fclean || (exit 0)
	make -C clientsrc fclean || (exit 0)

re: fclean all
